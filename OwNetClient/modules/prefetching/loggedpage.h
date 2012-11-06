#ifndef LOGGEDPAGE_H
#define LOGGEDPAGE_H

#include <QObject>
#include <QString>

class LoggedPage : public QObject
{
    Q_OBJECT
public:
    const int id();

    LoggedPage(int id, QString url);
    void addLinks(QString url) { m_links.push_back(url); }
    const QList<QString> &links() { return m_links; }
private:
    int m_id;
    QString m_url;
    QList<QString> m_links;
};

#endif // LOGGEDPAGE_H
