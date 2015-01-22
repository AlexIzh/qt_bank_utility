/********************************************************************************
** Form generated from reading UI file 'pixmapform.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PIXMAPFORM_H
#define UI_PIXMAPFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PixMapForm
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QPushButton *printButton;

    void setupUi(QWidget *PixMapForm)
    {
        if (PixMapForm->objectName().isEmpty())
            PixMapForm->setObjectName(QStringLiteral("PixMapForm"));
        PixMapForm->resize(1078, 719);
        gridLayout = new QGridLayout(PixMapForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        scrollArea = new QScrollArea(PixMapForm);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1052, 693));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 1024, 800));
        printButton = new QPushButton(scrollAreaWidgetContents);
        printButton->setObjectName(QStringLiteral("printButton"));
        printButton->setGeometry(QRect(500, 450, 123, 32));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 2, 0, 1, 1);


        retranslateUi(PixMapForm);

        QMetaObject::connectSlotsByName(PixMapForm);
    } // setupUi

    void retranslateUi(QWidget *PixMapForm)
    {
        PixMapForm->setWindowTitle(QApplication::translate("PixMapForm", "Form", 0));
        label->setText(QApplication::translate("PixMapForm", "imageLabel", 0));
        printButton->setText(QApplication::translate("PixMapForm", "\320\240\320\260\321\201\320\277\320\265\321\207\320\260\321\202\320\260\321\202\321\214", 0));
    } // retranslateUi

};

namespace Ui {
    class PixMapForm: public Ui_PixMapForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIXMAPFORM_H
