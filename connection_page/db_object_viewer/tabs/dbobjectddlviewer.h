#ifndef DBOBJECTDDLVIEWER_H
#define DBOBJECTDDLVIEWER_H

#include <QWidget>
#include "../../db_object_viewer/dbobjectviewergenerictab.h"
#include "connectivity/fetchresult.h"
#include "beans/objectddloptions.h"

class MultiEditorWidget;

class DbObjectDdlViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectDdlViewer(bool addSettingsButton, DbUiManager *uiManager, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual void loadData();

    virtual void createMainWidget(QLayout *);

    virtual QList<QAction*> getSpecificToolbarButtons(QToolBar *toolbar);

    void setDbObjectTypeName(const QString &dbObjectTypeName);

    void setPairEditorWidget(MultiEditorWidget *multiEditor);
    MultiEditorWidget *getEditorWidget() const;

signals:
    void switchToPair();

private slots:
    void ddlQueryCompleted(const QueryResult &result);
    void ddlFetched(const FetchResult &fetchResult);
    void fetchCompleted(const QString &);

    void showDdlOptions();

private:
    MultiEditorWidget *multiEditor;

    ObjectDdlOptions ddlOptions;

    bool addSettingsButton;

    QString dbObjectTypeName;

};

#endif // DBOBJECTDDLVIEWER_H
