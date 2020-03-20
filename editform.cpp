#include "editform.h"
#include "ui_editform.h"

EditForm::EditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditForm)
{
    ui->setupUi(this);
}

EditForm::~EditForm()
{
    delete ui;
}
