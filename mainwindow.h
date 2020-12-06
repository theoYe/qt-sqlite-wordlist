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


    //�˵��� file tool
    QMenu *fileMenu;   //���ݿ��ļ����뵼��
    QMenu *toolMenu;   //����

    //file:�˵���
    QAction *importAct;   //�򿪵������ݿ��ļ�
    QAction *dumpAct;   //�������ݿ��ļ�
    QAction *exitAct;   //�˳�

    //tool:�˵���
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;
};
#endif // MAINWINDOW_H
