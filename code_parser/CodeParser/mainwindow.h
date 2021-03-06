#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "../ireducelistener.h"

class ParseTreeNode;
class QStandardItemModel;
class QStandardItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent ( QCloseEvent * event );
    
private slots:
    void on_actionScan_triggered();

    void on_actionParse_triggered();

    void nodeActivated(const QModelIndex &index);

    void cursorPositionChanged();

    void on_actionParse_All_triggered();

    void on_actionNon_strict_parse_triggered();

    void on_actionFind_declaration_triggered();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;

    bool parseCurrent(bool constructDisplayTree, bool strictMode);
    void buildTree(ParseTreeNode *rootNode);
    void addChildNodes(ParseTreeNode *parseTreeNode, QStandardItem *treeViewNode);

};

#endif // MAINWINDOW_H
