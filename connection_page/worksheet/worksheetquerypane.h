#ifndef WORKSHEETQUERYPANE_H
#define WORKSHEETQUERYPANE_H

#include <QWidget>
#include "connectivity/queryresult.h"
#include "connectivity/ociexception.h"
#include "codeeditor/codeeditor.h"
#include "widgets/codeeditorandsearchpanewidget.h"

class WorksheetCodeEditor;
class QToolButton;
class DbConnection;
class QToolBar;
class CodeEditor;

class WorksheetQueryPane : public QWidget
{
    Q_OBJECT
public:
    explicit WorksheetQueryPane(QWidget *parent = 0);

    void setContents(const QString &contents);

    void setConnection(DbConnection *db);

    void showSearchWidget(){codeEditorAndSearchPaneWidget->showSearchPane();}
    void findNext(){codeEditorAndSearchPaneWidget->findNext();}
    void findPrevious(){codeEditorAndSearchPaneWidget->findPrevious();}

signals:
    void queryDone(const QueryResult &result);

private:
    DbConnection *db;
    CodeEditor *codeEditor;
    CodeEditorAndSearchPaneWidget *codeEditorAndSearchPaneWidget;
    QToolBar *toolbar;
    QAction *progressBarAction;

private slots:
    void executeQuery();
    void queryCompleted(const QueryResult &result);

};

#endif // WORKSHEETQUERYPANE_H
