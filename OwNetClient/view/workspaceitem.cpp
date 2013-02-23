#include "workspaceitem.h"

#include "workspacehelper.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>

WorkspaceItem::WorkspaceItem(QString id, QString name, QTreeWidget *treeWidget, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_name(name),
    m_active(false),
    m_treeItem(NULL),
    m_treeWidget(treeWidget),
    m_joined(false),
    m_openButton(NULL)
{
}

/**
 * @brief True if there are peers in the workspace available and it should be visible.
 * @param active Are there any peers in the workspace currently available
 */
void WorkspaceItem::setActive(bool active)
{
    m_active = m_joined || active;

    if (m_active) {
        if (m_treeItem) {
            m_treeItem->setHidden(false);
        } else {
            m_openButton = new QPushButton(tr("Open"), m_treeWidget);
            m_openButton->setMaximumWidth(100);
            if (WorkspaceHelper::currentWorkspaceId() == m_id)
                m_openButton->setEnabled(false);
            else
                connect(m_openButton, SIGNAL(clicked()), this, SLOT(openButtonClicked()));

            m_treeItem = new QTreeWidgetItem(m_treeWidget);
            m_treeItem->setText(0, m_name);

            m_treeWidget->addTopLevelItem(m_treeItem);
            m_treeWidget->setItemWidget(m_treeItem, 1, m_openButton);
        }
    } else if (m_treeItem) {
        m_treeItem->setHidden(true);
    }
}

/**
 * @brief Change the name of the workspace
 * @param name Name of the workspace
 */
void WorkspaceItem::setName(const QString &name)
{
    m_name = name;
    if (m_treeItem)
        m_treeItem->setText(0, name);
}

/**
 * @brief Should restart the app and load the workspace database
 */
void WorkspaceItem::openButtonClicked()
{
    WorkspaceHelper::loadWorkspace(m_id, m_name);
}
