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



void MainWindow::on_addEmloyee_triggered()
{
    /* Создаем диалог и подключаем его сигнал завершения работы
         * к слоту обновления вида модели представления данных
         * */
        EditForm *editForm = new EditForm();
        connect(editForm, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

        /* Выполняем запуск диалогового окна
         * */
        editForm->setWindowTitle(tr("Добавить Сотрудника"));
        editForm->exec();
}

void MainWindow::on_addTable_triggered()
{
    db->createStaffTable();
}

void MainWindow::slotUpdateModels()
{

}

void MainWindow::slotEditRecord(int id)
{

}
