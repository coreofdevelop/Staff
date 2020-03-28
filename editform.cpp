#include "editform.h"
#include "ui_editform.h"

EditForm::EditForm(int row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditForm)
{
    ui->setupUi(this);

    /* Настроим отображение изображений
         * */
        QPixmap photoPix(":/resource/img/user.png");
        int photoWidth = ui->imgPhoto->width();
        int photoHeight = ui->imgPhoto->height();
        ui->imgPhoto->setPixmap(photoPix.scaled(photoWidth, photoHeight, Qt::KeepAspectRatio));

        QPixmap calenPix(":/resource/img/calendar.png");
        int calenWidth = ui->imgCalendar->width();
        int calenHeight = ui->imgCalendar->height();
        ui->imgCalendar->setPixmap(calenPix.scaled(calenWidth, calenHeight, Qt::KeepAspectRatio));



        /* Метода для инициализации модели,
        * из которой будут транслироваться данные
        * */
        setupModel();

        /* Если строка не задана, то есть равна -1,
        * тогда диалог работает по принципу создания новой записи.
        * А именно, в модель вставляется новая строка и работа ведётся с ней.
        * */
        if(row == -1){
            model->insertRow(model->rowCount(QModelIndex()));
            mapper->toLast();
            /* В противном случае диалог настраивается на заданную запись
             * */
            } else {

                model->setFilter("id='"+QString::number(row)+"'");
                mapper->setCurrentModelIndex(model->index(0,0));

            }

}

EditForm::~EditForm()
{
    delete ui;
}

void EditForm::setupModel()
{
    /* Инициализируем модель и делаем выборку из неё
         * */
        model = new QSqlTableModel(this);
        model->setTable(STAFF_TABLE);
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();

        /* Инициализируем mapper и привязываем
         * поля данных к объектам LineEdit и DataEdit
         * */
        mapper = new QDataWidgetMapper();
        mapper->setModel(model);
        mapper->addMapping(ui->lineEditName , 1);
        mapper->addMapping(ui->lineEditPosition, 2);
        mapper->addMapping(ui->dateEditPass, 3);
        mapper->addMapping(ui->dateEditMed, 4);
        mapper->addMapping(ui->dateEditClothes, 5);
        mapper->addMapping(ui->dateEditVacation, 6);
        mapper->addMapping(ui->lineEditPhone, 7);
        mapper->addMapping(ui->dateEditBirth, 8);
        /* Ручное подтверждение изменения данных через mapper
         * */
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);


}

void EditForm::on_ButtonOk_clicked()
{
    mapper->submit();
    model->submitAll();
    emit signalReady();
    this->close();
}

void EditForm::on_ButtonCancel_clicked()
{
    this->close();
}
