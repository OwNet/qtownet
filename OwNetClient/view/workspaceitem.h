#ifndef WORKSPACEITEM_H
#define WORKSPACEITEM_H

#include <QObject>

class QTreeWidgetItem;
class QTreeWidget;
class QPushButton;

class WorkspaceItem : public QObject
{
    Q_OBJECT
public:
    explicit WorkspaceItem(QString id, QString name, QTreeWidget *treeWidget, QObject *parent = 0);

    void setActive(bool active);
    void setJoined(bool joined) { m_joined = joined; setActive(m_joined); }
    void setName(const QString &name);

    QString name() const { return m_name; }

private slots:
    void openButtonClicked();
    void removeButtonClicked();
    
private:
    QString m_id;
    QString m_name;
    bool m_active;
    bool m_joined;

    QTreeWidgetItem *m_treeItem;
    QTreeWidget *m_treeWidget;
    QPushButton *m_openButton;
    QPushButton *m_removeButton;
};

#endif // WORKSPACEITEM_H
