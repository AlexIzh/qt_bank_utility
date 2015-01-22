#include "easyform.h"
#include "ui_easyform.h"
#include "QIntValidator"
#include "QDebug"
#include "dialog.h"
#include "QSqlQuery"
#include <QDateTime>
#include <QFile>
#include <QTextCodec>
easyForm::easyForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::easyForm)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(3);




    ui->sumDigEdit1->setDisabled(true);
    QFile file("sprav.txt");


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;


    QTextStream stream(&file);
    QTextCodec *codec = QTextCodec::codecForName("cp1251");
    stream.setCodec(codec);
    while (!stream.atEnd()){
        m_spravList.append(stream.readLine());
    }

    qDebug() << "spravochnik" << m_spravList << endl;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        for (int j = 0; j < 2; j++){
        QTableWidgetItem *p = new QTableWidgetItem;
        ui->tableWidget->setItem(i,j,p);
    }
    connect(ui->bankTrBIKEdit, SIGNAL(textEdited(QString)), SLOT(checkOccasion(QString)));
    connect(ui->bankRecBIKEDit, SIGNAL(textEdited(QString)), SLOT(checkOccasion(QString)));

    connect(ui->pushButton, SIGNAL(clicked()), SLOT(draw()));

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), SLOT(onItemChanged(QTableWidgetItem*)));

    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(loadDB()));

    validator = new QIntValidator(202028100, 202028109, ui->fromNumEdit);

    ui->fromNumEdit->setText("20202810");
    connect(ui->fromNumEdit, SIGNAL(textChanged(QString)), SLOT(validate(QString)));
    ui->fromNumEdit->setValidator(validator);
    ui->orderNumEdit->setFocus();


    //auto fill
    ui->fromNumEdit->setText("202028101");

    db = QSqlDatabase::addDatabase("QMYSQL", "mydb");
    db.setHostName("bigblue");
    db.setDatabaseName("flightdb");
    db.setUserName("acarlson");
    db.setPassword("1uTbSbAs");
    if (db.isValid()) {
       db.open();
       this->createTables();
    }
}

bool easyForm::createTables()
    {
    // Create table "person"
    bool ret = false;
    if (db.isOpen())
        {
        QSqlQuery query1;
        ret = query1.exec("create table tables "
                  "(id integer primary key, "
                  "name varchar(20))");
        QSqlQuery query2;
        ret = query2.exec("create table summs "
                  "(id integer primary key, "
                  "symbol varchar(2),"
                  "save_id integer,"
                  "summ varchar(20))");
        QSqlQuery query3;
        ret = query3.exec("create table values "
                  "(id integer primary key, "
                  "order_number varchar(20),"
                  "date varchar(20),"
                  "from varchar(20),"
                  "acc_number_from varchar(20),"
                  "to varchar(20),"
                  "acc_number_to varchar(20),"
                  "inn varchar(20),"
                  "acc_number varchar(20),"
                  "name_bank_from varchar(20),"
                  "BINK_bank_from varchar(20),"
                  "name_bank_to varchar(20),"
                  "BINK_bank_to varchar(20),"
                  "summ_string varchar(20),"
                  "source varchar(20),"
                  "summ_number varchar(20),"
                  "save_id integer)");
        }
    return ret;
    }

void easyForm::loadDB() {
    Dialog *p = new Dialog(NULL);
    p->show();
}


void easyForm::validate(QString text){
    int pos = 0;
    if ((validator->validate(text,pos)) == QValidator::Acceptable){
        ui->fromNumEdit->setText(text);
    }
    else{
        ui->fromNumEdit->setText("20202810");
    }
}

void easyForm::onItemChanged(QTableWidgetItem *item){
    int sum = 0;

    bool okGovnoKod = true;
    item->text().toInt(&okGovnoKod);
    if (!okGovnoKod)
        item->setText("");

    if (item->column() == 1){
        for (int i = 0; i < 3; i++){
            bool ok = true;
            int part = ui->tableWidget->item(i,1)->text().toInt(&ok);
            sum += (ok ? part: 0);
            ui->sumDigEdit1->setText(QString::number(sum));
        }
    }

}

void easyForm::draw(){

    if (ui->orderNumEdit->text().isEmpty()){
        ui->orderNumEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле Номер Ордера не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->dateEdit->text().isEmpty()){
        ui->dateEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле Дата не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->fromEdit->text().isEmpty()){
        ui->fromEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("От кого не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->fromNumEdit->text().isEmpty() || (ui->fromNumEdit->text() == "20202810")){
        ui->fromNumEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Счет Отправителя не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->receiverEdit->text().isEmpty()){
        ui->receiverEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле Получатель не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->receiverNumEdit->text().isEmpty()){
        ui->receiverEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле счет получателя не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->INNEdit->text().isEmpty()){
        ui->INNEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле ИНН не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->bankNumEdit->text().isEmpty()){
        ui->bankNumEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле Дата не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->bankTrEdit->text().isEmpty()){
        ui->bankTrEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле банк-отправитель не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->bankTrBIKEdit->text().isEmpty()){
        ui->bankTrBIKEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле БИК банка-отправителя не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->bankRecEdit->text().isEmpty()){
        ui->bankRecEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле Банк-Получатель не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->bankRecBIKEDit->text().isEmpty()){
        ui->bankRecBIKEDit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле БИК Банка-Получателя не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->sumLitEdit->text().isEmpty()){
        ui->sumLitEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле Сумма прописью не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->incomeEdit->text().isEmpty()){
        ui->incomeEdit->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле Источник поступления не заполнено.");
        msgBox.exec();
        return;
    }
    if (ui->sumDigEdit1->text().isEmpty()){
        ui->sumDigEdit1->setFocus();
        QMessageBox msgBox;
        msgBox.setText("Поле сумма1 не заполнено.");
        msgBox.exec();
        return;
    }

    QImage image("1.jpg");
    QPainter painter;

    painter.begin(&image);
    painter.drawImage(0,0,image);
    QFont f;
    f.setPixelSize(18);
    painter.setFont(f);
    painter.drawText(380,180,ui->orderNumEdit->text());
    painter.drawText(500,180,ui->dateEdit->text());
    painter.drawText(200,300,ui->fromEdit->text());
    painter.drawText(480,300,ui->fromNumEdit->text());
    painter.drawText(200,390,ui->receiverEdit->text());
    painter.drawText(480,390,ui->receiverNumEdit->text());
    painter.drawText(100,455,ui->INNEdit->text());
    painter.drawText(480,455,ui->bankNumEdit->text());
    painter.drawText(400,500,ui->bankTrEdit->text());
    painter.drawText(380,550,ui->bankTrBIKEdit->text());
    painter.drawText(400,595,ui->bankRecEdit->text());
    painter.drawText(380,640,ui->bankRecBIKEDit->text());
    painter.drawText(220,690,ui->sumLitEdit->text());
    painter.drawText(280,735,ui->incomeEdit->text());
    painter.drawText(700,300,ui->sumDigEdit1->text());


    painter.drawText(700,550,ui->tableWidget->item(0,0)->text());
    painter.drawText(700,595,ui->tableWidget->item(1,0)->text());
    painter.drawText(700,640,ui->tableWidget->item(2,0)->text());

    painter.drawText(800,550,ui->tableWidget->item(0,1)->text());
    painter.drawText(800,595,ui->tableWidget->item(1,1)->text());
    painter.drawText(800,640,ui->tableWidget->item(2,1)->text());


    painter.end();
    m_image = image;
    image.save("ololosha.jpg",0,100);
    PixMapForm *p = new PixMapForm(NULL, m_image );
    p->show();

    QSqlQuery query;
//    QString date = Qt.formatDateTime(new Date(), "ddMMyy hh:mm:ss");
//    ret = query.exec(QString("select name, id from tables where name='%1'")
//            .arg(date));
//
    int id_ = -1;
//    while (query.next()) {
//         id_ = query.value(1).toInt();
//       }
    if (id_ == -1) {
        query.exec(QString("insert into tables values(NULL,'%1')")
            .arg(QDateTime::currentDateTime().toString()));
         id_ = query.lastInsertId().toInt();
    }
    query.exec(QString("insert into values (NULL, '%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16')")
            .arg(ui->orderNumEdit->text()).arg(ui->dateEdit->text()).arg(ui->fromEdit->text()).arg(ui->fromNumEdit->text()).arg(ui->receiverEdit->text())
               .arg(ui->receiverNumEdit->text()).arg(ui->INNEdit->text()).arg(ui->bankNumEdit->text())
               .arg(ui->bankTrEdit->text()).arg(ui->bankTrBIKEdit->text()).arg(ui->bankRecEdit->text()).arg(ui->bankRecBIKEDit->text())
               .arg(ui->sumLitEdit->text()).arg(ui->incomeEdit->text()).arg(ui->sumDigEdit1->text()).arg(id_));
    query.exec(QString("insert into summs (NULL, %1, %2, %3)").arg(ui->tableWidget->item(0,0)->text())
               .arg(id_).arg(ui->tableWidget->item(0,1)->text()));
    query.exec(QString("insert into summs (NULL, %1, %2, %3)").arg(ui->tableWidget->item(1,0)->text())
                       .arg(id_).arg(ui->tableWidget->item(1,1)->text()));
    query.exec(QString("insert into summs (NULL, %1, %2, %3)").arg(ui->tableWidget->item(2,0)->text())
                       .arg(id_).arg(ui->tableWidget->item(2,1)->text()));

}

void easyForm::checkOccasion(QString text){
    if (text.length() == 9)
    foreach(QString str, m_spravList){
        if (str.contains(text)){
            str.remove(text);
             if ((QObject::sender() == ui->bankTrBIKEdit))
                 ui->bankTrEdit->setText(str);
                else if (QObject::sender() == ui->bankRecBIKEDit)
                 ui->bankRecEdit->setText(str);
        }
    }
}


easyForm::~easyForm()
{
    db.close();
    delete ui;
}
