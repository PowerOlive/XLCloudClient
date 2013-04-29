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

#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QMovie>
#include <QAction>
#include <QMenu>
#include <QWebElement>
#include <QWebPage>
#include <QSettings>
#include <QUrl>
#include <QDebug>

namespace Ui {
class Browser;
}

class Browser : public QWidget
{
    Q_OBJECT
    
public:
    explicit Browser(QWidget *parent = 0);
    ~Browser();

    QRegExp searchRegex;
    
private slots:
    void on_reload_clicked();

    void on_fwd_clicked();
    void on_stop_clicked();

    void on_webView_linkClicked(const QUrl &arg1);
    void on_webView_loadStarted();

    void on_webView_loadFinished(bool arg1);

    void set_yyets ();
    void set_verycd ();

    void returnPressed ();

    void on_addToCloudButton_clicked();

signals:
    void browserLinksReady (const QString & url);

private:
    Ui::Browser *ui;

   enum SearchEngine
    {
        VeryCD,
        YYeTS
    };

   SearchEngine m_engine;
   QString m_discoveredUrls;
};

#endif // BROWSER_H
