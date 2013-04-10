/**
  @file
  @author Stefan Frings
*/

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QMap>
#include <QString>
#include <QTcpSocket>

/**
  This object represents a HTTP response, in particular the response headers.
  <p>
  Example code for proper response generation:
  <code><pre>
    response.setStatus(200,"OK"); // optional, because this is the default
    response.writeBody("Hello");
    response.writeBody("World!",true);
  </pre></code>
  <p>
  Example how to return an error:
  <code><pre>
    response.setStatus(500,"server error");
    response.write("The request cannot be processed because the servers is broken",true);
  </pre></code>
  <p>
  For performance reason, writing a single or few large packets is better than writing
  many small packets. In case of large responses (e.g. file downloads), a Content-Length
  header should be set before calling write(). Web Browsers use that information to display
  a progress bar.
*/

class HttpResponse : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HttpResponse)
public:

    /**
      Constructor.
      @param socket used to write the response
    */
    HttpResponse(QTcpSocket* m_socket, QObject *parent);

    /**
      Set a HTTP response header
      @param name name of the header
      @param value value of the header
    */
    void setHeader(QByteArray name, QByteArray value);

    /**
      Set a HTTP response header
      @param name name of the header
      @param value value of the header
    */
    void setHeader(QByteArray name, int value);

    /** Get the map of HTTP response headers */
    QMap<QByteArray,QByteArray>& getHeaders();

    /**
      Set status code and description. The default is 200,OK.
    */
    void setStatus(int m_statusCode, QByteArray description=QByteArray());

    /**
      Write body data to the socket.
      <p>
      The HTTP status line and headers are sent automatically before the first
      byte of the body gets sent.
      <p>
      If the response contains only a single chunk (indicated by lastPart=true),
      the response is transferred in traditional mode with a Content-Length
      header, which is automatically added if not already set before.
      <p>
      Otherwise, each part is transferred in chunked mode.
      @param data Data bytes of the body
      @param lastPart Indicator, if this is the last part of the response.
    */
    void write(QByteArray data, bool lastPart=false);

    /**
      Indicates wheter the body has been sent completely. Used by the connection
      handler to terminate the body automatically when necessary.
    */
    bool hasSentLastPart() const;

signals:
    void finished();

private:
    /** Write raw data to the socket. This method blocks until all bytes have been passed to the TCP buffer */
    bool writeToSocket(QByteArray data);

    /**
      Write the response HTTP status and headers to the socket.
      Calling this method is optional, because writeBody() calls
      it automatically when required.
    */
    void writeHeaders();

    bool isChunked() { return m_isChunked; }

    /** Request headers */
    QMap<QByteArray,QByteArray> m_headers;
    bool m_isChunked;

    /** Socket for writing output */
    QTcpSocket* m_socket;

    /** HTTP status code*/
    int m_statusCode;

    /** HTTP status code description */
    QByteArray m_statusText;

    /** Indicator whether headers have been sent */
    bool m_sentHeaders;

    /** Indicator whether the body has been sent completely */
    bool m_sentLastPart;
};

#endif // HTTPRESPONSE_H
