#include "requestreader.h"

#include "sockethandler.h"

#include <QTcpSocket>
#include <QBuffer>
#include <QHostAddress>
#include <QTemporaryFile>

RequestReader::RequestReader(SocketHandler *handler, QObject *parent) :
    QObject(parent),
    m_handler(handler),
    m_status(WaitForRequest),
    m_currentSize(0),
    m_expectedBodySize(0),
    m_maxSize(16000),
    m_maxMultiPartSize(1000000)
{
}

void RequestReader::readRequest(QIODevice &stream)
{
    int toRead = m_maxSize - m_currentSize + 1; // allow one byte more to be able to detect overflow

    QByteArray newData = stream.readLine(toRead).trimmed();
    m_currentSize += newData.size();
    if (!newData.isEmpty()) {
        QList<QByteArray> list = newData.split(' ');
        if (list.count()!=3 || !list.at(2).contains("HTTP")) {
            qWarning("HttpRequest: received broken HTTP request, invalid first line");
            m_status = Abort;
        }
        else {
            m_method=list.at(0);
            m_url = QString(list.at(1));
            m_version=list.at(2);
            setStatus(WaitForHeader);
        }
    }
}

void RequestReader::readHeader(QIODevice &stream)
{
    int toRead = m_maxSize - m_currentSize + 1; // allow one byte more to be able to detect overflow
    QByteArray newData = stream.readLine(toRead).trimmed();
    m_currentSize += newData.size();
    int colon = newData.indexOf(':');
    if (colon > 0) {
        // Received a line with a colon - a header
        m_currentHeader = newData.left(colon);
        QByteArray value = newData.mid(colon+1).trimmed();
        m_headers.insert(m_currentHeader, value);
    }
    else if (!newData.isEmpty()) {
        // Received additional line of previous header
        if (m_headers.contains(m_currentHeader)) {
            m_headers.insert(m_currentHeader,m_headers.value(m_currentHeader)+" "+newData);
        }
    }
    else {
        // received an empty line - end of headers reached
        // Empty line received, that means all headers have been received
        // Check for multipart/form-data
        QByteArray contentType = m_headers.value("Content-Type");
        if (contentType.startsWith("multipart/form-data")) {
            int posi=contentType.indexOf("boundary=");
            if (posi>=0) {
                m_boundary=contentType.mid(posi+9);
            }
        }
        QByteArray contentLength = getHeader("Content-Length");
        if (!contentLength.isEmpty()) {
            m_expectedBodySize = contentLength.toInt();
        }
        if (m_expectedBodySize == 0) {
            setStatus(Complete);
        }
        else if (m_boundary.isEmpty() && m_expectedBodySize+m_currentSize>m_maxSize) {
            qWarning("HttpRequest: expected body is too large");
            setStatus(Abort);
        }
        else if (!m_boundary.isEmpty() && m_expectedBodySize>m_maxMultiPartSize) {
            qWarning("HttpRequest: expected multipart body is too large");
            setStatus(Abort);
        }
        else {
            setStatus(WaitForBody);
        }
    }
}

void RequestReader::readBody(QIODevice &stream)
{
    Q_ASSERT(m_expectedBodySize!=0);
    if (m_boundary.isEmpty()) {
        // normal body, no multipart
        int toRead = m_expectedBodySize-m_bodyData.size();
        QByteArray newData = stream.read(toRead);
        m_currentSize += newData.size();
        m_bodyData.append(newData);
        if (m_bodyData.size()>=m_expectedBodySize) {
            setStatus(Complete);
        }
    }
    else {
        // multipart body, store into temp file
        if (!m_tempFile.isOpen()) {
            m_tempFile.open();
        }
        // Transfer data in 64kb blocks
        int fileSize = m_tempFile.size();
        int toRead = m_expectedBodySize-fileSize;
        if (toRead > 65536) {
            toRead = 65536;
        }
        fileSize += m_tempFile.write(stream.read(toRead));
        if (fileSize >= m_maxMultiPartSize) {
            qWarning("HttpRequest: received too many multipart bytes");
            setStatus(Abort);
        }
        else if (fileSize>=m_expectedBodySize) {
            m_tempFile.flush();
            if (m_tempFile.error()) {
                qCritical("HttpRequest: Error writing temp file for multipart body");
            }
            parseMultiPartFile();
            m_tempFile.close();
            setStatus(Complete);
        }
    }
}

RequestReader::RequestStatus RequestReader::readFromSocket(QTcpSocket *socket)
{
    if (m_status == Complete || m_status == Abort)
        return Unchanged;

    QByteArray newData = socket->readAll();
    m_wholeRequestBody.append(newData);

    QBuffer buffer(&newData);
    buffer.open(QBuffer::ReadOnly);

    while (buffer.bytesAvailable() && m_status != Complete && m_status != Abort) {
        if (m_status == WaitForRequest)
            readRequest(buffer);
        else if (m_status == WaitForHeader)
            readHeader(buffer);
        else if (m_status == WaitForBody)
            readBody(buffer);

        if (m_currentSize > m_maxSize) {
            qWarning("HttpRequest: received too many bytes");
            setStatus(Abort);
        }
    }

    m_peerAddress = socket->peerAddress().toString();
    m_peerPort = socket->peerPort();

    return m_status;
}

/**
  Get the value of a HTTP request header.
  @param name Name of the header
  @return If the header occurs multiple times, only the last
  one is returned.
*/
QByteArray RequestReader::getHeader(const QByteArray &name) const
{
    return m_headers.value(name);
}

void RequestReader::setStatus(RequestReader::RequestStatus status)
{
    m_status = status;
}

void RequestReader::parseMultiPartFile()
{
    qDebug("HttpRequest: parsing multipart temp file");
    m_tempFile.seek(0);
    bool finished=false;
    while (!m_tempFile.atEnd() && !finished && !m_tempFile.error()) {

#ifdef SUPERVERBOSE
        qDebug("HttpRequest: reading multpart headers");
#endif
        QByteArray fieldName;
        QByteArray fileName;
        while (!m_tempFile.atEnd() && !finished && !m_tempFile.error()) {
            QByteArray line=m_tempFile.readLine(65536).trimmed();
            if (line.startsWith("Content-Disposition:")) {
                if (line.contains("form-data")) {
                    int start=line.indexOf(" name=\"");
                    int end=line.indexOf("\"",start+7);
                    if (start>=0 && end>=start) {
                        fieldName=line.mid(start+7,end-start-7);
                    }
                    start=line.indexOf(" filename=\"");
                    end=line.indexOf("\"",start+11);
                    if (start>=0 && end>=start) {
                        fileName=line.mid(start+11,end-start-11);
                    }
#ifdef SUPERVERBOSE
                    qDebug("HttpRequest: multipart field=%s, filename=%s",fieldName.data(),fileName.data());
#endif
                }
                else {
                    qDebug("HttpRequest: ignoring unsupported content part %s",line.data());
                }
            }
            else if (line.isEmpty()) {
                break;
            }
        }

#ifdef SUPERVERBOSE
        qDebug("HttpRequest: reading multpart data");
#endif
        QTemporaryFile* uploadedFile=0;
        QByteArray fieldValue;
        while (!m_tempFile.atEnd() && !finished && !m_tempFile.error()) {
            QByteArray line=m_tempFile.readLine(65536);
            if (line.startsWith("--"+m_boundary)) {
                // Boundary found. Until now we have collected 2 bytes too much,
                // so remove them from the last result
                if (fileName.isEmpty() && !fieldName.isEmpty()) {
                    // last field was a form field
                    fieldValue.remove(fieldValue.size()-2,2);
                    m_parameters.insert(fieldName,fieldValue);
                    qDebug("HttpRequest: set parameter %s=%s",fieldName.data(),fieldValue.data());
                }
                else if (!fileName.isEmpty() && !fieldName.isEmpty()) {
                    // last field was a file
#ifdef SUPERVERBOSE
                    qDebug("HttpRequest: finishing writing to uploaded file");
#endif
                    uploadedFile->resize(uploadedFile->size()-2);
                    uploadedFile->flush();
                    uploadedFile->seek(0);
                    m_parameters.insert(fieldName,fileName);
                    qDebug("HttpRequest: set parameter %s=%s",fieldName.data(),fileName.data());
                    m_uploadedFiles.insert(fieldName,uploadedFile);
                    qDebug("HttpRequest: uploaded file size is %i",(int) uploadedFile->size());
                }
                if (line.contains(m_boundary+"--")) {
                    finished=true;
                }
                break;
            }
            else {
                if (fileName.isEmpty() && !fieldName.isEmpty()) {
                    // this is a form field.
                    m_currentSize+=line.size();
                    fieldValue.append(line);
                }
                else if (!fileName.isEmpty() && !fieldName.isEmpty()) {
                    // this is a file
                    if (!uploadedFile) {
                        uploadedFile=new QTemporaryFile();
                        uploadedFile->open();
                    }
                    uploadedFile->write(line);
                    if (uploadedFile->error()) {
                        qCritical("HttpRequest: error writing temp file, %s",qPrintable(uploadedFile->errorString()));
                    }
                }
            }
        }
    }
    if (m_tempFile.error()) {
        qCritical("HttpRequest: cannot read temp file, %s",qPrintable(m_tempFile.errorString()));
    }
#ifdef SUPERVERBOSE
    qDebug("HttpRequest: finished parsing multipart temp file");
#endif
}
