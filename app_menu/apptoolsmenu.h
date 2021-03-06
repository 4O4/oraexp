#ifndef APPTOOLSMENU_H
#define APPTOOLSMENU_H

#include "appmainmenu.h"
#include "connection_page/dbuimanager.h"

class QMenu;
class QToolBar;
class QAction;

class AppToolsMenu : public AppMainMenu
{
    Q_OBJECT
public:
    explicit AppToolsMenu(QMenu *toolsMenu, QToolBar *toolbar, QObject *parent);

    void updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab);

private:
    void setupMenu(QMenu *toolsMenu, QToolBar *toolbar);

    QAction *toolsCompareSchemasAction;
    QAction *toolsExportSchemaAction;

    QAction *toolsCompareDataAction;
    QAction *toolsCopyDataAction;
    QAction *toolsExportDataAction;
    QAction *toolsImportDataAction;

    QAction *toolsObjectLookupAction;

    QAction *toolsSessionBrowserAction;

    QAction *toolsOptionsAction;

private slots:
    void addSchemaComparer(){uiManager()->addSchemaComparer();}
    void addSchemaExporter(){uiManager()->addSchemaExporter();}
    void addDataComparer(){uiManager()->addDataComparer();}
    void addDataCopier(){uiManager()->addDataCopier();}
    void addDataExporter(){uiManager()->addDataExporter();}
    void addDataImporter(){uiManager()->addDataImporter();}

    void showObjectLookupDialog(){uiManager()->showObjectLookupDialog();}

    void addSessionBrowser(){uiManager()->addSessionBrowser();}

    void showOptionsDialog();

#ifdef DEBUG
    void resize800x600();
    void resize1024x768();
#endif

};

#endif // APPTOOLSMENU_H
