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
//    wordb->insertRow(Word("aa1", "aaaaaa"));
//    wordb->insertRow(Word("aa2", "bbbbbb"));
//    wordb->updateRow(Word("aa2","bbbbb"), Word("aa3","33333"));


    wordb->getAll(table->words);
//    fprintf(stderr,"list:%s",WordDB::wordList[1].meaning.toUtf8().data());
    setupTabs();

}

void WordWidget::updateTable(){
    for (int i = 0; i < table->words.size(); ++i) {
        addEntry(table->words.at(i));
    }
}

void WordWidget::addEntry(Word word){
     addEntry(word.spelling.toUtf8().data() , word.meaning.toUtf8().data());
}
void WordWidget::importFromFile(const QString &fileName){}
void WordWidget::dumpToFile(const QString &fileName){


    wordb->dumpToFile(fileName);
}

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

        wordb->insertRow(Word(spelling, meaning));
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

           //�������ݿ�
            wordb->updateRow(Word( spelling, meaning) , Word(spelling, newMeaning));
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
        //���ݿ�ɾ�� �����������ɾ��ǰ�棬 ����index.row()���صĽ���Ǹ��¹���
        Word word =table->words.at(index.row());
        wordb->deleteRow(word);
        //����ɾ��
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
