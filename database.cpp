#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

bool DataBase::connectToDataBase()
{
    /* База данных открывается по заданному пути
         * и имени базы данных, если она существует
         * */
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName(DATABASE_HOSTNAME);
        db.setDatabaseName("DataBase/" DATABASE_NAME);
        if(db.open()){
            return true;
        } else {
            return false;
        }
}

bool DataBase::inserIntoStaffTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
         * в который передаются данные для вставки в таблицу.
         * */
        QSqlQuery query;
        /* В начале SQL запрос формируется с ключами,
         * которые потом связываются методом bindValue
         * для подстановки данных из QVariantList
         * */
        query.prepare("INSERT INTO " STAFF_TABLE " ( " STAFF_NAME ", "
                                                        STAFF_PASS ", "
                                                        STAFF_MEDIC ", "
                                                        STAFF_CLOTHES ", "
                                                        STAFF_PHONE ", "
                                                        STAFF_BIRTH " ) "
                      "VALUES (?, ?, ?, ?, ?, ?)");
        query.addBindValue(data[0].toString());
        query.addBindValue(data[1].toString());
        query.addBindValue(data[2].toString());
        query.addBindValue(data[3].toString());
        query.addBindValue(data[4].toString());
        query.addBindValue(data[6].toString());

        // После чего выполняется запросом методом exec()
        if(!query.exec()){
            qDebug() << "error insert into " << STAFF_TABLE;
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
}

bool DataBase::createStaffTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
         * с последующим его выполнением.
         * */
        QSqlQuery query;
        if(!query.exec( "CREATE TABLE " STAFF_TABLE " ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                STAFF_NAME  " VARCHAR(255)    NOT NULL,"
                                STAFF_PASS        " DATE,"
                                STAFF_MEDIC       " DATE,"
                                STAFF_CLOTHES     " DATE,"
                                STAFF_PHONE       " VARCHAR(36),"
                                STAFF_BIRTH       " DATE"
                            " )"
                        )){
            qDebug() << "DataBase: error of create " << STAFF_TABLE;
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }

}
