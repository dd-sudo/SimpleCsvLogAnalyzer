/********************************************************************************
** Form generated from reading UI file 'SimpleCsvLogAnalyzer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLECSVLOGANALYZER_H
#define UI_SIMPLECSVLOGANALYZER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "src/QCustomPlot/qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_SimpleCsvLogAnalyzer
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionZoom_Reset;
    QAction *actionSave_Plot_Image;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *mainHorizontalLayout;
    QTabWidget *leftTab;
    QWidget *verticalTabWidgetPage1;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QLabel *label_2;
    QListWidget *dataListY;
    QFrame *line_11;
    QLabel *label_3;
    QListWidget *dataListX;
    QPushButton *plotSelected;
    QPushButton *plotSelectedXY;
    QTabWidget *rightTabs;
    QWidget *plotTab;
    QVBoxLayout *verticalLayout_3;
    QCustomPlot *plot1;
    QVBoxLayout *verticalLayout_4;
    QWidget *dataTableViewTab;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *dataTableWidget;
    QHBoxLayout *dataStatisticsLayout;
    QFrame *line_8;
    QLabel *minValueX;
    QFrame *line_2;
    QLabel *maxValueX;
    QFrame *line_3;
    QLabel *averageValueX;
    QFrame *line_4;
    QLabel *spanValueX;
    QFrame *line;
    QLabel *minValueY;
    QFrame *line_5;
    QLabel *maxValueY;
    QFrame *line_6;
    QLabel *averageValueY;
    QFrame *line_7;
    QLabel *spanValueY;
    QFrame *line_9;
    QLabel *totalDataLength;
    QFrame *line_10;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuPlot;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SimpleCsvLogAnalyzer)
    {
        if (SimpleCsvLogAnalyzer->objectName().isEmpty())
            SimpleCsvLogAnalyzer->setObjectName(QString::fromUtf8("SimpleCsvLogAnalyzer"));
        SimpleCsvLogAnalyzer->resize(774, 657);
        actionOpen = new QAction(SimpleCsvLogAnalyzer);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionExit = new QAction(SimpleCsvLogAnalyzer);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionZoom_Reset = new QAction(SimpleCsvLogAnalyzer);
        actionZoom_Reset->setObjectName(QString::fromUtf8("actionZoom_Reset"));
        actionSave_Plot_Image = new QAction(SimpleCsvLogAnalyzer);
        actionSave_Plot_Image->setObjectName(QString::fromUtf8("actionSave_Plot_Image"));
        centralwidget = new QWidget(SimpleCsvLogAnalyzer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        mainHorizontalLayout = new QHBoxLayout();
        mainHorizontalLayout->setObjectName(QString::fromUtf8("mainHorizontalLayout"));
        leftTab = new QTabWidget(centralwidget);
        leftTab->setObjectName(QString::fromUtf8("leftTab"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftTab->sizePolicy().hasHeightForWidth());
        leftTab->setSizePolicy(sizePolicy);
        leftTab->setMaximumSize(QSize(250, 16777215));
        verticalTabWidgetPage1 = new QWidget();
        verticalTabWidgetPage1->setObjectName(QString::fromUtf8("verticalTabWidgetPage1"));
        verticalLayout = new QVBoxLayout(verticalTabWidgetPage1);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        pushButton = new QPushButton(verticalTabWidgetPage1);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMaximumSize(QSize(16777215, 32));

        verticalLayout->addWidget(pushButton);

        label_2 = new QLabel(verticalTabWidgetPage1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMaximumSize(QSize(16777215, 15));

        verticalLayout->addWidget(label_2);

        dataListY = new QListWidget(verticalTabWidgetPage1);
        dataListY->setObjectName(QString::fromUtf8("dataListY"));
        sizePolicy.setHeightForWidth(dataListY->sizePolicy().hasHeightForWidth());
        dataListY->setSizePolicy(sizePolicy);
        dataListY->setContextMenuPolicy(Qt::CustomContextMenu);
        dataListY->setDragEnabled(true);
        dataListY->setDragDropOverwriteMode(true);
        dataListY->setDragDropMode(QAbstractItemView::DropOnly);
        dataListY->setDefaultDropAction(Qt::CopyAction);

        verticalLayout->addWidget(dataListY);

        line_11 = new QFrame(verticalTabWidgetPage1);
        line_11->setObjectName(QString::fromUtf8("line_11"));
        line_11->setFrameShape(QFrame::HLine);
        line_11->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_11);

        label_3 = new QLabel(verticalTabWidgetPage1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMaximumSize(QSize(16777215, 15));

        verticalLayout->addWidget(label_3);

        dataListX = new QListWidget(verticalTabWidgetPage1);
        dataListX->setObjectName(QString::fromUtf8("dataListX"));
        sizePolicy.setHeightForWidth(dataListX->sizePolicy().hasHeightForWidth());
        dataListX->setSizePolicy(sizePolicy);
        dataListX->setDragDropMode(QAbstractItemView::DragOnly);
        dataListX->setSelectionMode(QAbstractItemView::SingleSelection);

        verticalLayout->addWidget(dataListX);

        plotSelected = new QPushButton(verticalTabWidgetPage1);
        plotSelected->setObjectName(QString::fromUtf8("plotSelected"));
        plotSelected->setMaximumSize(QSize(300, 32));

        verticalLayout->addWidget(plotSelected);

        plotSelectedXY = new QPushButton(verticalTabWidgetPage1);
        plotSelectedXY->setObjectName(QString::fromUtf8("plotSelectedXY"));
        plotSelectedXY->setMaximumSize(QSize(16777215, 32));

        verticalLayout->addWidget(plotSelectedXY);

        leftTab->addTab(verticalTabWidgetPage1, QString());

        mainHorizontalLayout->addWidget(leftTab);

        rightTabs = new QTabWidget(centralwidget);
        rightTabs->setObjectName(QString::fromUtf8("rightTabs"));
        plotTab = new QWidget();
        plotTab->setObjectName(QString::fromUtf8("plotTab"));
        verticalLayout_3 = new QVBoxLayout(plotTab);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        plot1 = new QCustomPlot(plotTab);
        plot1->setObjectName(QString::fromUtf8("plot1"));
        sizePolicy.setHeightForWidth(plot1->sizePolicy().hasHeightForWidth());
        plot1->setSizePolicy(sizePolicy);
        verticalLayout_4 = new QVBoxLayout(plot1);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        verticalLayout_3->addWidget(plot1);

        rightTabs->addTab(plotTab, QString());
        dataTableViewTab = new QWidget();
        dataTableViewTab->setObjectName(QString::fromUtf8("dataTableViewTab"));
        verticalLayout_5 = new QVBoxLayout(dataTableViewTab);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        dataTableWidget = new QTableWidget(dataTableViewTab);
        dataTableWidget->setObjectName(QString::fromUtf8("dataTableWidget"));
        dataTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        dataTableWidget->setFrameShape(QFrame::StyledPanel);
        dataTableWidget->setFrameShadow(QFrame::Sunken);
        dataTableWidget->setLineWidth(1);
        dataTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dataTableWidget->setAlternatingRowColors(true);
        dataTableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        dataTableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);
        dataTableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        dataTableWidget->setCornerButtonEnabled(false);

        verticalLayout_5->addWidget(dataTableWidget);

        rightTabs->addTab(dataTableViewTab, QString());

        mainHorizontalLayout->addWidget(rightTabs);


        verticalLayout_2->addLayout(mainHorizontalLayout);

        dataStatisticsLayout = new QHBoxLayout();
        dataStatisticsLayout->setSpacing(5);
        dataStatisticsLayout->setObjectName(QString::fromUtf8("dataStatisticsLayout"));
        dataStatisticsLayout->setContentsMargins(0, 0, 0, -1);
        line_8 = new QFrame(centralwidget);
        line_8->setObjectName(QString::fromUtf8("line_8"));
        line_8->setFrameShape(QFrame::VLine);
        line_8->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_8);

        minValueX = new QLabel(centralwidget);
        minValueX->setObjectName(QString::fromUtf8("minValueX"));
        minValueX->setAlignment(Qt::AlignCenter);

        dataStatisticsLayout->addWidget(minValueX);

        line_2 = new QFrame(centralwidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_2);

        maxValueX = new QLabel(centralwidget);
        maxValueX->setObjectName(QString::fromUtf8("maxValueX"));
        maxValueX->setAlignment(Qt::AlignCenter);

        dataStatisticsLayout->addWidget(maxValueX);

        line_3 = new QFrame(centralwidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_3);

        averageValueX = new QLabel(centralwidget);
        averageValueX->setObjectName(QString::fromUtf8("averageValueX"));
        averageValueX->setAlignment(Qt::AlignCenter);

        dataStatisticsLayout->addWidget(averageValueX);

        line_4 = new QFrame(centralwidget);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_4);

        spanValueX = new QLabel(centralwidget);
        spanValueX->setObjectName(QString::fromUtf8("spanValueX"));
        spanValueX->setAlignment(Qt::AlignCenter);

        dataStatisticsLayout->addWidget(spanValueX);

        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line);

        minValueY = new QLabel(centralwidget);
        minValueY->setObjectName(QString::fromUtf8("minValueY"));
        minValueY->setAlignment(Qt::AlignCenter);

        dataStatisticsLayout->addWidget(minValueY);

        line_5 = new QFrame(centralwidget);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_5);

        maxValueY = new QLabel(centralwidget);
        maxValueY->setObjectName(QString::fromUtf8("maxValueY"));
        maxValueY->setAlignment(Qt::AlignCenter);

        dataStatisticsLayout->addWidget(maxValueY);

        line_6 = new QFrame(centralwidget);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_6);

        averageValueY = new QLabel(centralwidget);
        averageValueY->setObjectName(QString::fromUtf8("averageValueY"));
        averageValueY->setAlignment(Qt::AlignCenter);

        dataStatisticsLayout->addWidget(averageValueY);

        line_7 = new QFrame(centralwidget);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        line_7->setFrameShape(QFrame::VLine);
        line_7->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_7);

        spanValueY = new QLabel(centralwidget);
        spanValueY->setObjectName(QString::fromUtf8("spanValueY"));
        spanValueY->setAlignment(Qt::AlignCenter);

        dataStatisticsLayout->addWidget(spanValueY);

        line_9 = new QFrame(centralwidget);
        line_9->setObjectName(QString::fromUtf8("line_9"));
        line_9->setFrameShape(QFrame::VLine);
        line_9->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_9);

        totalDataLength = new QLabel(centralwidget);
        totalDataLength->setObjectName(QString::fromUtf8("totalDataLength"));

        dataStatisticsLayout->addWidget(totalDataLength);

        line_10 = new QFrame(centralwidget);
        line_10->setObjectName(QString::fromUtf8("line_10"));
        line_10->setFrameShape(QFrame::VLine);
        line_10->setFrameShadow(QFrame::Sunken);

        dataStatisticsLayout->addWidget(line_10);


        verticalLayout_2->addLayout(dataStatisticsLayout);

        SimpleCsvLogAnalyzer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SimpleCsvLogAnalyzer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 774, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuPlot = new QMenu(menubar);
        menuPlot->setObjectName(QString::fromUtf8("menuPlot"));
        SimpleCsvLogAnalyzer->setMenuBar(menubar);
        statusbar = new QStatusBar(SimpleCsvLogAnalyzer);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        SimpleCsvLogAnalyzer->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuPlot->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuPlot->addAction(actionZoom_Reset);
        menuPlot->addSeparator();
        menuPlot->addAction(actionSave_Plot_Image);

        retranslateUi(SimpleCsvLogAnalyzer);

        leftTab->setCurrentIndex(0);
        rightTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SimpleCsvLogAnalyzer);
    } // setupUi

    void retranslateUi(QMainWindow *SimpleCsvLogAnalyzer)
    {
        SimpleCsvLogAnalyzer->setWindowTitle(QApplication::translate("SimpleCsvLogAnalyzer", "SimpleCsvLogAnalyzer", nullptr));
        actionOpen->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Open", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("SimpleCsvLogAnalyzer", "O", nullptr));
#endif // QT_NO_SHORTCUT
        actionExit->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Exit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionExit->setShortcut(QApplication::translate("SimpleCsvLogAnalyzer", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionZoom_Reset->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Zoom Reset", nullptr));
#ifndef QT_NO_SHORTCUT
        actionZoom_Reset->setShortcut(QApplication::translate("SimpleCsvLogAnalyzer", "0", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave_Plot_Image->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Save Plot Image", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave_Plot_Image->setShortcut(QApplication::translate("SimpleCsvLogAnalyzer", "S", nullptr));
#endif // QT_NO_SHORTCUT
        pushButton->setText(QApplication::translate("SimpleCsvLogAnalyzer", "PushButton", nullptr));
        label_2->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Y Axis Data List", nullptr));
        label_3->setText(QApplication::translate("SimpleCsvLogAnalyzer", "X Axis Data List", nullptr));
        plotSelected->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Plot Selected Data vs Index", nullptr));
        plotSelectedXY->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Plot Selected X-Y Data", nullptr));
        leftTab->setTabText(leftTab->indexOf(verticalTabWidgetPage1), QApplication::translate("SimpleCsvLogAnalyzer", "Data", nullptr));
        rightTabs->setTabText(rightTabs->indexOf(plotTab), QApplication::translate("SimpleCsvLogAnalyzer", "Plot", nullptr));
        rightTabs->setTabText(rightTabs->indexOf(dataTableViewTab), QApplication::translate("SimpleCsvLogAnalyzer", "Data Table", nullptr));
        minValueX->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Min X: nan", nullptr));
        maxValueX->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Max X: nan", nullptr));
        averageValueX->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Average X: nan", nullptr));
        spanValueX->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Span X: nan", nullptr));
        minValueY->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Min Y: nan", nullptr));
        maxValueY->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Max Y: nan", nullptr));
        averageValueY->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Average Y: nan", nullptr));
        spanValueY->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Span Y: nan", nullptr));
        totalDataLength->setText(QApplication::translate("SimpleCsvLogAnalyzer", "Data Length: nan", nullptr));
        menuFile->setTitle(QApplication::translate("SimpleCsvLogAnalyzer", "File", nullptr));
        menuPlot->setTitle(QApplication::translate("SimpleCsvLogAnalyzer", "Plot", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimpleCsvLogAnalyzer: public Ui_SimpleCsvLogAnalyzer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLECSVLOGANALYZER_H
