#ifndef WORDMODEL_H
#define WORDMODEL_H

#include<QAbstractTableModel>
#include <QList>
#include "sqlite3.h"

struct Word
{
    QString spelling;
    QString meaning;

    bool operator==(const Word &other) const
    {
        return spelling == other.spelling && meaning == other.meaning;
    }
};

inline QDataStream &operator<<(QDataStream &stream, const Word &word)
{
    return stream << word.spelling << word.meaning;
}

inline QDataStream &operator>>(QDataStream &stream, Word &word)
{
    return stream >> word.spelling >> word.meaning;
}



class WordTableModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    WordTableModel(QObject *parent = 0);
    WordTableModel(QList<Word> words, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    QList<Word> getWords() const;
private:
    QList<Word> words;
};


class WordDB{
public:
    //初始化数据库（若不存在则新建数据库）
    WordDB(){
       initDB();
    }

    static int initDB(){
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char *sql;

        /* 打开数据库 */
        rc = sqlite3_open("words.db", &db);
        if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        } else {
        fprintf(stdout, "Opened database successfully\n");
        }

        /* Create SQL statement */
        sql = "CREATE TABLE IF NOT EXISTS WORD(" \
        "ID INT PRIMARY KEY NOT NULL," \
        "SPELL TEXT NOT NULL," \
        "MEANING TEXT NOT NULL);" ;

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
        if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return 0;
        } else {
        fprintf(stdout, "Table created successfully\n");
        }
        sqlite3_close(db);
    }

    //获取数据
    QList<Word> getAll();

    //插入
    int insertRow(Word word);

    //删除
    int deleteRow(Word word);

    //导出
    int dumpToFile(QFile fileName);

    //导入
    int importFromFile(QFile fileName);
};


#endif // WORDMODEL_H
