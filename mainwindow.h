#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>
#include "wordwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void updateActions(const QItemSelection &selection);
    void openFile();
    void saveFile();

private:
    void createMenus();


    WordWidget * wordWidget;


    //菜单栏 file tool
    QMenu *fileMenu;   //数据库文件导入导出
    QMenu *toolMenu;   //工具

    //file:菜单项
    QAction *importAct;   //打开导入数据库文件
    QAction *dumpAct;   //保存数据库文件
    QAction *exitAct;   //退出

    //tool:菜单项
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;
};
#endif // MAINWINDOW_H
