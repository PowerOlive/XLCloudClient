/*
 *  CloudClient - A Qt cloud client for lixian.vip.xunlei.com
 *  Copyright (C) 2012 by Aaron Lewis <the.warl0ck.1989@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef THUNDERPANEL_H
#define THUNDERPANEL_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSettings>
#include <QMenu>
#include <QHash>
#include <QAction>
#include <QSortFilterProxyModel>
#include <QCursor>
#include <QApplication>
#include <QKeyEvent>
#include <QClipboard>
#include <QStandardItem>
#include <QDebug>

#include "CloudObject.h"
#include "util.h"
#include "config.h"

namespace Ui {
class ThunderPanel;
}

class ThunderPanel : public QWidget
{
    Q_OBJECT
    
public:
    enum RequestType
    {
        Download,
        Preview,
        Image
    };

    enum IndirectRequestType
    {
        RemoveTasks,
        AddTask
    };

    enum DisplayFilterMode
    {
        FilterWithRegex         = 0,
        FilterWithWildcard      = 1,
        FilterWithFixedString   = 2
    };

    explicit ThunderPanel(QWidget *parent = 0);
    ~ThunderPanel();
    QString my_downloaderScriptTemplate;

    void resizeColumnToContents (int column);

    void setQuickViewMode (bool ok);

    void setCloudTasks (const QList<Thunder::Task> & tasks);
    QPair<QString, int> getTasksAsScript();

    QPair<QStringList, QList<int> > getSelectedTaskIDsAndRows ();
    void removeSelectedTasks (const QList<int> & ids);
    Thunder::RemoteTask getFirstSelectedTask ();

    void keyEvent (QKeyEvent *e);

public slots:
    void setBTSubTask (const Thunder::BitorrentTask & task);
    void loadSettings ();

signals:
    void doThisLink (const Thunder::RemoteTask & task,
                     ThunderPanel::RequestType type,
                     bool autoOpen);
    void doIndirectRequest (ThunderPanel::IndirectRequestType type);

private:
    Ui::ThunderPanel *ui;
    bool my_quickViewMode;
    QString my_gdriveid;

    /*!
     * \brief Mapping between task id and sub tasks of a BT task
     */
    QHash<QString, QStandardItem*> my_BTSubTaskMapping;

    QSortFilterProxyModel *my_filterModel;
    QStandardItemModel *my_model;
    DisplayFilterMode  displayFilterMode;

    /*!
     * \brief getUserDataByOffset
     * \param 0 when retrieving DisplayRole
     * \param row
     * \return
     */
    QString getUserDataByOffset (unsigned long long offset, int row = -1);

    QMenu *my_contextMenu;

private slots:
    void slotShowContextMenu (const QPoint & pt);
    void slotCookiesReady (const QString & gdriveid);

    void slotPreviewThisTask ();
    void slotDownloadThisTask ();

    void slotRemoveTheseTasks ();

    void slotCopyDownloadAddress ();
    void slotCopySourceAddress ();
    void slotCopyTaskName ();
    void slotCopyAria2cScript ();

    void slotResizeFirstColumnOfTreeView();
    void slotResizeAllColumnsOfTreeView();

    void on_treeView_doubleClicked(const QModelIndex &index);
    void on_filter_textChanged(const QString &arg1);
    void on_toolButton_clicked();
};

#endif // THUNDERPANEL_H
