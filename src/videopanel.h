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

#ifndef VIDEOPANEL_H
#define VIDEOPANEL_H

#include <QWidget>
#include <QStatusBar>
#include "qmpwidget.h"

namespace Ui {
class VideoPanel;
}

class VideoPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit VideoPanel(QWidget *parent = 0);
    ~VideoPanel();
    
    void play (const QString & url);
    void setStatusBar (QStatusBar *bar);

private slots:
    void on_stopButton_clicked();
    void on_muteButton_clicked();

    void slotMplayerStatusChanged (int);

    void on_playPauseButton_clicked();

private:
    Ui::VideoPanel *ui;

    int m_storedVolume;
};

#endif // VIDEOPANEL_H
