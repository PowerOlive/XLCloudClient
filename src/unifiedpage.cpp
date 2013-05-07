#include "unifiedpage.h"

UnifiedPage::UnifiedPage(QObject *parent) :
    QWebPage(parent)
{
}

bool UnifiedPage::acceptNavigationRequest(QWebFrame *frame,
                                          const QNetworkRequest &request,
                                          NavigationType type)
{
    if (type == QWebPage::NavigationTypeLinkClicked)
    {
        mainFrame()->load(request);
        return true;
    }

    return QWebPage::acceptNavigationRequest(frame, request, type);
}
