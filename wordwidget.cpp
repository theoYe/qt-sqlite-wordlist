#include "wordwidget.h"
#include "adddialog.h"

#include <QtWidgets>
WordWidget::WordWidget(QWidget * parent)
    :QTabWidget(parent)
{
    table = new WordTableModel(this);
    wordb = new WordDB();  //��ʼ�����ݿ�
//    newAddressTab = new NewAddressTab(this);
//    connect(newAddressTab, &NewAddressTab::sendDetails,
//        this, &AddressWidget::addEntry);

    //addTab(newAddressTab, "Address Book");
    wordb->insertRow(Word("aaa", "bbb"));
    setupTabs();
}


void WordWidget::importFromFile(const QString &fileName){}
void WordWidget::dumpToFile(const QString &fileName){}

void WordWidget::showAddEntryDialog(){

    AddDialog aDialog;

    if (aDialog.exec()){
        QString spelling= aDialog.spellingText->text();
        QString meaning= aDialog.meaningText->toPlainText();

        addEntry(spelling, meaning);
    }

}


void WordWidget::addEntry(QString spelling, QString meaning){
    if (!table->getWords().contains({ spelling, meaning })) {
        table->insertRows(0, 1, QModelIndex());  //0��ʾ�嵽��ǰ��,1����1��

        QModelIndex index = table->index(0, 0, QModelIndex());
        table->setData(index, spelling, Qt::EditRole);
        index = table->index(0, 1, QModelIndex());  // 0��1��
        table->setData(index, meaning, Qt::EditRole);
//        removeTab(indexOf(newmeaningTab));
    } else {
        QMessageBox::information(this, tr("Duplicate spelling"),
            tr("The spelling \"%1\" already exists.").arg(spelling));
    }
}
void WordWidget::editEntry(){
    QTableView *temp = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QString spelling;
    QString meaning;
    int row = -1;

    foreach (QModelIndex index, indexes) {
        row = proxy->mapToSource(index).row();
        QModelIndex nameIndex = table->index(row, 0, QModelIndex());
        QVariant varName = table->data(nameIndex, Qt::DisplayRole);
        spelling = varName.toString();

        QModelIndex meaningIndex = table->index(row, 1, QModelIndex());
        QVariant varAddr = table->data(meaningIndex, Qt::DisplayRole);
        meaning = varAddr.toString();
    }
//! [4a]

//! [4b]
    AddDialog aDialog;
    aDialog.setWindowTitle(tr("Edit a Contact"));


    aDialog.spellingText->setReadOnly(true);
    aDialog.spellingText->setText(spelling);
    aDialog.meaningText->setText(meaning);

    if (aDialog.exec()) {
        QString newMeaning = aDialog.meaningText->toPlainText();
        if (newMeaning != meaning) {
            QModelIndex index = table->index(row, 1, QModelIndex());
            table->setData(index, newMeaning, Qt::EditRole);
        }
    }
}
void WordWidget::removeEntry(){
    QTableView *temp = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();

    foreach (QModelIndex index, indexes) {
        int row = proxy->mapToSource(index).row();  //��ȡ�ڼ���
        table->removeRows(row, 1, QModelIndex());
    }

//    if (table->rowCount(QModelIndex()) == 0) {
//        insertTab(0, newAddressTab, "Address Book");
//    }

}



void  WordWidget::setupTabs(){
    QStringList groups;
    groups << "ABC" << "DEF" << "GHI" << "JKL" << "MNO" << "PQR" << "STU" << "VW" << "XYZ";

    for (int i = 0; i < groups.size(); ++i) {
        QString str = groups.at(i);
        QString regExp = QString("^[%1].*").arg(str);  //str �滻%1

        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(table);  //��������ģ��TableModel, ���ݱ仯�Զ�����view��
        proxyModel->setFilterRegExp(QRegExp(regExp, Qt::CaseInsensitive));
        proxyModel->setFilterKeyColumn(0);  //������������Ϊ���������ݵ���

        QTableView *tableView = new QTableView;
        tableView->setModel(proxyModel);

        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->hide();
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);

        tableView->setSortingEnabled(true);

        connect(tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &WordWidget::selectionChanged);

        connect(this, &QTabWidget::currentChanged, this, [this](int tabIndex) {
            auto *tableView = qobject_cast<QTableView *>(widget(tabIndex));
            if (tableView)
                emit selectionChanged(tableView->selectionModel()->selection());
        });

        addTab(tableView, str);
    }
}
