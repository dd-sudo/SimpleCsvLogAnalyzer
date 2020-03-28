/********************************************************************************
** Form generated from reading UI file 'DataMath.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAMATH_H
#define UI_DATAMATH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DataMath
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QListWidget *dataLabels;
    QListWidget *operatorList;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *dataMath;
    QPushButton *clear;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DataMath)
    {
        if (DataMath->objectName().isEmpty())
            DataMath->setObjectName(QString::fromUtf8("DataMath"));
        DataMath->resize(531, 284);
        verticalLayout = new QVBoxLayout(DataMath);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        dataLabels = new QListWidget(DataMath);
        dataLabels->setObjectName(QString::fromUtf8("dataLabels"));
        dataLabels->setDragEnabled(true);
        dataLabels->setDragDropMode(QAbstractItemView::DragOnly);

        horizontalLayout->addWidget(dataLabels);

        operatorList = new QListWidget(DataMath);
        operatorList->setObjectName(QString::fromUtf8("operatorList"));
        operatorList->setDragEnabled(true);
        operatorList->setDragDropMode(QAbstractItemView::DragOnly);

        horizontalLayout->addWidget(operatorList);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(DataMath);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        dataMath = new QLineEdit(DataMath);
        dataMath->setObjectName(QString::fromUtf8("dataMath"));
        dataMath->setAcceptDrops(false);
        dataMath->setReadOnly(true);

        horizontalLayout_2->addWidget(dataMath);

        clear = new QPushButton(DataMath);
        clear->setObjectName(QString::fromUtf8("clear"));

        horizontalLayout_2->addWidget(clear);


        verticalLayout->addLayout(horizontalLayout_2);

        buttonBox = new QDialogButtonBox(DataMath);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DataMath);
        QObject::connect(buttonBox, SIGNAL(accepted()), DataMath, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DataMath, SLOT(reject()));

        QMetaObject::connectSlotsByName(DataMath);
    } // setupUi

    void retranslateUi(QDialog *DataMath)
    {
        DataMath->setWindowTitle(QApplication::translate("DataMath", "Create new derived data (Data Math)", nullptr));
        label->setText(QApplication::translate("DataMath", "Data Math:", nullptr));
#ifndef QT_NO_TOOLTIP
        dataMath->setToolTip(QApplication::translate("DataMath", "A derived data element is a data element derived from other data elements using a mathematical, logical, or other type of transformation, e.g. arithmetic formula, composition, aggregation.", nullptr));
#endif // QT_NO_TOOLTIP
        dataMath->setInputMask(QString());
        clear->setText(QApplication::translate("DataMath", "Undo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DataMath: public Ui_DataMath {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAMATH_H
