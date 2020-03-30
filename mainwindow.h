#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractButton>

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
    void on_addTable_triggered();
    void slotUpdateModels();
    void slotEditRecord(int id);
    void on_Settings_triggered();
    void on_SelectionChanged();

    // методы включения фильтров и их сброс
    void on_filterPass_triggered();
    void on_filterMed_triggered();
    void on_filterClouthes_triggered();
    void on_filterVacation_triggered();
    void on_filterReset_triggered();

    // методы редоктиорвания записей
    void on_addEmloyee_triggered();
    void on_changeEmployee_triggered();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_deleteEmployee_triggered();

    void on_reloadBd_triggered();
    void on_newDatabase_triggered();

    void on_open_triggered();
    void on_saveAs_triggered();    
    void on_quit_triggered();

    void on_about_triggered();

private:
    void setupModel(const QString &tableName);
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
    QString fileName = "DataBase/DataBase.db";
};
#endif // MAINWINDOW_H
