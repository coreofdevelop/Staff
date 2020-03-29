#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QSqlError>

#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "Staff"
#define DATABASE_NAME       "DataBase.db"

#define STAFF_TABLE        "laboratory"
#define STAFF_NAME         "Имя"
#define STAFF_POSITION     "Должность"
#define STAFF_PASS         "Пропуск"
#define STAFF_MEDIC        "Медосмотр"
#define STAFF_CLOTHES      "Спецодежда"
#define STAFF_VACATION     "Отпуск"
#define STAFF_PHONE        "Телефон"
#define STAFF_BIRTH        "Дата рождения"


class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);

        /* Методы для непосредственной работы с классом
         * Подключение к базе данных и вставка записей в таблицу
         * */
        bool connectToDataBase();
        bool inserIntoStaffTable(const QVariantList &data);
        bool createStaffTable();

        bool removeRow(int id);




private:
        // Сам объект базы данных, с которым будет производиться работа
           QSqlDatabase    db;

};

#endif // DATABASE_H
