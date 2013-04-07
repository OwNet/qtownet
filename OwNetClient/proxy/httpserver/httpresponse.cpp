/**
  @file
  @author Stefan Frings
*/

#include "httpresponse.h"

HttpResponse::HttpResponse(QTcpSocket* socket, QObject *parent) :
    QObject(parent)
{
    this->m_socket=socket;
    m_statusCode=200;
    m_statusText="OK";
    m_sentHeaders=false;
    m_sentLastPart=false;
}

void HttpResponse::setHeader(QByteArray name, QByteArray value) {
    Q_ASSERT(m_sentHeaders==false);
    m_headers.insert(name,value);
}

void HttpResponse::setHeader(QByteArray name, int value) {
    Q_ASSERT(m_sentHeaders==false);
    m_headers.insert(name,QByteArray::number(value));
}

QMap<QByteArray,QByteArray>& HttpResponse::getHeaders() {
    return m_headers;
}

void HttpResponse::setStatus(int statusCode, QByteArray description) {
    this->m_statusCode=statusCode;
    m_statusText=description;
}

void HttpResponse::writeHeaders() {
    Q_ASSERT(m_sentHeaders==false);
    QByteArray buffer;
    buffer.append("HTTP/1.1 ");
    buffer.append(QByteArray::number(m_statusCode));
    buffer.append(' ');
    buffer.append(m_statusText);
    buffer.append("\r\n");
    foreach(QByteArray name, m_headers.keys()) {
        buffer.append(name);
        buffer.append(": ");
        buffer.append(m_headers.value(name));
        buffer.append("\r\n");
    }
    buffer.append("\r\n");
    writeToSocket(buffer);
    m_sentHeaders=true;
}

bool HttpResponse::writeToSocket(QByteArray data) {
    if (m_socket->state() == QTcpSocket::UnconnectedState)
        return true;

    int remaining=data.size();
    char* ptr=data.data();
    while (m_socket->isOpen() && remaining>0) {
        // Wait until the previous buffer content is written out, otherwise it could become very large
        m_socket->waitForBytesWritten(-1);
        int written=m_socket->write(data);
        if (written==-1) {
          return false;
        }
        ptr+=written;
        remaining-=written;
    }
    return true;
}

void HttpResponse::write(QByteArray data, bool lastPart) {
    Q_ASSERT(m_sentLastPart==false);
    if (m_sentHeaders == false && (!lastPart || !data.isEmpty())) {
        QByteArray connectionMode=m_headers.value("Connection");
        if (!m_headers.contains("Content-Length") && !m_headers.contains("Transfer-Encoding") && connectionMode!="close" && connectionMode!="Close") {
            if (!lastPart) {
                m_headers.insert("Transfer-Encoding","chunked");
            }
            else {
                m_headers.insert("Content-Length",QByteArray::number(data.size()));
            }
        }
        writeHeaders();
    }
    bool chunked=m_headers.value("Transfer-Encoding")=="chunked" || m_headers.value("Transfer-Encoding")=="Chunked";
    if (chunked) {
        if (data.size()>0) {
            QByteArray buffer=QByteArray::number(data.size(),16);
            buffer.append("\r\n");
            writeToSocket(buffer);
            writeToSocket(data);
            writeToSocket("\r\n");
        }
    }
    else {
        writeToSocket(data);
    }
    if (lastPart) {
        if (chunked) {
            writeToSocket("0\r\n\r\n");
        }
        else if (!m_headers.contains("Content-Length")) {
            m_socket->disconnectFromHost();
        }
        m_sentLastPart=true;
        emit finished();
    }
}


bool HttpResponse::hasSentLastPart() const {
    return m_sentLastPart;
}
