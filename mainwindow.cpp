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


        /* Инициализируем модели для представления данных
             * с заданием названий колонок
             * */
             this->setupModel(STAFF_TABLE,
                              QStringList() << tr("id")
                              << tr("Имя")
                              << tr("Должность")
                              << tr("Пропуск")
                              << tr("Медосмотр")
                              << tr("Спецодежда")
                              << tr("Отпуск")
                              << tr("Телефон")
                              << tr("Дата рождения")
                              );
               /* Инициализируем внешний вид таблицы с данными
               * */
               this->createUI();

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
    modelStaff->select();
}

void MainWindow::slotEditRecord(int id)
{

    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    EditForm *editForm = new EditForm(id);
    connect(editForm, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    editForm->setWindowTitle(("Редактировать Сотрудника"));
    editForm->exec();

}

void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{

    /* Производим инициализацию модели представления данных
    * */
    modelStaff = new QSqlTableModel(this);
    modelStaff->setTable(tableName);
    modelStaff->select();

    filter = new FilterProxyModel(this);
    filter->setSourceModel(modelStaff);


    /* Устанавливаем названия колонок в таблице с сортировкой данных
    * */
    for(int i = 0, j = 0; i < modelStaff->columnCount(); i++, j++){
       modelStaff->setHeaderData(i,Qt::Horizontal,headers[j]);
    }

}

void MainWindow::createUI()
{
    //ui->tableView->setModel(modelStaff);     // Устанавливаем модель на TableView
        ui->tableView->setModel(filter);
        ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
        //настройки отображения колонок
        ui->tableView->setColumnHidden(4, !Column[0]);
        ui->tableView->setColumnHidden(5, !Column[1]);
        ui->tableView->setColumnHidden(6, !Column[2]);
        ui->tableView->setColumnHidden(7, !Column[3]);
        // Разрешаем выделение строк
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        // Устанавливаем режим выделения лишь одно строки в таблице
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        // Устанавливаем разтяжение колонок
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // Устанавливаем размер колонок по содержимому
        ui->tableView->resizeColumnsToContents();
        // Запрещаем отдельное редактирование ячейки
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        // Разрешаем сортировку
        ui->tableView->setSortingEnabled(true);

}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    // Получаем индек ячейки
        QModelIndex idIndex = ui->tableView->model()->index(index.row(), 0, index.parent());
        // Если индекс валиден
        if (idIndex.isValid()){
        int id = idIndex.data().toUInt();
        slotEditRecord(id);
        }
        else
        {
            return;
        }
}
