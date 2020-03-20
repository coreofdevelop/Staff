#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Сотрудники");

        /* Первым делом необходимо создать объект для работы с базой данных
         * и инициализировать подключение к базе данных
         * */
        db = new DataBase(this);
        if(!db->connectToDataBase()){
            ui->statusbar->showMessage("Ошибка подключения файла базы данных!", 5000);
        }else {
             ui->statusbar->showMessage("Seccess!", 2000);
        }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_add_table_triggered()
{
    db->createStaffTable();
}
