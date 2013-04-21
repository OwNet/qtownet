#include "ownetclient.h"
#include "ownetcloudserver.h"

#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QMessageBox>

OwNetClient::OwNetClient(OwNetCloudServer *cloudServer, QObject *parent) :
    m_cloudServer(cloudServer), QObject(parent)
{    
    m_path = QCoreApplication::applicationDirPath().append("/OwNetClient");

#if defined(Q_OS_WIN)
    m_path = m_path.append(".exe");
#endif

    qCritical() << "Path to OwNetClient " << m_path;
}

void OwNetClient::start()
{
    m_process = new QProcess(this);
    m_output = "";

    connect(m_process, SIGNAL(finished(int)), this, SLOT(finished()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyRead()));

    m_process->setReadChannel(QProcess::StandardOutput);
    m_process->setProcessChannelMode(QProcess::MergedChannels);

    m_process->start(m_path, QStringList());
}

void OwNetClient::finished()
{
    if (m_process->exitStatus() != QProcess::NormalExit)
    {
        // first crash or crashed again in 10 secs
        if (m_lastError.isNull() || m_lastError < QDateTime::currentDateTime().addSecs(-10))
        {
            qDebug() << "Sending crash report";
            m_cloudServer->sendCrashReport(m_output);

            start();

            m_lastError = QDateTime::currentDateTime();
        }
        else
        {
            qDebug() << "Disabling OwNet client (crashed again in 10 secs)";
            QMessageBox::critical(NULL, "OwNet", "Oooops! OwNet has crashed. We have sent a report to our servers, so that we can figure out what's going on. If you know anything more, please contact us via <a href=\"http://ownet.fiit.stuba.sk\">http://ownet.fiit.stuba.sk</a>.");

            QCoreApplication::exit(1);
        }
    }
    else
        QCoreApplication::exit(0);
}

void OwNetClient::readyRead()
{
    m_output.append(m_process->readAll());
}

