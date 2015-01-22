#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    this->db = db;
}

QListWidget *Dialog::list() {
    return ui->listWidget;
}

QDialogButtonBox *Dialog::box() {
    return ui->buttonBox;
}

Dialog::~Dialog()
{
    delete ui;
}
