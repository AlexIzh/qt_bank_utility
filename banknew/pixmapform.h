#ifndef PIXMAPFORM_H
#define PIXMAPFORM_H

#include <QWidget>

namespace Ui {
class PixMapForm;
}

class PixMapForm : public QWidget
{
    Q_OBJECT

public:
    explicit PixMapForm(QWidget *parent = 0, QImage image = QImage(0,0,QImage::Format_RGB32));
    ~PixMapForm();

private:
    Ui::PixMapForm *ui;

public slots:
    void print();
};

#endif // PIXMAPFORM_H
