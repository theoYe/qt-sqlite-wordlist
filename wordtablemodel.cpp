#include "wordtablemodel.h"

//! [0]
WordTableModel::WordTableModel(QObject * parent)
    : QAbstractTableModel(parent)
{}
WordTableModel::WordTableModel(QList<Word> words, QObject *parent )
    :QAbstractTableModel(parent),words(words)
{}


//! [0]

//! [1]
int WordTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return words.size();
}

int WordTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}
//! [1]

//! [2]
QVariant WordTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= words.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &word = words.at(index.row());

        if (index.column() == 0)
            return word.spelling;
        else if (index.column() == 1)
            return word.meaning;
    }
    return QVariant();
}
//! [2]

//! [3]
QVariant WordTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("spelling");

            case 1:
                return tr("meaning");

            default:
                return QVariant();
        }
    }
    return QVariant();
}
//! [3]

//! [4]
bool WordTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        words.insert(position, { QString(), QString() });

    endInsertRows();
    return true;
}
//! [4]

//! [5]
bool WordTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        words.removeAt(position);

    endRemoveRows();
    return true;
}
//! [5]

//! [6]
bool WordTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();  //ÐÐÊý
        auto word = words.value(row);

        if (index.column() == 0)
            word.spelling = value.toString();
        else if (index.column() == 1)
            word.meaning = value.toString();
        else
            return false;

        words.replace(row, word);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}
//! [6]

//! [7]
Qt::ItemFlags WordTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
//! [7]

//! [8]
QList<Word> WordTableModel::getWords() const
{
    return words;
}
//! [8]
