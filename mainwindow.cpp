#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


//! [0]
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
     wordWidget = new WordWidget();
     setCentralWidget(wordWidget);
     createMenus();
     setWindowTitle(tr("Word Book"));
}
//! [0]

//! [1a]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    importAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(importAct);
    connect(importAct, &QAction::triggered, this, &MainWindow::openFile);
//! [1a]

    dumpAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(dumpAct);
    connect(dumpAct, &QAction::triggered, this, &MainWindow::saveFile);

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    addAct = new QAction(tr("&Add Entry..."), this);
    toolMenu->addAction(addAct);
    connect(addAct, &QAction::triggered, wordWidget, &WordWidget::showAddEntryDialog);

//! [1b]
    editAct = new QAction(tr("&Edit Entry..."), this);
    editAct->setEnabled(false);
    toolMenu->addAction(editAct);
    connect(editAct, &QAction::triggered, wordWidget, &WordWidget::editEntry);

    toolMenu->addSeparator();

    removeAct = new QAction(tr("&Remove Entry"), this);
    removeAct->setEnabled(false);
    toolMenu->addAction(removeAct);
    connect(removeAct, &QAction::triggered, wordWidget, &WordWidget::removeEntry);

    connect(wordWidget, &WordWidget::selectionChanged,
        this, &MainWindow::updateActions);
}
//! [1b]

//! [2]
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        wordWidget->importFromFile(fileName);
}
//! [2]

//! [3]
void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
        wordWidget->dumpToFile(fileName);
}
//! [3]

//! [4]
void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();

    if (!indexes.isEmpty()) {
        removeAct->setEnabled(true);
        editAct->setEnabled(true);
    } else {
        removeAct->setEnabled(false);
        editAct->setEnabled(false);
    }
}
//! [4]
