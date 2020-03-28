#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSettings>

#include "database.h"
#include "editform.h"
#include "filterproxymodel.h"
#include "setform.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, QString name = "Staff");
    ~MainWindow();

    // Функции загрузки и сохранения настроек
    void saveSattings();
    void loadSettings();

private slots:
    void on_addEmloyee_triggered();

    void on_addTable_triggered();

    void slotUpdateModels();

    void slotEditRecord(int id);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_Settings_triggered();

private:
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();

private:
    Ui::MainWindow              *ui;
    DataBase                    *db;
    QSqlTableModel              *modelStaff;
    FilterProxyModel            *filter;
    QSettings                   *settings;
    SetForm                     *setForm;

    // Настройки программы
    QList<bool> Column{1,1,1,1,1,1,1};
    QList<int> Month{3,2,0,0};
};
#endif // MAINWINDOW_H
