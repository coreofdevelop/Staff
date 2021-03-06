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
        //setWindowTitle(name);
        loadSettings();


        /* Первым делом необходимо создать объект для работы с базой данных
         * и инициализировать подключение к базе данных
         * */
        db = new DataBase(this);

        // Проверим существование файла
        if(!QFile::exists(fileName)){
            QMessageBox::critical(this, tr("Ошибка открытия файла базы данных"), tr("Не удалось открыть базу данных, файл не был найден! "
                                                                             "<p>Укажите пожалуйста путь к файлу."));
            QString filePath = QFileDialog::getOpenFileName(this, tr("Открыть файл базы данных"),
                                                            fileName,
                                                            tr("Файлы базы данных (*.db);;Все файлы (*.*)"));
            if(filePath.isEmpty())
                return;

            fileName = filePath;
        }

        if(!db->connectToDataBase(fileName)){
            ui->statusbar->showMessage("Ошибка подключения файла базы данных!", 5000);
        }else {
             ui->statusbar->showMessage("Seccess!", 2000);

        /* Инициализируем модели для представления данных
             * с заданием названий колонок
             * */
             this->setupModel(STAFF_TABLE);
               /* Инициализируем внешний вид таблицы с данными
               * */
               this->createUI();

}
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
    settings->beginGroup("SettingsTable");
    settings->beginGroup("size");
    settings->setValue("tableSize", ui->tableView->geometry());
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
    settings->beginGroup("SettingsTable");
    settings->beginGroup("size");
    ui->tableView->setGeometry(settings->value("tableSize", QRect(0,0,700,250)).toRect());
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
    on_SelectionChanged();
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

void MainWindow::setupModel(const QString &tableName)
{

    /* Производим инициализацию модели представления данных
    * */

    filter = new FilterProxyModel(this);
    modelStaff = new QSqlTableModel(this);

    modelStaff->setTable(tableName);
    modelStaff->select();

    filter->setSourceModel(modelStaff);

    /* Устанавливаем названия колонок в таблице с сортировкой данных
    * */
    QStringList headers = QStringList() << tr("id")
    << tr("Имя")
    << tr("Должность")
    << tr("Пропуск")
    << tr("Медосмотр")
    << tr("Спецодежда")
    << tr("Отпуск")
    << tr("Телефон")
    << tr("Дата Рождения");

    for(int i = 0, j = 0; i < modelStaff->columnCount(); i++, j++){
       modelStaff->setHeaderData(i,Qt::Horizontal,headers[j]);
    }

}

void MainWindow::createUI()
{
    //ui->tableView->setModel(modelStaff);     // Устанавливаем модель на TableView
    //setColumnsWidth();
        ui->tableView->setModel(filter);
        ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей

        //настройки отображения колонок подгружаемые из файла настроек
        ui->tableView->setColumnHidden(2, !Column[0]);
        ui->tableView->setColumnHidden(4, !Column[1]);
        ui->tableView->setColumnHidden(5, !Column[2]);
        ui->tableView->setColumnHidden(6, !Column[3]);
        ui->tableView->setColumnHidden(7, !Column[4]);
        ui->tableView->setColumnHidden(8, !Column[5]);
        // включение отключение кнопок фильтра
        ui->filterMed->setEnabled(Column[1]);
        ui->filterClouthes->setEnabled(Column[2]);
        ui->filterVacation->setEnabled(Column[3]);

        // Разрешаем выделение строк
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        // Устанавливаем режим выделения лишь одно строки в таблице
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

        // Устанавливаем размеры колонок
        this->setColumnsWidth();

        // Запрещаем отдельное редактирование ячейки
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        // Разрешаем сортировку
        ui->tableView->setSortingEnabled(true);

        // соеденяем сигнал выбора ячейки, с нашим слотом выключения кнопок удаления и редактиорования сотрудника
        connect(ui->tableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                   this,SLOT(on_SelectionChanged()));
        // отмена выбора
        connect(ui->tableView->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                    this,SLOT(on_SelectionChanged()));


}

void MainWindow::setColumnsWidth()
{
    QRect rect = ui->tableView->rect();
    // Зададим приоритеты столбцам
    float columnPriority[] = {0.0f, 1.0f, 0.75f, 0.5f, 0.5f, 0.5f, 0.5f, 0.6f, 0.5f};
    // Количество отображаемых столбцов
    int columnCount = 0;
    // Последний отображаемый столбец
    int lastVisibleColumn = 0;
    // Сумма приоритетов для нахождения коэфициента
    float columnPrioritySum = 0;
    // массив для хранения номеров столбцов
    int columnNumb[9];
    int j = 0;
    for (int i = 0; i < 9; i++){
        if(!ui->tableView->isColumnHidden(i)){
            columnCount++;
            columnPrioritySum += columnPriority[i];
            columnNumb[j] = i;
            j++;
        }
    }

    // Зададим размеры всем отображаемым столбцам, кроме последнего
    for (int i = 0; i < columnCount-1; i++){
        ui->tableView->setColumnWidth(columnNumb[i],
                       int(columnPriority[columnNumb[i]]*float(rect.width()-15)/columnPrioritySum));
    }

    // Найедм последний отображенный столбец
    for (int i=0; i < 9; i++) {
        if(!ui->tableView->isColumnHidden(i)){
            lastVisibleColumn = i;
        }
    }

    // Последний столбец растянем до конца оставшейся таблицы (это для компенсации погрешностей в расчетах ширины столбцов)
     ui->tableView->horizontalHeader()->setSectionResizeMode(lastVisibleColumn , QHeaderView::Stretch);

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // Переопределенная функция (обработчик событий) для получения события об изменении размера основного окна

    QWidget::resizeEvent(event);
    this->setColumnsWidth();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    // Получаем индек ячейки
        QModelIndex idIndex = ui->tableView->model()->index(index.row(), 0, index.parent());
        // Если индекс валиден
        if (idIndex.isValid()){
        int id = idIndex.data().toInt();
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
        // Обновим все данные в программе
            createUI();
            this->slotUpdateModels();
        }
}

void MainWindow::on_SelectionChanged()
{
    // прячем кнопки редактиования
    if (ui->tableView->selectionModel()->selectedRows().isEmpty())
    {
        ui->deleteEmployee->setEnabled(false);
        ui->changeEmployee->setEnabled(false);
     }
    else {
        ui->deleteEmployee->setEnabled(true);
        ui->changeEmployee->setEnabled(true);
    }
}

void MainWindow::on_filterPass_triggered()
{
    // Передаем настройку фильтрации и номер колонки
    filter->setFilter(Month[0],3);
    ui->statusbar->showMessage("Включен фильтр по пропуску! Показаны записи "
                               "с датами в течении следующийх "+QString::number(Month[0])+ " месяцев.", 8000);
}

void MainWindow::on_filterMed_triggered()
{
    // Передаем настройку фильтрации и номер колонки
    filter->setFilter(Month[1],4);
    ui->statusbar->showMessage("Включен фильтр по медосмотру! Показаны записи "
                               "с датами в течении следующийх "+QString::number(Month[1])+ " месяцев.", 8000);
}

void MainWindow::on_filterClouthes_triggered()
{
    // Передаем настройку фильтрации и номер колонки
    filter->setFilter(Month[2],5);
    ui->statusbar->showMessage("Включен фильтр по спецодежде! Показаны записи "
                               "с датами в течении следующийх "+QString::number(Month[2])+ " месяцев.", 8000);
}

void MainWindow::on_filterVacation_triggered()
{
    // Передаем настройку фильтрации и номер колонки
    filter->setFilter(Month[3],6);
    ui->statusbar->showMessage("Включен фильтр по отпуску! Показаны записи "
                               "с датами в течении следующийх "+QString::number(Month[3])+ " месяцев.", 8000);

}

void MainWindow::on_filterReset_triggered()
{
    // сброс фильтра
    filter->resetFilter();
    ui->statusbar->showMessage("Выключен фильтр!", 5000);
}

void MainWindow::on_changeEmployee_triggered()
{
    // Получаем индек ячейки
    QModelIndex idIndex = ui->tableView->selectionModel()->selectedIndexes().first();
    // Если индекс валиден
    if (idIndex.isValid()){
    int id = idIndex.data().toInt();
    slotEditRecord(id);
    }
    else
    {
        return;
    }
}

void MainWindow::on_deleteEmployee_triggered()
{
    // С помощью QMessageBox уточним действительно ли удалять строку,
    // но только установим свои названия кнопок
    QMessageBox messageBox(QMessageBox::Question,
                tr("Удаление записи из таблицы"),
                tr("Вы уверены что хотите удалить запись?"),
                QMessageBox::Yes | QMessageBox::No,
                this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Да"));
        messageBox.setButtonText(QMessageBox::No, tr("Нет"));
        messageBox.exec();
        // Если есть согласие
       if(messageBox.clickedButton()->text() == tr("Да"))
       {

    // Получаем индек ячейки
    QModelIndex idIndex = ui->tableView->selectionModel()->selectedIndexes().first();
    // Если индекс валиден
    if (idIndex.isValid()){
    int id = idIndex.data().toInt();
    // Удаляем строку с нашим id
    if(db->removeRow(id)){
        this->slotUpdateModels();
        ui->statusbar->showMessage("Запись удалена!", 2000);
        }else{
            ui->statusbar->showMessage("Не удалось удалить запись", 2000);
        }
    }
    }
}

void MainWindow::on_quit_triggered()
{
    this->close();
}

void MainWindow::on_reloadBd_triggered()
{

    if(!db->openDataBase(fileName)){
        ui->statusbar->showMessage("Ошибка подключения файла базы данных!", 5000);
    }else {
         ui->statusbar->showMessage("Seccess!", 2000);
         this->setupModel(STAFF_TABLE);
         this->createUI();
    }
}

void MainWindow::on_open_triggered()
{

    // для того что бы узнать имя файла, используем стандартный диалог выбора файла (QFileDialog)
    QString filePath = QFileDialog::getOpenFileName(this, tr("Открыть файл базы данных"),
                                                    fileName,
                                                    tr("Файлы базы данных (*.db);;Все файлы (*.*)"));

    if(filePath.isEmpty())
        return;

    fileName = filePath;

    if(!db->openDataBase(fileName)){
        ui->statusbar->showMessage("Ошибка подключения файла базы данных!", 5000);
    }else {
         ui->statusbar->showMessage("Seccess!", 2000);
         //перед созданием новых новой модели удалим старую
         delete filter;
         delete modelStaff;
         // инициализируем модель представлния
         this->setupModel(STAFF_TABLE);
         // и таблицу отображения данных
         this->createUI();
    }
}

void MainWindow::on_saveAs_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Сохранить файл как"), "/DataBase",
                                    tr("Файл базы данных (*.db);;Файл базы данных (*.*)"));
    if(!filePath.isEmpty()){
        // Если файл существует удалим перед копированием
        if(!QFile::copy(fileName,filePath)){
            QFile::remove(filePath);
            QFile::copy(fileName,filePath);
        }
    }
}

void MainWindow::on_newDatabase_triggered()
{
    // Получаем полное имя файла
    QString filePath = QFileDialog::getSaveFileName(this, tr("Сохранить файл как"), "/",
                                    tr("Файл базы данных (*.db);;Файл базы данных (*.*)"));
    // Если оно не пустое
    if(!filePath.isEmpty()) {
       // Если файл существует удалим
       if(QFile::exists(filePath))
           QFile::remove(filePath);

       fileName = filePath;

    // создаем новый файл БД
    if(!db->openDataBase(fileName)){
        ui->statusbar->showMessage("Ошибка подключения файла базы данных!", 5000);
        }else {
         ui->statusbar->showMessage("Seccess!", 2000);
         // создаем таблицу сотрудников
         db->createStaffTable();
         //перед созданием новых новой модели удалим старую
         delete filter;
         delete modelStaff;
         // инициализируем модель представлния
         this->setupModel(STAFF_TABLE);
         // и таблицу отображения данных
         this->createUI();
        }
    }
}

void MainWindow::on_about_triggered()
{
    QMessageBox::information(this, "О Программе...", "<p><b>Staff (Сотрудники)</b></p> <p>Автор: Духовской Н.Н. mail: mind_87@mail.ru <p>2020г");
}
