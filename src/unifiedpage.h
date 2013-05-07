#ifndef UNIFIEDPAGE_H
#define UNIFIEDPAGE_H

#include <QWebPage>
#include <QDebug>
#include <QWebFrame>
#include <QNetworkRequest>
#include <QAction>

class UnifiedPage : public QWebPage
{
    Q_OBJECT
public:
    explicit UnifiedPage(QObject *parent = 0);
    
signals:

protected:
    bool acceptNavigationRequest(QWebFrame *frame,
                                 const QNetworkRequest &request,
                                 NavigationType type);
    
public slots:
    
};

#endif // UNIFIEDPAGE_H
