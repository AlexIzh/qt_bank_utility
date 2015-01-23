#include "easyform.h"
#include "ui_easyform.h"
#include "QIntValidator"
#include "QDebug"
#include "dialog.h"
#include "QSqlQuery"
#include <QDateTime>
#include "QSqlError"
#include "QDir"
#include "QList"
#include "QCompleter"

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

    QList<QString> lines;
    while (!stream.atEnd()){
        QString line = stream.readLine();
        BIKModel model;
        model.bik = line.right(9);
        model.nameBank = line.left(line.size()-9);
        m_spravList.append(model);
        lines.append(line);
    }

    QFile symbols_file("symbols.txt");
    if (!symbols_file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;


    QTextStream stream__(&symbols_file);
//    stream__.setCodec(codec);

    while (!stream__.atEnd()){
        QString line = stream__.readLine();
        m_symbols.append(line);
    }


    QCompleter *completer = new QCompleter(lines, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    connect(completer, SIGNAL(activated(QString)), SLOT(activateCompleter(QString)));

    ui->bankRecBIKEDit->setCompleter(completer);
    ui->bankTrBIKEdit->setCompleter(completer);
    QValidator *v = new QIntValidator(0, 999999999, this);
    ui->bankRecBIKEDit->setValidator(v);
    ui->bankTrBIKEdit->setValidator(v);
//    qDebug() << "spravochnik" <<  m_spravList << endl;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        for (int j = 0; j < 2; j++){
        QTableWidgetItem *p = new QTableWidgetItem;
        ui->tableWidget->setItem(i,j,p);
    }
    connect(ui->bankTrBIKEdit, SIGNAL(textChanged(QString)), SLOT(checkOccasion(QString)));
    connect(ui->bankRecBIKEDit, SIGNAL(textChanged(QString)), SLOT(checkOccasion(QString)));

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

    db = QSqlDatabase::addDatabase("QSQLITE");
       db.setHostName("localhost");
       db.setUserName("root");
       db.setPassword("sdg2101zmv");
       db.setDatabaseName("__db__");
    if (!db.isValid()){
      qDebug() << db.lastError().text();
      return;
    }
    if (!db.open()) {
        qDebug() << db.lastError().text();
        return;
    }
    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '__db__'";
    QSqlQuery q = db.exec(query);

    if (q.size() == 0){
        db.exec("CREATE DATABASE IF NOT EXISTS __db__");

        qDebug() << db.lastError().text();

    }
//    this->clearTables();
    this->createTables();
}


bool easyForm::clearTables() {
    bool result = true;
    if (db.isOpen()) {
        db.exec("drop table tables");
        db.exec("drop table vals");
        db.exec("drop table summs");
    }
    return result;
}

bool easyForm::createTables()
    {
    // Create table "person"
    bool ret = true;
    if (db.isOpen())
        {
//        QSqlQuery query1;
        this->db.exec("create table tables "
                  "(id integer primary key, "
                  "name varchar(20))");
//        QSqlQuery query2;
        this->db.exec("create table summs "
                  "(id integer primary key, "
                   "save_id integer,"
                  "symbol varchar(2),"
                  "summ varchar(20))");
        this->db.exec("create table vals "
                  "(id integer primary key, "
                  "order_number varchar(20),"
                  "date varchar(20),"
                  "from_ varchar(20),"
                  "acc_number_from varchar(20),"
                  "to_ varchar(20),"
                  "acc_number_to varchar(20),"
                  "inn varchar(20),"
                  "acc_number varchar(20),"
                  "name_bank_from varchar(20),"
                  "BIK_bank_from varchar(20),"
                  "name_bank_to varchar(20),"
                  "BIK_bank_to varchar(20),"
                  "summ_string varchar(20),"
                  "source varchar(20),"
                  "summ_number varchar(20),"
                  "save_id integer)");
        qDebug()<<db.lastError();
        }
    return ret;
    }

void easyForm::loadDB() {
    Dialog *p = new Dialog(NULL);
    p->show();
    QSqlQuery query1;
    query1 = db.exec("select id, name from tables");
    QStringList list;
    while (query1.next()) {
        int id = query1.value(0).toInt();
        QString name = query1.value(1).toString();
        QString string = QString("'%1': '%2'").arg(id).arg(name);
        list << string;
        qDebug() << string << " ";
    }

    p->list()->insertItems(list.size(),list);

    connect(p->box(), &QDialogButtonBox::accepted, [=]() {
        QList<QListWidgetItem*> result = p->list()->selectedItems();
        if (result.size() > 0) {
            int index = p->list()->currentIndex().row();
            int id = index + 1;
            QString string = QString("select * from vals where save_id=%1").arg(id);
            qDebug() << string;
            QSqlQuery query = db.exec(string);
            qDebug() << query.size()<<endl;
            if (query.size()<0) {
                ui->orderNumEdit->setText("0");
                ui->dateEdit->setDate(QDate::fromString("0", NULL));
                ui->fromEdit->setText("0");
                ui->fromNumEdit->setText("0");
                ui->receiverEdit->setText("0");
                ui->receiverNumEdit->setText("0");
                ui->INNEdit->setText("0");
                ui->bankNumEdit->setText("0");
                ui->bankRecEdit->setText("0");
                ui->bankRecBIKEDit->setText("0");
                ui->bankTrEdit->setText("0");
                ui->bankTrBIKEdit->setText("0");
                ui->sumLitEdit->setText("0");
                ui->incomeEdit->setText("0");
                ui->sumDigEdit1->setText("0");
            }
            while (query.next()) {
                int id = query.value(0).toInt();
                ui->orderNumEdit->setText(query.value("order_number").toString());
                ui->dateEdit->setDate(QDate::fromString(query.value("date").toString(), NULL));
                ui->fromEdit->setText(query.value("from_").toString());
                ui->fromNumEdit->setText(query.value("acc_number_from").toString());
                ui->receiverEdit->setText(query.value("to_").toString());
                ui->receiverNumEdit->setText(query.value("acc_number_to").toString());
                ui->INNEdit->setText(query.value("inn").toString());
                ui->bankNumEdit->setText(query.value("acc_number").toString());
                ui->bankRecEdit->setText(query.value("name_bank_from").toString());
                ui->bankRecBIKEDit->setText(query.value("BIK_bank_from").toString());
                ui->bankTrEdit->setText(query.value("name_bank_to").toString());
                ui->bankTrBIKEdit->setText(query.value("BIK_bank_to").toString());
                ui->sumLitEdit->setText(query.value("summ_string").toString());
                ui->incomeEdit->setText(query.value("source").toString());
                ui->sumDigEdit1->setText(query.value("summ_number").toString());
            }
            query = db.exec(QString("select id,symbol, summ  from summs where save_id = %1").arg(id));
            int i = 0;
            while (query.next() || i < 3) {
                ui->tableWidget->item(i,0)->setText(query.value(1).toString());
                ui->tableWidget->item(i,1)->setText(query.value(2).toString());
                i++;
            }
        }
       });
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
    } else if (item->column() == 0) {
        qDebug() << m_symbols;
        bool contains = false;
        foreach (QString str, m_symbols) {
            QString code = str.trimmed().right(2);
            if (item->text() == code) {
                item->setToolTip(str.trimmed().left(str.trimmed().size()-2).trimmed());
                contains = true;
            }
        }
        if (!contains) {
            item->setText("");
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

    QSqlQuery query = db.exec(QString("insert into tables values(NULL,'%1')")
            .arg(QDateTime::currentDateTime().toString()));
    int id_ = query.lastInsertId().toInt();
    QString first_string = QString("insert into vals values(NULL, '%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15',%16)")
            .arg(ui->orderNumEdit->text()).arg(ui->dateEdit->text()).arg(ui->fromEdit->text()).arg(ui->fromNumEdit->text()).arg(ui->receiverEdit->text())
               .arg(ui->receiverNumEdit->text()).arg(ui->INNEdit->text()).arg(ui->bankNumEdit->text())
               .arg(ui->bankTrEdit->text()).arg(ui->bankTrBIKEdit->text()).arg(ui->bankRecEdit->text()).arg(ui->bankRecBIKEDit->text())
               .arg(ui->sumLitEdit->text()).arg(ui->incomeEdit->text()).arg(ui->sumDigEdit1->text()).arg(id_);
    qDebug()<<first_string;
    db.exec(first_string);
    qDebug()<<db.lastError();
    db.exec(QString("insert into summs values(NULL, %1, '%2', '%3')").arg(id_).arg(ui->tableWidget->item(0,0)->text())
               .arg(ui->tableWidget->item(0,1)->text()));
    db.exec(QString("insert into summs values(NULL, %1, '%2', '%3')").arg(id_).arg(ui->tableWidget->item(1,0)->text())
                       .arg(ui->tableWidget->item(1,1)->text()));
    db.exec(QString("insert into summs values(NULL, %1, '%2', '%3')").arg(id_).arg(ui->tableWidget->item(2,0)->text())
                       .arg(ui->tableWidget->item(2,1)->text()));

}

void easyForm::checkOccasion(QString text){
    if (text.length()>9) {
        QString bik = text.right(9);
        if ((QObject::sender() == ui->bankTrBIKEdit))
            ui->bankTrBIKEdit->setText(bik);
        else
            ui->bankRecBIKEDit->setText(bik);
    }
    if (text.length() == 9)
    foreach(BIKModel model, m_spravList){
        QString str = model.bik;
        if (str.contains(text)){
            str.remove(text);
             if ((QObject::sender() == ui->bankTrBIKEdit))
                 ui->bankTrEdit->setText(model.nameBank.trimmed());
                else if (QObject::sender() == ui->bankRecBIKEDit)
                 ui->bankRecEdit->setText(model.nameBank.trimmed());
        }
    }
}

void easyForm::activateCompleter(QString text) {
    QString bik = text.right(9);
    QString nameBank = text.left(text.size()-9);
    if (((QCompleter*)QObject::sender())->widget() == ui->bankRecBIKEDit) {
        ui->bankRecBIKEDit->setText(bik);
        ui->bankRecEdit->setText(nameBank.trimmed());
    } else {
        ui->bankTrBIKEdit->setText(bik);
        ui->bankTrEdit->setText(nameBank.trimmed());
    }
//((QCompleter*)QObject::sender())->setCurrentRow(-1);
}

easyForm::~easyForm()
{
    db.close();
    delete ui;
}
