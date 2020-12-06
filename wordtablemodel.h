#ifndef WORDMODEL_H
#define WORDMODEL_H

#include<QAbstractTableModel>
#include <QList>
#include <QMap>
#include "sqlite3.h"
#include "Utils.h"
#include "fileapi.h"
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

    QList<Word> words;
};




class WordDB{
public:
    //��ʼ�����ݿ⣨�����������½����ݿ⣩
    WordDB(){
       initDB();
    }

    static QMap<QString, QString> map ;
//    static QList<Word> wordList;

    int initDB(){
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
    int getAll(QList<Word> &wordList){

        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char *sql;
        const char* data = "Callback function called";

        /* Open database */
        rc = sqlite3_open("words.db", &db);
        if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
        } else {
        fprintf(stderr, "Opened database successfully\n");
        }

        /* Create SQL statement */
        sql = "SELECT * from WORD";

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
        if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        } else {
        fprintf(stdout, "Operation done successfully\n");
          //wordList.push_back(Word(i.key(), i.value()));

        QMapIterator<QString, QString> i(WordDB::map);
        while (i.hasNext()) {
            i.next();
            wordList.push_front(Word(i.key(), i.value()));
        }
        }
        sqlite3_close(db);
        return 0;
    }

    //callback ��������ʹ�� select��ѯʱ�ᱻ���ö��
    static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

    }
    WordDB::map[argv[1]] = argv[2];
    printf("\n");
    return 0;

    printf("callback called\n");
    }

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
                "VALUES ('%s', '%s');\0", word.spelling.toUtf8().data(),word.meaning.toUtf8().data()
                );

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
    int deleteRow(Word word){
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char  sql[2048];
        const char* data = "Callback function called";

        /* Open database */
        rc = sqlite3_open("words.db", &db);
        if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
        } else {
        fprintf(stderr, "Opened database successfully\n");
        }

        /* Create merged SQL statement */
        sprintf(sql , "DELETE from WORD where spell='%s';", word.spelling.toUtf8().data());


        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
        if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        } else {
        fprintf(stdout, "Operation done successfully\n");
        }
        sqlite3_close(db);
        return 0;
    }

    //�༭
    int updateRow(Word oldWord , Word newWord){

        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char sql[2048];
        const char* data = "Callback function called";

        /* Open database */
        rc = sqlite3_open("words.db", &db);
        if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
        } else {
        fprintf(stderr, "Opened database successfully\n");
        }

        /* Create merged SQL statement */
        sprintf(sql ,"UPDATE  WORD set meaning='%s' where spell='%s';",
                newWord.meaning.toUtf8().data(),oldWord.spelling.toUtf8().data());


        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, NULL, (void*)data, &zErrMsg);
        if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        } else {
        fprintf(stdout, "Operation done successfully\n");
        }
        sqlite3_close(db);
    }

    //����
    int dumpToFile(const QString &fileName){
        sqlite3 *db;
        FILE    *dump_file;
        int i;
        sqlite3_stmt *stmt;
        int col_num = 3;


        int rc;
        rc = sqlite3_open("words.db", &db);
        if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
        } else {
        fprintf(stderr, "Opened database successfully\n");
        }



        dump_file = fopen(fileName.toUtf8().data(), "w+");
        if (dump_file == NULL) {
            /* Error handling with errno and exit */
        }

        sqlite3_prepare_v2(db, "SELECT id, spell, meaning FROM WORD;",
                           100, &stmt, NULL);
        /* dump columns names into the file */
        for (i = 0; i < col_num; i++) {
            if(i != col_num-1)
                fprintf (dump_file, "%s,", sqlite3_column_name(stmt, i));
            else
                fprintf (dump_file, "%s", sqlite3_column_name(stmt, i));
        }

        fprintf(dump_file , "\n");
        /* Dump columns data into the file */
        while (SQLITE_ROW == sqlite3_step(stmt)) {
            for (i = 0; i < col_num; i++) {
                if(i != col_num-1)
                    fprintf (dump_file, "%s,", sqlite3_column_text(stmt, i));
                else
                    fprintf (dump_file, "%s", sqlite3_column_text(stmt, i));
            }
          fprintf (dump_file,"\n");
        }
        /* We're ready to leave */
        sqlite3_finalize (stmt);
        fflush(dump_file);  //���ӿ��ܻ�û����
        sqlite3_close(db);
    }

    //����
    int importFromFile(QFile fileName);
};

//WordDB::map = QMap<QString, QString>();




#endif // WORDMODEL_H
