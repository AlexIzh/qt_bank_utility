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

Dialog::~Dialog()
{
    delete ui;
}
