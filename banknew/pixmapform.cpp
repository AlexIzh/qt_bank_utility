#include "pixmapform.h"
#include "ui_pixmapform.h"
#include "easyform.h"

PixMapForm::PixMapForm(QWidget *parent, QImage image) :
    QWidget(parent),
    ui(new Ui::PixMapForm)
{
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap::fromImage(image));
    connect (ui->printButton, SIGNAL(clicked()), SLOT(print()));
}

PixMapForm::~PixMapForm()
{
    delete ui;
}


void PixMapForm::print(){
    QPrinter printer;
    printer.setOutputFileName("ololosha.jpg");
    QPrintDialog *dlg = new QPrintDialog(&printer, NULL);
    dlg->exec();
}
