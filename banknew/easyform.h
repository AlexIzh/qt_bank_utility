#ifndef EASYFORM_H
#define EASYFORM_H

#include <QWidget>
#include <QPainter>
#include <QMessageBox>
#include <pixmapform.h>
#include <QPrinter>
#include <QPrintDialog>
#include <QTableWidget>
#include "QSqlDatabase"
namespace Ui {
class easyForm;
}

class BIKModel {
public:
    QString bik;
    QString nameBank;
};

class easyForm : public QWidget
{
    Q_OBJECT

public:
    explicit easyForm(QWidget *parent = 0);
    ~easyForm();
    QImage m_image;

private:
    Ui::easyForm *ui;
    QSqlDatabase db;
    QIntValidator *validator;
    QList<BIKModel> m_spravList;
    QList<QString> m_symbols;
private slots:
    void draw();
    void loadDB();
    void onItemChanged(QTableWidgetItem* item);
    void validate(QString text);
    bool createTables();
    bool clearTables();
    void checkOccasion(QString text);
    void activateCompleter(QString text);
};

#endif // EASYFORM_H
