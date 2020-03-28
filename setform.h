#ifndef SETFORM_H
#define SETFORM_H

#include <QDialog>

namespace Ui {
class SetForm;
}

class SetForm : public QDialog
{
    Q_OBJECT

public:
    explicit SetForm(QWidget *parent = nullptr);
    ~SetForm();

    void setSetting(QList<bool> column, QList<int> filter);
    QList<bool> columnSetting();
    QList<int> monthSetting();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SetForm *ui;

private:
    QList<bool> setColumn{1,1,1,1,1,1,1};
    QList<int> setMonth{3,2,0,0};
};

#endif // SETFORM_H
