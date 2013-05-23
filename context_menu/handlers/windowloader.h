#ifndef WINDOWLOADER_H
#define WINDOWLOADER_H

#include "contextmenuhandler.h"
#include "beans/dynamicwindowinfo.h"
#include <QDomNodeList>

class DynamicConnectionPageWindow;

class WindowLoader : public ContextMenuHandler
{
public:
    explicit WindowLoader();
    virtual ~WindowLoader(){}
    
    virtual void handle(DbUiManager *uiManager, const QHash<QString,QString> &properties);

private:
    DynamicConnectionPageWindow *createDynamicWindow(const QHash<QString,QString> &properties);
    DynamicWindowInfo readWindowInfo(const QString windowName);
    void readWidgetList(DynamicWindowInfo &windowInfo, const QDomNodeList &widgetNodes);

    
};

Q_DECLARE_METATYPE(WindowLoader)

#endif // WINDOWLOADER_H