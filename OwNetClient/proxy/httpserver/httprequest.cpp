/**
  @file
  @author Stefan Frings
*/

#include "httprequest.h"
#include <QList>
#include <QDir>
#include <QHostAddress>

HttpRequest::HttpRequest(QSettings* settings, QObject *parent)
    : QObject(parent)
{
    m_status=waitForRequest;
    m_currentSize=0;
    m_expectedBodySize=0;
    m_maxSize=settings->value("maxRequestSize","16000").toInt();
    m_maxMultiPartSize=settings->value("maxMultiPartSize","1000000").toInt();
}

void HttpRequest::readRequest(QTcpSocket& socket) {
#ifdef SUPERVERBOSE
    qDebug("HttpRequest: read request");
#endif
    int toRead=m_maxSize-m_currentSize+1; // allow one byte more to be able to detect overflow
    QByteArray newData=socket.readLine(toRead).trimmed();
    m_currentSize+=newData.size();
    if (!newData.isEmpty()) {
        QList<QByteArray> list=newData.split(' ');
        if (list.count()!=3 || !list.at(2).contains("HTTP")) {
            qWarning("HttpRequest: received broken HTTP request, invalid first line");
            m_status=abort;
        }
        else {
            m_method=list.at(0);
            m_path=list.at(1);
            m_version=list.at(2);
            m_status=waitForHeader;
            m_url = QString(m_path);
        }
    }
}

void HttpRequest::readHeader(QTcpSocket& socket) {
#ifdef SUPERVERBOSE
    qDebug("HttpRequest: read header");
#endif
    int toRead=m_maxSize-m_currentSize+1; // allow one byte more to be able to detect overflow
    QByteArray newData=socket.readLine(toRead).trimmed();
    m_currentSize+=newData.size();
    int colon=newData.indexOf(':');
    if (colon>0)  {
        // Received a line with a colon - a header
        m_currentHeader=newData.left(colon);
        QByteArray value=newData.mid(colon+1).trimmed();
        m_headers.insert(m_currentHeader,value);
#ifdef SUPERVERBOSE
        qDebug("HttpRequest: received header %s: %s",currentHeader.data(),value.data());
#endif
    }
    else if (!newData.isEmpty()) {
        // received another line - belongs to the previous header
#ifdef SUPERVERBOSE
        qDebug("HttpRequest: read additional line of header");
#endif
        // Received additional line of previous header
        if (m_headers.contains(m_currentHeader)) {
            m_headers.insert(m_currentHeader,m_headers.value(m_currentHeader)+" "+newData);
        }
    }
    else {
        // received an empty line - end of headers reached
#ifdef SUPERVERBOSE
        qDebug("HttpRequest: headers completed");
#endif
        // Empty line received, that means all headers have been received
        // Check for multipart/form-data
        QByteArray contentType=m_headers.value("Content-Type");
        if (contentType.startsWith("multipart/form-data")) {
            int posi=contentType.indexOf("boundary=");
            if (posi>=0) {
                m_boundary=contentType.mid(posi+9);
            }
        }
        QByteArray contentLength=getHeader("Content-Length");
        if (!contentLength.isEmpty()) {
            m_expectedBodySize=contentLength.toInt();
        }
        if (m_expectedBodySize==0) {
#ifdef SUPERVERBOSE
            qDebug("HttpRequest: expect no body");
#endif
            m_status=complete;
        }
        else if (m_boundary.isEmpty() && m_expectedBodySize+m_currentSize>m_maxSize) {
            qWarning("HttpRequest: expected body is too large");
            m_status=abort;
        }
        else if (!m_boundary.isEmpty() && m_expectedBodySize>m_maxMultiPartSize) {
            qWarning("HttpRequest: expected multipart body is too large");
            m_status=abort;
        }
        else {
#ifdef SUPERVERBOSE
            qDebug("HttpRequest: expect %i bytes body",expectedBodySize);
#endif
            m_status=waitForBody;
        }
    }
}

void HttpRequest::readBody(QTcpSocket& socket) {
    Q_ASSERT(m_expectedBodySize!=0);
    if (m_boundary.isEmpty()) {
        // normal body, no multipart
#ifdef SUPERVERBOSE
        qDebug("HttpRequest: receive body");
#endif
        int toRead=m_expectedBodySize-m_bodyData.size();
        QByteArray newData=socket.read(toRead);
        m_currentSize+=newData.size();
        m_bodyData.append(newData);
        if (m_bodyData.size()>=m_expectedBodySize) {
            m_status=complete;
        }
    }
    else {
        // multipart body, store into temp file
#ifdef SUPERVERBOSE
        qDebug("HttpRequest: receiving multipart body");
#endif
        if (!m_tempFile.isOpen()) {
            m_tempFile.open();
        }
        // Transfer data in 64kb blocks
        int fileSize=m_tempFile.size();
        int toRead=m_expectedBodySize-fileSize;
        if (toRead>65536) {
            toRead=65536;
        }
        fileSize+=m_tempFile.write(socket.read(toRead));
        if (fileSize>=m_maxMultiPartSize) {
            qWarning("HttpRequest: received too many multipart bytes");
            m_status=abort;
        }
        else if (fileSize>=m_expectedBodySize) {
#ifdef SUPERVERBOSE
            qDebug("HttpRequest: received whole multipart body");
#endif
            m_tempFile.flush();
            if (m_tempFile.error()) {
                qCritical("HttpRequest: Error writing temp file for multipart body");
            }
            parseMultiPartFile();
            m_tempFile.close();
            m_status=complete;
        }
    }
}

void HttpRequest::decodeRequestParams() {
#ifdef SUPERVERBOSE
    qDebug("HttpRequest: extract and decode request parameters");
#endif
    // Get URL parameters
    QByteArray rawParameters;
    int questionMark=m_path.indexOf('?');
    if (questionMark>=0) {        
        rawParameters=m_path.mid(questionMark+1);
        m_path=m_path.left(questionMark);
    }
    // Get request body parameters
    QByteArray contentType=m_headers.value("Content-Type");
    if (!m_bodyData.isEmpty() && (contentType.isEmpty() || contentType.startsWith("application/x-www-form-urlencoded"))) {
        if (rawParameters.isEmpty()) {
            rawParameters.append('&');
            rawParameters.append(m_bodyData);
        }
        else {
            rawParameters=m_bodyData;
        }
    }
    // Split the parameters into pairs of value and name
    QList<QByteArray> list=rawParameters.split('&');
    foreach (QByteArray part, list) {
        int equalsChar=part.indexOf('=');
        if (equalsChar>=0) {
            QByteArray name=part.left(equalsChar).trimmed();
            QByteArray value=part.mid(equalsChar+1).trimmed();
            m_parameters.insert(urlDecode(name),urlDecode(value));
        }
        else if (!part.isEmpty()){
            // Name without value
            m_parameters.insert(urlDecode(part),"");
        }
    }
}

void HttpRequest::readFromSocket(QTcpSocket& socket) {
    Q_ASSERT(m_status!=complete);
    if (m_status==waitForRequest) {
        readRequest(socket);
    }
    else if (m_status==waitForHeader) {
        readHeader(socket);
    }
    else if (m_status==waitForBody) {
        readBody(socket);
    }
    if (m_currentSize>m_maxSize) {
        qWarning("HttpRequest: received too many bytes");
        m_status=abort;
    }
    if (m_status==complete) {
        // Extract and decode request parameters from url and body
        // decodeRequestParams();
        // Extract cookies from headers
        // extractCookies();
    }
    m_peerAddress = socket.peerAddress().toString();
    m_peerPort = socket.peerPort();
}


HttpRequest::RequestStatus HttpRequest::getStatus() const {
    return m_status;
}


QByteArray HttpRequest::getMethod() const {
    return m_method;
}


QByteArray HttpRequest::getPath() const {
    return urlDecode(m_path);
}

QString HttpRequest::getAbsoluteUrl() const
{
    return m_url;
}


QByteArray HttpRequest::getVersion() const {
    return m_version;
}


QByteArray HttpRequest::getHeader(const QByteArray& name) const {
    return m_headers.value(name);
}

QList<QByteArray> HttpRequest::getHeaders(const QByteArray& name) const {
    return m_headers.values(name);
}

QMultiMap<QByteArray,QByteArray> HttpRequest::getHeaderMap() const {
    return m_headers;
}

QByteArray HttpRequest::getParameter(const QByteArray& name) const {
    return m_parameters.value(name);
}

QList<QByteArray> HttpRequest::getParameters(const QByteArray& name) const {
    return m_parameters.values(name);
}

QMultiMap<QByteArray,QByteArray> HttpRequest::getParameterMap() const {
    return m_parameters;
}

QByteArray HttpRequest::getBody() const {
    return m_bodyData;
}

QByteArray HttpRequest::urlDecode(const QByteArray source) {
    QByteArray buffer(source);
    buffer.replace('+',' ');
    int percentChar=buffer.indexOf('%');
    while (percentChar>=0) {
        bool ok;
        char byte=buffer.mid(percentChar+1,2).toInt(&ok,16);
        if (ok) {
            buffer.replace(percentChar,3,(char*)&byte,1);
        }
        percentChar=buffer.indexOf('%',percentChar+1);
    }
    return buffer;
}


void HttpRequest::parseMultiPartFile() {
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

HttpRequest::~HttpRequest() {
    foreach(QByteArray key, m_uploadedFiles.keys()) {
        QTemporaryFile* file=m_uploadedFiles.value(key);
        file->close();
        delete file;
    }
}

QTemporaryFile* HttpRequest::getUploadedFile(const QByteArray fieldName) {
    return m_uploadedFiles.value(fieldName);
}

QByteArray HttpRequest::getCookie(const QByteArray& name) const {
    return m_cookies.value(name);
}

/** Get the map of cookies */
QMap<QByteArray,QByteArray>& HttpRequest::getCookieMap() {
    return m_cookies;
}

