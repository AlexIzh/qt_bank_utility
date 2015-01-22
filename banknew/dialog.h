#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "QSqlDatabase"
#include "QListWidget"
#include "QDialogButtonBox"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QListWidget *list();//readonly
    QDialogButtonBox *box();//readonly
private:
    Ui::Dialog *ui;
    QSqlDatabase db;
};

#endif // DIALOG_H
