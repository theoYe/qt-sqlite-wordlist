#ifndef WORDWIDGET_H
#define WORDWIDGET_H

#include <QTabWidget>
#include "wordtablemodel.h"
#include <QSortFilterProxyModel>
#include <QWidget>
#include <QWidget>


#include <QItemSelection>
#include <QTabWidget>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE


class WordWidget: public QTabWidget
{
    Q_OBJECT
public:
    WordWidget(QWidget *parent = 0);
    void importFromFile(const QString &fileName);
    void dumpToFile(const QString &fileName);

public slots:
    void showAddEntryDialog();
    void addEntry(QString spelling, QString meaning);
    void editEntry();
    void removeEntry();

signals:
    void selectionChanged (const QItemSelection &selected);

private:
    void setupTabs();

    WordTableModel *table;
    //NewAddressTab *newAddressTab;
    QSortFilterProxyModel *proxyModel;
};
//! [0]

#endif // WORDWIDGET_H
