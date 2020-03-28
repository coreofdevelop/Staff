#include "setform.h"
#include "ui_setform.h"

SetForm::SetForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetForm)
{
    ui->setupUi(this);

    QStringList listItems = QStringList() << "0" << "1" << "2" << "3" << "4" << "5" << "6";
    ui->comboBoxPass->addItems(listItems);
    ui->comboBoxMed->addItems(listItems);
    ui->comboBoxClothes->addItems(listItems);
    ui->comboBoxVacation->addItems(listItems);
}

SetForm::~SetForm()
{
    delete ui;
}

void SetForm::setSetting(QList<bool> column, QList<int> filter)
{
    ui->checkBoxPosition->setChecked(column[0]);
        ui->checkBoxMed->setChecked(column[1]);
        ui->checkBoxClothes->setChecked(column[2]);
        ui->checkBoxVacation->setChecked(column[3]);
        ui->checkBoxPhone->setChecked(column[4]);
        ui->checkBoxBirth->setChecked(column[5]);

        ui->comboBoxPass->setCurrentIndex(filter[0]);
        ui->comboBoxMed->setCurrentIndex(filter[1]);
        ui->comboBoxClothes->setCurrentIndex(filter[2]);
        ui->comboBoxVacation->setCurrentIndex(filter[3]);
}

QList<bool> SetForm::columnSetting()
{
    return setColumn;
}

QList<int> SetForm::monthSetting()
{
    return setMonth;
}

void SetForm::on_buttonBox_accepted()
{
    setColumn[0] = ui->checkBoxPosition->isChecked();
        setColumn[1] = ui->checkBoxMed->isChecked();
        setColumn[2] = ui->checkBoxClothes->isChecked();
        setColumn[3] = ui->checkBoxVacation->isChecked();
        setColumn[4] = ui->checkBoxPhone->isChecked();
        setColumn[5] = ui->checkBoxBirth->isChecked();

        setMonth[0] = ui->comboBoxPass->currentText().toInt();
        setMonth[1] = ui->comboBoxMed->currentText().toInt();
        setMonth[2] = ui->comboBoxClothes->currentText().toInt();
        setMonth[3] = ui->comboBoxVacation->currentText().toInt();
}
