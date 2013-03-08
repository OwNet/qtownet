#include "workspaceitem.h"

#include "workspacehelper.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>

WorkspaceItem::WorkspaceItem(QString id, QString name, QTreeWidget *treeWidget, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_name(name),
    m_active(false),
    m_treeItem(NULL),
    m_treeWidget(treeWidget),
    m_joined(false),
    m_openButton(NULL),
    m_removeButton(NULL)
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
            m_treeItem = new QTreeWidgetItem(m_treeWidget);
            QWidget *widget = new QWidget(m_treeWidget);

            m_openButton = new QPushButton(tr("Open"), widget);
            if (WorkspaceHelper::currentWorkspaceId() == m_id)
                m_openButton->setEnabled(false);
            else
                connect(m_openButton, SIGNAL(clicked()), this, SLOT(openButtonClicked()));

            m_removeButton = new QPushButton(tr("Remove"), widget);
            if (WorkspaceHelper::currentWorkspaceId() == m_id)
                m_removeButton->setEnabled(false);
            else
                connect(m_removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));

            QHBoxLayout *layout = new QHBoxLayout(widget);
            layout->addWidget(m_openButton);
            layout->addWidget(m_removeButton);
            widget->setMaximumWidth(170);

            m_treeItem->setText(0, m_name);

            m_treeWidget->addTopLevelItem(m_treeItem);
            m_treeWidget->setItemWidget(m_treeItem, 1, widget);
        }
    } else if (m_treeItem) {
        delete m_treeItem;
        m_treeItem = NULL;
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
    if (QMessageBox::Yes == QMessageBox::question(m_treeWidget->parentWidget(),
                                                  tr("OwNet - Switch Workspace"),
                                                  tr("Are you sure you want to switch to the workspace %1? This will restart the application.").arg(m_name),
                                                  QMessageBox::Yes, QMessageBox::No))
        WorkspaceHelper::loadWorkspace(m_id, m_name);
}

/**
 * @brief Remove the current workspace from settings and also the database file.
 * Ask user for confirmation.
 */
void WorkspaceItem::removeButtonClicked()
{
    if (QMessageBox::Yes == QMessageBox::question(m_treeWidget->parentWidget(),
                                                  tr("OwNet - Remove Workspace"),
                                                  tr("Are you sure you want to remove the workspace %1?").arg(m_name),
                                                  QMessageBox::Yes, QMessageBox::No))
        if (WorkspaceHelper::removeWorkspace(m_id)) {
            m_joined = false;
            setActive(false);
        }
}
