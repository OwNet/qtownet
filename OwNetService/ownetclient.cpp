#include "ownetclient.h"

#include <QDebug>
#include <QProcess>
#include <QCoreApplication>

OwNetClient::OwNetClient(QObject *parent) :
    QObject(parent)
{
    m_path = QCoreApplication::applicationDirPath().append("/../OwNetClient/OwNetClient");
}

void OwNetClient::start()
{
    m_process = new QProcess(this);
    m_output = "";

    connect(m_process, SIGNAL(finished(int)), this, SLOT(finished()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyRead()));

    m_process->setReadChannel(QProcess::StandardOutput);
    m_process->setProcessChannelMode(QProcess::MergedChannels);

    m_process->start(m_path);

}

void OwNetClient::finished()
{
    if (m_process->exitStatus() != QProcess::NormalExit)
    {
        // first crash or crashed again in 10 secs
        if (m_lastError.isNull() || m_lastError < QDateTime::currentDateTime().addSecs(-10))
        {
            start();

            qDebug() << "Sending crash report";
            // send m_output

            m_lastError = QDateTime::currentDateTime();
        }
        else
        {
            qDebug() << "Disabling OwNet client (crashed again in 10 secs)";
            // notify user

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
