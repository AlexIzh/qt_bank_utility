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
private slots:
    void draw();
    void loadDB();
    void onItemChanged(QTableWidgetItem* item);
    void validate(QString text);
    bool createTables();
    bool clearTables();
};

#endif // EASYFORM_H
