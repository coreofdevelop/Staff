#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString name)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Сотрудники");

    settings = new QSettings ("settings.ini", QSettings::IniFormat, this);
        setObjectName(name);
        setWindowTitle(name);
        loadSettings();

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
    saveSattings();
    delete ui;
}

void MainWindow::saveSattings()
{
    settings->beginGroup("forms");
    settings->beginGroup(objectName());
    settings->setValue("geometryMain", geometry());
    settings->endGroup();
    settings->endGroup();
    settings->beginGroup("Colum");
    for (int i = 0; i < 6; i++) {
        settings->setValue("Colum" + QString::number(i), Column[i]);
    }
    settings->endGroup();
    settings->beginGroup("Filter");
    for (int i = 0; i < 4; i++) {
        settings->setValue("Filter" + QString::number(i), Month[i]);
        qDebug() << Month[i];
    }
    settings->endGroup();
}

void MainWindow::loadSettings()
{
    settings->beginGroup("forms");
    settings->beginGroup(objectName());
    setGeometry(settings->value("geometryMain", QRect(300,200,600,300)).toRect());
    settings->endGroup();
    settings->endGroup();
    settings->beginGroup("Colum");
    for (int i = 0; i < 6; i++) {
        Column[i] = settings->value("Colum" + QString::number(i), Column[i]).toBool() ;
    }
    settings->endGroup();
    settings->beginGroup("Filter");
    for (int i = 0; i < 4; i++) {
        Month[i] = settings->value("Filter" + QString::number(i), Month[i]).toInt();
        qDebug() << Month[i];
    }
    settings->endGroup();
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

        //настройки отображения колонок подгружаемые из файла настроек
        ui->tableView->setColumnHidden(2, !Column[0]);
        ui->tableView->setColumnHidden(4, !Column[1]);
        ui->tableView->setColumnHidden(5, !Column[2]);
        ui->tableView->setColumnHidden(6, !Column[3]);
        ui->tableView->setColumnHidden(7, !Column[4]);
        ui->tableView->setColumnHidden(8, !Column[5]);

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

void MainWindow::on_Settings_triggered()
{
    setForm = new SetForm ();
        // Передаем текущие настройки
        setForm->setSetting(Column, Month);

        if(setForm->exec()) {
            Column = setForm->columnSetting();
            Month = setForm->monthSetting();
            createUI();
            this->slotUpdateModels();
            qDebug() << Column;

        }
}
