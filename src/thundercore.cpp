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

#include "thundercore.h"

ThunderCore::ThunderCore(QObject *parent) :
    QObject(parent),
    tmp_cookieIsStored (false),
    tc_nam (new QNetworkAccessManager (this))
{
    connect (tc_nam, SIGNAL(finished(QNetworkReply*)),
             SLOT(slotFinished(QNetworkReply*)));
}

QList<Thunder::Task> ThunderCore::getCloudTasks()
{
    return tc_cloudTasks;
}

QList<Thunder::Task> ThunderCore::getGarbagedTasks()
{
    return tc_garbagedTasks;
}

QList<Thunder::BatchTask> ThunderCore::getUploadedBatchTasks()
{
    return tmp_batchTasks;
}

Thunder::RemoteTask ThunderCore::getSingleRemoteTask()
{
    return tmp_singleTask;
}

void ThunderCore::addBatchTaskPre(const QString &urls)
{
    post (QUrl("http://dynamic.cloud.vip.xunlei.com/interface/batch_task_check"),
          "random=123456&url=" + urls.toUtf8().toPercentEncoding());
}

void ThunderCore::addBatchTaskPost(const QStringList &urls)
{
    QByteArray postData = "cid%5B%5D=&class_id=0&"
            "batch_old_taskid=0&batch_old_database=0";

    foreach (const QString & url, urls)
    {
        postData.append("&url%5B%5D=").append(url.toUtf8().toPercentEncoding());
    }

    post (QUrl("http://dynamic.cloud.vip.xunlei.com/interface/batch_task_commit?callback=a"),
          postData);
}

void ThunderCore::delayCloudTask(const QStringList &ids)
{
    QString id_list;
    foreach (const QString & id, ids)
    {
        id_list.append(id).append("_1,");
    }

    get (QUrl ("http://dynamic.cloud.vip.xunlei.com/interface/task_delay"
               "?taskids=" + id_list +
               "&interfrom=task&noCacheIE=1362310408959"));
}

void ThunderCore::cleanupHistory()
{
    QUrl url ("http://dynamic.cloud.vip.xunlei.com/interface/history_clear"
              "?tcache=1328430359476&flag=6");

    url.addQueryItem("uid", tc_session.value("userid"));
    get (url);
}

void ThunderCore::commitBitorrentTask(const QList<Thunder::BTSubTask> &tasks)
{
    QString ids, sizes;
    foreach (const Thunder::BTSubTask & task, tasks)
    {
        ids.append(task.id).append("_");
        sizes.append(task.size).append("_");
    }

    post (QUrl("http://dynamic.cloud.vip.xunlei.com/"
               "interface/bt_task_commit?callback=a&t=" +
               QDateTime::currentDateTime().toString().toAscii().toPercentEncoding()),
          "goldbean=0&class_id=0&o_taskid=0&o_page=task&silverbean=0"
          "&findex=" + ids.toAscii() +
          "&uid=" + tc_session.value("userid").toAscii() +
          "&btname=" + tmp_btTask.ftitle.toUtf8().toPercentEncoding() +
          "&tsize=" + QByteArray::number(tmp_btTask.btsize) +
          "&size=" + sizes.toAscii() +
          "&cid=" + tmp_btTask.infoid.toAscii());
}

void ThunderCore::addCloudTaskPost(const Thunder::RemoteTask &task)
{
    QUrl url ("http://dynamic.cloud.vip.xunlei.com/interface/"
              "task_commit?callback=ret_task&cid=&gcid=&goldbean=0&"
              "silverbean=0&type=2&o_page=task&o_taskid=0&ref_url=");

    url.addQueryItem("size", task.size);
    url.addQueryItem("uid", tc_session["userid"]);
    url.addQueryItem("t", task.name);
    url.addQueryItem("url", task.url);

    get (url);
}

void ThunderCore::loginWithCapcha(const QByteArray &capcha)
{
    error (tr("Login .. cookie: %1").arg(QString::fromAscii(capcha)), Info);

    post (QUrl("http://login.xunlei.com/sec2login/"),
          "login_hour=720&login_enable=0&u=" + tc_userName.toAscii() +
          "&verifycode=" + capcha +
          "&p=" + Util::getEncryptedPassword(
              tc_passwd, QString::fromAscii(capcha), true).toAscii());
}

void ThunderCore::getContentsOfBTFolder(const Thunder::Task &bt_task, const int & page)
{
    get ("http://dynamic.cloud.vip.xunlei.com/interface/fill_bt_list"
         "?callback=fill_bt_list&g_net=1&noCacheIE=1328405858893&"
         "&p=" + QString::number(page) +
         "&infoid=" + bt_task.cid +
         "&tid=" + bt_task.id +
         "&uid=" + tc_session.value("userid"));
}

void ThunderCore::setCapcha(const QString &code)
{
    loginWithCapcha(code.toAscii());
}

void ThunderCore::reloadCloudTasks()
{
    /// Never play with callback parameter!
    get (QUrl("http://dynamic.cloud.vip.xunlei.com/interface/showtask_unfresh?"
              "callback=tc&t="
              "&type_id=4&page=1&tasknum=500"
              "&p=1&interfrom=task"));
}

void ThunderCore::addCloudTaskPre(const QString &url)
{
    QUrl link ("http://dynamic.cloud.vip.xunlei.com/interface/"
               "task_check?callback=queryCid"
               "&random=13271369889801529719.0135479392&tcache=1327136998160");
    link.addQueryItem("url", url);

    get (link);
}

void ThunderCore::slotFinished(QNetworkReply *reply)
{
    const QUrl & url = reply->url();
    const QString & urlStr = url.toString();
    const QByteArray & data = reply->readAll();
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (httpStatus < 200 || httpStatus > 400)
    {
        error (tr("Error reading %1, got %2")
               .arg(urlStr)
               .arg(httpStatus), Notice);
        return;
    }
    /////

    if (urlStr.startsWith("http://login.xunlei.com/check"))
    {
        QByteArray capcha;
        foreach (const QNetworkCookie & cookie, tc_nam->cookieJar()->cookiesForUrl(url))
        {
            if (cookie.name() == "check_result") { capcha = cookie.value(); break; }
        }

        /* Get rid of heading 0: */
        capcha = capcha.remove(0, 2);

        if (capcha.isEmpty())
        {
            error (tr("Capcha required, retrieving .."), Notice);
            tc_loginStatus = Failed; emit StatusChanged (LoginChanged);

            /* TODO: */
            get (QString("http://verify.xunlei.com/image?cachetime=1359365355018"));

            return;
        }

        loginWithCapcha(capcha);

        return;
    }

    if (urlStr.startsWith("http://verify.xunlei.com/image"))
    {
        tc_capcha = data;
        error (tr("Capcha received, size %1").arg(tc_capcha.size()), Notice);

        emit StatusChanged(CapchaReady);

        return;
    }

    if (urlStr.startsWith("http://login.xunlei.com/sec2login/"))
    {
        foreach (const QNetworkCookie & cookie, tc_nam->cookieJar()->cookiesForUrl(url))
        {
            if (cookie.name() == "usernick")
            {
                error (tr("User nick: %1").arg(QString::fromAscii(cookie.value())), Info);
            }

            tc_session.insert(cookie.name(), cookie.value());
        }

        if (! tc_session.contains("jumpkey"))
        {
            error (tr("Logon failure"), Warning);
            tc_loginStatus = Failed; emit StatusChanged (LoginChanged);

            return;
        }

        get (QUrl("http://dynamic.cloud.vip.xunlei.com/login?"
                  "cachetime=1327129660280&cachetime=1327129660555&from=0"));

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/login"))
    {
        get (QUrl("http://dynamic.cloud.vip.xunlei.com/user_task?st=0&userid=" +
                  tc_session.value("userid")));

        tc_loginStatus = NoError; emit StatusChanged (LoginChanged);

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/showtask_unfresh"))
    {
        error (tr("Parsing task data .."), Info);
        parseCloudPage(data);

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/user_task"))
    {
        /*
        * It's best to handle the page with QtScript!
        */
        error (tr("Retrieved user page, fetching task data .."), Info);
        reloadCloudTasks();

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/task_delete"))
    {
        error (tr("Task removed from cloud!"), Info);
        return;

        /*!
         * \brief Reload user page!
         */
        reloadCloudTasks();

        error (tr("Task removed, reloading page .."), Info);
        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/task_check"))
    {
        QStringList fields = Util::parseFunctionFields(data);

        if (fields.size() < 10)
        {
            error (tr("Protocol changed or parser failure (incorrect column count), submit this line: %1")
                   .arg(QString::fromUtf8(data)), Warning);
            return;
        }

        tmp_singleTask.name = fields.at(4);
        tmp_singleTask.size = Util::toReadableSize(fields.at(2).toULongLong());

        emit RemoteTaskChanged(SingleTaskReady);

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/task_commit"))
    {
        error(tr("Task added, reloading page .."), Notice);

        reloadCloudTasks();

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/"
                          "interface/torrent_upload"))
    {
        // todo: bt task editing?
        QByteArray json = data;

        // remove btResult =
        json.remove (0, 51);

        // chop </script>
        json.chop (10);

        QJson::Parser parser;
        bool ok = false;
        QVariant result = parser.parse(json, &ok);

        if (! ok)
        {
            error (tr("JSON parse failure, protocol changed or invalid data."),
                   Warning);
            qDebug() << json;

            return;
        }

        tmp_btTask.ftitle = result.toMap().value("ftitle").toString();
        tmp_btTask.infoid = result.toMap().value("infoid").toString();
        tmp_btTask.btsize = result.toMap().value("btsize").toULongLong();

        foreach (const QVariant & item, result.toMap().value("filelist").toList())
        {
            QVariantMap map = item.toMap();

            Thunder::BTSubTask task;
            task.name = map.value("subtitle").toString();
            task.format_size = map.value("subformatsize").toString();
            task.size = map.value("subsize").toString();
            task.id = map.value("id").toString();

            tmp_btTask.subtasks.append(task);
        }

        emit RemoteTaskChanged(BitorrentTaskReady);

        return;

    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/"
                          "interface/bt_task_commit"))
    {
        error(tr("Bitorrent commited, reloading tasks .."), Notice);

        reloadCloudTasks();

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/history_clear"))
    {
        error(tr("History emptied"), Notice);

        // ERROR checking?

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/fill_bt_list"))
    {
        error(tr("BT task page retrieved, parsing (page %1)..").arg(url.queryItemValue("p")), Notice);

        QByteArray json = data;

        // remove a(
        json.remove (0, 13);

        // chop )
        json.chop (1);

        QJson::Parser parser;
        bool ok = false;
        QVariant result = parser.parse(json, &ok);

        if (! ok)
        {
            error (tr("BT task page: JSON parse failure, "
                      "protocol changed or invalid data."),
                   Warning);
            qDebug() << json;

            return;
        }

        result = result.toMap().value("Result");

        Thunder::BitorrentTask bt_task;
        const QVariantMap & resultMap = result.toMap();

        int now_page = resultMap.value ("now_page").toInt();
        int btnum = resultMap.value("btnum").toInt();
        int btpernum = resultMap.value("btpernum").toInt();

        if (btpernum != 0)
        {
            if (now_page < btnum / btpernum)
            {
                /// dirty hack: repicate current request!
                QNetworkRequest request = reply->request();
                QUrl url = request.url();
                url.removeQueryItem("p");
                url.addQueryItem("p", QString::number(++ now_page));
                request.setUrl(url);

                tc_nam->get(request);
            }
        }

        if (resultMap.size() == 0)
        {
            error(tr("BT task not finished, skipping sub tasks."), Notice);
            return;
        }

        bt_task.taskid = resultMap.value("Tid").toString();

        QVariant mainMap = resultMap.value("Record");

        foreach (const QVariant & record, mainMap.toList())
        {
            const QVariantMap & map = record.toMap();
            Thunder::BTSubTask subtask;

            subtask.id = map.value("id").toString();
            subtask.size = Util::toReadableSize(map.value("filesize").toULongLong());
            subtask.link = map.value("downurl").toString().replace("\\/", "/");
            subtask.name = map.value("title").toString();

            bt_task.subtasks.append(subtask);
        }

        emit BTSubTaskReady (bt_task);
        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/batch_task_check"))
    {
        QByteArray json = data;

        // remove <script>document.domain='xunlei.com';parent.begin_task_batch_resp(
        json.remove(0, json.indexOf("(") + 1);

        // chop ,'123456');</script>"
        json.chop(json.length() - json.lastIndexOf("]") - 1);

        QJson::Parser parser;
        bool ok = false;

        QVariant result = parser.parse(json, &ok);
        if (! ok)
        {
            error (tr("Batch task parser: JSON parse failure, "
                      "protocol changed or invalid data."),
                   Warning);
            qDebug() << json;

            return;
        }

        tmp_batchTasks.clear();
        foreach (const QVariant & item, result.toList())
        {
            const QVariantMap & itemData = item.toMap();
            Thunder::BatchTask batch_task;

            batch_task.url = itemData.value("url").toString();
            batch_task.name = itemData.value("name").toString();
            batch_task.size = itemData.value("filesize").toULongLong();
            batch_task.formatsize = itemData.value("formatsize").toString();

            tmp_batchTasks.push_back(batch_task);
        }

        emit RemoteTaskChanged(ThunderCore::BatchTaskReady);

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/batch_task_commit"))
    {
        reloadCloudTasks();
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/task_delay"))
    {
        return;
    }

    qDebug() << "Unhandled reply:" << "\n----";
    qDebug() << "URL: " << urlStr;
    qDebug() << "Cookie: ";
    foreach (const QNetworkCookie & cookie, tc_nam->cookieJar()->cookiesForUrl(url))
    {
        qDebug() << cookie.name() << cookie.value();
    }
    qDebug() << "Data: " << data;
}

QByteArray ThunderCore::getCapchaCode()
{
    return tc_capcha;
}

Thunder::BitorrentTask ThunderCore::getUploadedBTTasks()
{
    return tmp_btTask;
}

QString ThunderCore::getCookieFilePath()
{
    return Util::getHomeLocation() + "/.tdcookie";
}

void ThunderCore::parseCloudPage(const QByteArray &body)
{
    /// CACHE TASK IDS for automatic task renewal
    QStringList local_taskids;

    QVariantMap json_map, json_info, user_info;
    QJson::Parser parser;
    bool ok = false;

    QByteArray json = body; json.chop(1); json.remove(0, 3);
    QVariant result = parser.parse(json, &ok);

    if (! ok)
        goto error;

    json_map = result.toMap();
    if (! json_map.contains("info"))
        goto error;

    json_info = json_map.value("info").toMap();
    if (! json_info.contains("tasks"))
        goto error;

    user_info = json_info.value("user").toMap();
    if (user_info.isEmpty())
        goto error;

    /// LOAD TASKS
    tc_cloudTasks.clear();

    foreach (const QVariant & taskItem, json_info.value("tasks").toList())
    {
        const QVariantMap & taskMap = taskItem.toMap();

        Thunder::Task task;
        task.id     = taskMap.value("id").toString();
        task.type   = Thunder::Single;

        task.source = taskMap.value("url").toString();
        task.cid    = taskMap.value("cid").toString();
        task.name   = taskMap.value("taskname").toString();
        task.link   = taskMap.value("lixian_url").toString();
        task.bt_url = taskMap.value("url").toString();
        task.size   = taskMap.value("ysfilesize").toULongLong();
        task.status = taskMap.value("download_status").toInt();

        if (! tmp_cookieIsStored)
        {
            tmp_cookieIsStored = true;
            tc_session.insert("gdriveid", user_info.value("cookie").toString());

            Util::writeFile(getCookieFilePath(),
                                ".vip.xunlei.com\tTRUE\t/\tFALSE\t90147186842\tgdriveid\t" +
                            tc_session.value("gdriveid").toAscii() + "\n");

//            Util::writeCookieToFile(getCookieFilePath(),
//                                    tc_nam->cookieJar()->cookiesForUrl(
//                                        QUrl("http://gdl.lixian.vip.xunlei.com")));

        }

        if (! task.isEmpty())
        {
            if (task.bt_url.startsWith("bt://"))
                task.type = Thunder::BT;
            tc_cloudTasks.push_back(task);

            local_taskids.append(task.id);
        }
    }

    /// MAGIC!
    delayCloudTask(local_taskids);

    error (tr("%1 task(s) loaded.").arg(tc_cloudTasks.size()), Notice);
    emit StatusChanged(TaskChanged);

    return;

error:
    error (tr("JSON parse error! Was the protocol changed?"), Warning);
    return;
}

void ThunderCore::removeCloudTasks(const QStringList &ids)
{
    post (QUrl("http://dynamic.cloud.vip.xunlei.com/interface/task_delete?type=2&callback=a"),
          "databases=0,&old_databaselist=&old_idlist=&taskids=" + ids.join(",").toAscii());
}

void ThunderCore::get(const QUrl &url)
{
    tc_nam->get(QNetworkRequest(url));
}

void ThunderCore::uploadBitorrent(const QString &file)
{
    tmp_btTask.subtasks.clear();

    const QByteArray & torrent = Util::readWholeFile(file);
    const QByteArray & boundary = Util::getRandomString(10).toAscii();

    QNetworkRequest request (QString("http://dynamic.cloud.vip.xunlei.com/"
                                     "interface/torrent_upload"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "multipart/form-data; boundary=----" + boundary);
    tc_nam->post(request, "------" + boundary + "\n"
                 "Content-Disposition: form-data; "
                 "name=\"filepath\"; filename=sample.torrent\n"
                 "Content-Type: application/x-bitorrent\n\n" + torrent +
                 "Content-Disposition: form-data; name=\"random\"\n\n" +
                 "13284335922471757912.3826739355\n"
                 "------" + boundary + "\n");
}

void ThunderCore::post(const QUrl &url, const QByteArray &body)
{
    QNetworkRequest request (url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    tc_nam->post(request, body);
}

void ThunderCore::login(const QString &user, const QString &passwd)
{
    tc_userName = user;
    tc_passwd   = passwd;

    error ("Getting capcha code ..", Info);
    this->get(QString("http://login.xunlei.com/check?u=%1&cachetime=%2")
              .arg(user)
              .arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
}
