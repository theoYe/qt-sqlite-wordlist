#ifndef WORDMODEL_H
#define WORDMODEL_H

#include<QAbstractTableModel>
#include <QList>
#include "sqlite3.h"

struct Word
{
    QString spelling;
    QString meaning;
    Word(){}
    Word(QString spelling,QString meaning)
        :spelling(spelling),meaning(meaning){}



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
    //��ʼ�����ݿ⣨�����������½����ݿ⣩
    WordDB(){
       initDB();
    }

    static int initDB(){
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char *sql;

        /* �����ݿ� */
        rc = sqlite3_open("words.db", &db);
        if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        } else {
        fprintf(stdout, "Opened database successfully\n");
        }

        /* Create SQL statement */
        sql = "CREATE TABLE IF NOT EXISTS WORD(" \
        "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
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

    //��ȡ����
    QList<Word> getAll();

    //����
    int insertRow(Word word){
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char sql[2048];

        /* Open database */
        rc = sqlite3_open("words.db", &db);
        if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
        } else {
        fprintf(stderr, "Opened database successfully\n");
        }

        /* Create SQL statement */

        sprintf(sql ,"INSERT INTO WORD (SPELL,MEANING) " \
                "VALUES ('%s', '%s');\0", word.spelling.data(),word.meaning.data()
                );

//        sql = "INSERT INTO WORD (ID,NAME,AGE,ADDRESS,SALARY) " \
//        "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
//        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) " \
//        "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); " \
//        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
//        "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
//        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
//        "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
        if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        } else {
        fprintf(stdout, "Records created successfully\n");
        }
        sqlite3_close(db);
        return 0;
    }

    //ɾ��
    int deleteRow(Word word);

    //�༭
    int updateRow(Word oldWord , Word newWord);

    //����
    int dumpToFile(QFile fileName);

    //����
    int importFromFile(QFile fileName);
};


#endif // WORDMODEL_H
