#-------------------------------------------------
#
# Project created by QtCreator 2013-01-27T19:53:03
#
#-------------------------------------------------

QT       += core gui webkit sql network phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CloudClient
TEMPLATE = app

LIBS += -lqjson

isEmpty (PREFIX) {
    PREFIX = /usr
}
    
message ("Install prefix set to $${PREFIX}")

de.files = CloudClient.desktop
de.path = $${PREFIX}/share/applications

bi.files = CloudClient
bi.path = $${PREFIX}/bin/

icon.files = resources/images/thunder-256.png
icon.path = $${PREFIX}/share/icons/hicolor/256x256/apps/

INSTALLS += de bi icon

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/thundercore.cpp \
    src/util.cpp \
    src/thunderpanel.cpp \
    src/videopanel.cpp \
    src/qmpwidget.cpp \
    src/mplayer.cpp \
    src/logview.cpp \
    src/preferencesdlg.cpp \
    src/browser.cpp \
    src/addcloudtask.cpp \
    src/transf0r.cpp \
    src/downloader.cpp \
    src/downloaderchildwidget.cpp \
    src/saycapcha.cpp \
    src/fileselectorline.cpp \
    src/mediaplayer.cpp \
    src/osd.cpp \
    src/urllineedit.cpp \
    src/searchlineedit.cpp \
    src/simpleeditor.cpp

HEADERS  += src/mainwindow.h \
    src/thundercore.h \
    src/CloudObject.h \
    src/util.h \
    src/thunderpanel.h \
    src/videopanel.h \
    src/qmpwidget.h \
    src/mplayer.h \
    src/logview.h \
    src/preferencesdlg.h \
    src/browser.h \
    src/addcloudtask.h \
    src/transf0r.h \
    src/downloaderchildwidget.h \
    src/downloader.h \
    src/saycapcha.h \
    src/fileselectorline.h \
    src/mediaplayer.h \
    src/osd.h \
    src/urllineedit.h \
    src/searchlineedit.h \
    src/simpleeditor.h

FORMS    += ui/mainwindow.ui \
    ui/thunderpanel.ui \
    ui/videopanel.ui \
    ui/logview.ui \
    ui/preferencesdlg.ui \
    ui/browser.ui \
    ui/addcloudtask.ui \
    ui/transf0r.ui \
    ui/downloaderchildwidget.ui \
    ui/saycapcha.ui \
    ui/fileselectorline.ui \
    ui/simpleeditor.ui

RESOURCES += \
    resources.qrc
