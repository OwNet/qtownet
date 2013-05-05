#ifndef REQUESTREADER_H
#define REQUESTREADER_H

#include <QObject>
#include <QIODevice>
#include <QTemporaryFile>
#include <QMultiMap>

class SocketHandler;
class QTcpSocket;

class RequestReader : public QObject
{
    Q_OBJECT
public:
    enum RequestStatus {
        WaitForRequest,
        WaitForHeader,
        WaitForBody,
        Complete,
        Abort,
        Unchanged
    };

    explicit RequestReader(SocketHandler *handler, QObject *parent = 0);

    RequestStatus readFromSocket(QTcpSocket *socket);

    QString url() const { return m_url; }
    QString peerAddress() const { return m_peerAddress; }
    int peerPort() const { return m_peerPort; }
    RequestStatus status() const { return m_status; }
    QMultiMap<QByteArray,QByteArray> headerMap() { return m_headers; }
    QByteArray method() const { return m_method; }

    QByteArray requestBody() const { return m_bodyData; }
    QByteArray wholeRequestBody() const { return m_wholeRequestBody; }
    QByteArray getHeader(const QByteArray& name) const;

    QTemporaryFile &temporaryFile() { return m_tempFile; }

private:
    void readRequest(QIODevice &stream);
    void readHeader(QIODevice &stream);
    void readBody(QIODevice &stream);
    void parseMultiPartFile();

    void setStatus(RequestStatus status);

    SocketHandler *m_handler;

    RequestStatus m_status; ///< Status of reading
    QString m_url; ///< Full request url
    QByteArray m_method; ///< Request method
    QByteArray m_version; ///< Request protocol version
    QByteArray m_boundary; ///< Boundary of multipart/form-data body. Empty if there is no such header
    QByteArray m_wholeRequestBody; ///< The whole request body with headers
    QByteArray m_bodyData; ///< Storage for raw body data
    QMultiMap<QByteArray,QByteArray> m_headers; ///< Request headers
    QMultiMap<QByteArray,QByteArray> m_parameters; ///< Parameters of the request
    QMap<QByteArray,QTemporaryFile*> m_uploadedFiles; ///< Uploaded files of the request, key is the field name
    QTemporaryFile m_tempFile; ///< Temp file, that is used to store the multipart/form-data body
    QByteArray m_currentHeader; ///< Name of the current header, or empty if no header is being processed

    int m_maxSize; ///< Maximum size of requests in bytes
    int m_maxMultiPartSize; ///< Maximum allowed size of multipart forms in bytes
    int m_currentSize; ///< Current size
    int m_expectedBodySize; ///< Expected size of body

    QString m_peerAddress; ///< IP address of the requester
    quint16 m_peerPort; ///< Port of the requester
};

#endif // REQUESTREADER_H
