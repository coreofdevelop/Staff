#ifndef EDITFORM_H
#define EDITFORM_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QPixmap>

#include <database.h>

namespace Ui {
class EditForm;
}

class EditForm : public QDialog
{
    Q_OBJECT

public:
    explicit EditForm(int row = -1, QWidget *parent = nullptr);
    ~EditForm();

signals:
    void signalReady();

private slots:
    void on_ButtonOk_clicked();

    void on_ButtonCancel_clicked();

private:
    void setupModel();

private:
    Ui::EditForm                *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;
};

#endif // EDITFORM_H
