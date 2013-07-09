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

#include "preferencesdlg.h"
#include "ui_preferencesdlg.h"

PreferencesDlg::PreferencesDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDlg)
{
    ui->setupUi(this);

    QSettings settings;

    settings.beginGroup("General");
    ui->user->setText(settings.value("User").toString());
    ui->credential->setText(settings.value("Credential").toString());
    ui->quickPreviewMode->setChecked(settings.value("QuickViewMode").toBool());
    ui->downloadScriptTemplate->setText(
                settings.value("DownloaderScriptTemplate",
                               TC_DEFAULT_DOWNLOAD_TEMPLATE).toString());

    int displayFilterMode = settings.value("DisplayFilterMode", 0).toInt();
    if (ui->taskDisplayFilterMode->count() > displayFilterMode)
        ui->taskDisplayFilterMode->setCurrentIndex(displayFilterMode);

    // dirty fix!
    tmp_lastCred = ui->credential->text();
    settings.endGroup();

    settings.beginGroup("Proxy");
    ui->proxyEnabled->setChecked(settings.value("ProxyEnabled").toBool());
    ui->proxyuser->setText(settings.value("User").toString());
    ui->proxycred->setText(settings.value("Cred").toString());
    ui->proxyserver->setText(settings.value("Server").toString());
    settings.endGroup();

    settings.beginGroup("Transf0r");
    ui->storageLocation->setText(settings.value("StorageLocation", Util::getHomeLocation()).toString());
    ui->useVoiceNotification->setChecked(settings.value("UseVoiceNotification", false).toBool());
    settings.endGroup();

    int cIdx = settings.value("Index").toInt();
    if (cIdx < ui->tabWidget->count())
        ui->tabWidget->setCurrentIndex(cIdx);

    settings.beginGroup("Video");
    ui->mplayerBuffer->setText(settings.value("mplayerBufferSize").toString());
    ui->mplayerPath->setText(settings.value("mplayerPath").toString());
    ui->mplayerPath->setParam(true);
    settings.endGroup();

    settings.beginGroup("Browser");
    ui->browserLinksRegularExpression->setText(settings.value("RegularExpression").toString());
    ui->verycdAccount->setText(settings.value("verycdAccount").toString());
    ui->verycdCred->setText(settings.value("verycdCred").toString());
    settings.endGroup();
}

PreferencesDlg::~PreferencesDlg()
{
    delete ui;
}

void PreferencesDlg::on_buttonBox_accepted()
{
    QSettings settings;

    settings.beginGroup("General");
    settings.setValue("Index", ui->tabWidget->currentIndex());
    settings.setValue("User", ui->user->text());
    settings.setValue("QuickViewMode", ui->quickPreviewMode->isChecked());
    settings.setValue("DisplayFilterMode", ui->taskDisplayFilterMode->currentIndex());
    settings.setValue("DownloaderScriptTemplate", ui->downloadScriptTemplate->text());

    if (ui->credential->text() != tmp_lastCred)
        settings.setValue("Credential", Util::getMD5Hex(ui->credential->text()));
    settings.endGroup();

    settings.beginGroup("Proxy");
    settings.setValue("ProxyEnabled", ui->proxyEnabled->isChecked());
    settings.setValue("Server", ui->proxyserver->text());
    settings.setValue("User", ui->proxyuser->text());
    settings.setValue("Cred", ui->proxycred->text());
    settings.endGroup();

    settings.beginGroup("Transf0r");
    settings.setValue("UseVoiceNotification", ui->useVoiceNotification->isChecked());
    settings.setValue("StorageLocation", ui->storageLocation->text());
    settings.endGroup();

    settings.beginGroup("Video");
    settings.setValue("mplayerPath", ui->mplayerPath->text());
    settings.setValue("mplayerBufferSize", ui->mplayerBuffer->text());
    settings.endGroup();

    settings.beginGroup("Browser");
    settings.setValue("RegularExpression", ui->browserLinksRegularExpression->text().trimmed().isEmpty()
                      ? "ed2k://"
                      : ui->browserLinksRegularExpression->text());
    settings.setValue("verycdAccount", ui->verycdAccount->text());
    settings.setValue("verycdCred", ui->verycdCred->text());
    settings.endGroup();
}
