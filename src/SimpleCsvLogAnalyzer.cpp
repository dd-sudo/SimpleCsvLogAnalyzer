﻿#include "SimpleCsvLogAnalyzer.h"
#include "ui_SimpleCsvLogAnalyzer.h"

SimpleCsvLogAnalyzer::SimpleCsvLogAnalyzer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SimpleCsvLogAnalyzer)
{
    ui->setupUi(this);

    // setup some shortcuts
    moveRight = new QShortcut(this);
    moveRight->setKey(Qt::Key_Right);
    connect(moveRight,SIGNAL(activated()),this,SLOT(moveRigthPressed()));

    QPixmap pixmap(":/images/images/timer.png");
    loadingSplash = new QSplashScreen(pixmap,Qt::SplashScreen);

    // init dataTableView custom context menu
    tableViewContextMenu = new QMenu("Data Table Operations");
    tableViewContextMenu->addAction("Plot",this,SLOT(plotThis()));
    tableViewContextMenu->addAction("Statistics",this,SLOT(statisticsForThis()));
    tableViewContextMenu->addAction("Hide",this,SLOT(hideThis()));

    initPlot();
    initDataTable();

}

SimpleCsvLogAnalyzer::~SimpleCsvLogAnalyzer()
{
    delete ui;
}

void SimpleCsvLogAnalyzer::on_actionExit_triggered()
{
    cleanUpAndExit();
    exit(0);
}

void SimpleCsvLogAnalyzer::cleanUpAndExit()
{
    // TDB
}

void SimpleCsvLogAnalyzer::initPlot(){

    // Clear graph etc.
    disconnect(ui->plot1, &QCustomPlot::mouseMove, this, &SimpleCsvLogAnalyzer::slotMouseMove);
    ui->plot1->clearGraphs();

    // Create value and cursor plot
    valuePlot = ui->plot1->addGraph();
    //cursorPlot = ui->plot1->addGraph();



    // Allow draggingplot in X axis only (horizontal)
    ui->plot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->plot1->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->plot1->axisRect()->setRangeZoom(Qt::Horizontal);

    // setup tracer
    verticalLine = new QCPCurve(ui->plot1->xAxis, ui->plot1->yAxis);
    QVector<double> x(2) , y(2);
    x[0] = 0;
    y[0] = 0;
    x[1] = 0;
    y[1] = 0;

    verticalLine->setName("Vertical");
    verticalLine->setData(x, y);
    tracer = new QCPItemTracer(ui->plot1);

    tracer->setStyle(QCPItemTracer::tsCrosshair);
    tracer->setPen(QPen(Qt::darkGray));
    connect(ui->plot1, &QCustomPlot::mouseMove, this, &SimpleCsvLogAnalyzer::slotMouseMove);
}

void SimpleCsvLogAnalyzer::initDataTable()
{
    // Auto adjust horizontal header labels
    ui->dataTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void SimpleCsvLogAnalyzer::clearPlotNDisableTracer(){
    xVals.clear();
    yVals.clear();
    valuePlot->setData(xVals,yVals);
    tracer->setGraph(nullptr);
}

void SimpleCsvLogAnalyzer::setupTracer()
{
    tracer->setGraph(valuePlot);
}

void SimpleCsvLogAnalyzer::zoomReset(){
    // Rescales X axis using default method (min and max of x values)
    // rescales Y axis using min and max of Y axis values, with a margin of 1/10th of the span
    // to provide better visibilty for peaks and valleys in plot curve
    valuePlot->rescaleKeyAxis();
    ui->plot1->yAxis->setRange(stats.y.min-0.1*stats.y.span,stats.y.max+0.1*stats.y.span);
    ui->plot1->replot();
}

QList<double> SimpleCsvLogAnalyzer::calculateSlopeOfCurve(int valueIndex){
    QList<double> ret;
    ret.append(NAN);
    ret.append(NAN);
    if( ((valueIndex+1) < yVals.length()) && ((valueIndex)>0) ){
        ret[0] = (yVals[valueIndex] - yVals[valueIndex-1]); // left slope
        ret[1] = (yVals[valueIndex+1] - yVals[valueIndex]); // right slope
    }
    return ret;
}

statistics SimpleCsvLogAnalyzer::calculateStatisticalData(QVector<double> x, QVector<double> y){
    statistics ret;
        ret.x.min = std::numeric_limits<double>::max();
        ret.x.max = std::numeric_limits<double>::min();
        ret.x.span = 0;
        ret.x.average = 0;
        int averageArrayLen = x.length();
        if(x.length()>0){
            double accu = 0;
            double tmp;
            for(int i=0; i<x.length(); ++i){
                tmp = x[i];
                if(!qIsNaN(tmp)){
                    accu += tmp;
                } else {
                    --averageArrayLen;
                }
                if(tmp<ret.x.min){
                    ret.x.min = tmp;
                }
                if(tmp>ret.x.max){
                    ret.x.max = tmp;
                }
            }
            ret.x.average = accu/averageArrayLen;
            ret.x.span = ret.x.max - ret.x.min;
        } else {
            qDebug() << "No data in array!";
            ret.x.min = NAN;
            ret.x.max = NAN;
            ret.x.span = NAN;
            ret.x.average = NAN;
        }
        ret.x.length = x.length();


        ret.y.min = std::numeric_limits<double>::max();
        ret.y.max = std::numeric_limits<double>::min();
        ret.y.span = 0;
        ret.y.average = 0;
        averageArrayLen = y.length();
        if(y.length()>0){
            double accu = 0;
            double tmp;
            for(int i=0; i<y.length(); ++i){
                tmp = y[i];
                if(!qIsNaN(tmp)){
                    accu += tmp;
                } else {
                    --averageArrayLen;
                }
                if(tmp<ret.y.min){
                    ret.y.min = tmp;
                }
                if(tmp>ret.y.max){
                    ret.y.max = tmp;
                }
            }
            ret.y.average = accu/averageArrayLen;
            ret.y.span = ret.y.max - ret.y.min;
        } else {
            qDebug() << "No data in array!";
            ret.y.min = NAN;
            ret.y.max = NAN;
            ret.y.span = NAN;
            ret.y.average = NAN;
        }
        ret.y.length = y.length();

        return ret;

}

void SimpleCsvLogAnalyzer::populateStatisticsLabels(){
    // X Axis
    ui->minValueX->setText("Min X: " + QString::number(stats.x.min));
    ui->maxValueX->setText("Max X: " + QString::number(stats.x.max));
    ui->spanValueX->setText("Span X: " + QString::number(stats.x.span));
    ui->averageValueX->setText("Average X: " + QString::number(stats.x.average));
    // Y Axis
    ui->minValueY->setText("Min Y: " + QString::number(stats.y.min));
    ui->maxValueY->setText("Max Y: " + QString::number(stats.y.max));
    ui->spanValueY->setText("Span Y: " + QString::number(stats.y.span));
    ui->averageValueY->setText("Average Y: " + QString::number(stats.y.average));
    //Data length
    ui->totalDataLength->setText(QString("Data Length: %1").arg(stats.y.length));

}

void SimpleCsvLogAnalyzer::clearStatisticsLabels()
{
    // X Axis
    ui->minValueX->setText("Min X: nan");
    ui->maxValueX->setText("Max X: nan");
    ui->spanValueX->setText("Span X: nan");
    ui->averageValueX->setText("Average X: nan");
    // Y Axis
    ui->minValueY->setText("Min Y: nan");
    ui->maxValueY->setText("Max Y: nan");
    ui->spanValueY->setText("Span Y: nan");
    ui->averageValueY->setText("Average Y: nan");
    //Data length
    ui->totalDataLength->setText(QString("Data Length: N/A"));
}

void SimpleCsvLogAnalyzer::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Select a file...","","*.csv");
    qDebug() << "filePath:" << filePath;
    if(filePath.isEmpty()){
        qDebug() << "Dosya seçilmedi";
        return;
    }
    ui->rightTabs->setCurrentIndex(0);
    loadingSplash->move(this->x()+this->width()/2,this->y()+this->height()/2);
    loadingSplash->show();
    qApp->processEvents();

    ui->dataListX->clear();
    ui->dataListY->clear();

    clearPlotNDisableTracer();
    clearStatisticsLabels();

    csvFile.setFilePath(filePath);
    m_labels = csvFile.getCsvFileLabels();
    m_sampleValues = csvFile.getSampleValuesForLabels();
    ui->dataListY->addItems(m_labels);
    ui->dataListX->addItems(m_labels);
    csvFile.file2TableWidget(ui->dataTableWidget);

    qApp->processEvents();
    loadingSplash->close();

}

void SimpleCsvLogAnalyzer::plotGraph(QString xName, QString yName)
{
    if(xName.isEmpty() && yName.isEmpty()){
        qDebug() << "Select label(s) before plotting...";
        return;
    }
    loadingSplash->move(this->x()+this->width()/2,this->y()+this->height()/2);
    loadingSplash->show();
    qApp->processEvents();
    clearPlotNDisableTracer();

    if(xName.isEmpty()){
        plotType = PLOT_Y_VS_POINT_NUM;
        yVals = csvFile.getDataByName(yName);
        xVals.resize(yVals.length());
        for(int i=0;i<xVals.length();++i){
            xVals[i] = i;
        }
        ui->plot1->xAxis->setLabel("Time Points");
        ui->plot1->yAxis->setLabel(yName);
        // configure plot style
        valuePlot->setLineStyle(QCPGraph::lsLine);
        valuePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot, 1));
        valuePlot->setData(xVals,yVals);
        QPen pen = valuePlot->pen();
        pen.setColor(Qt::blue);
        valuePlot->setPen(pen);
    } else {
        plotType = PLOT_Y_VS_X;
        yVals = csvFile.getDataByName(yName);
        xVals  = csvFile.getDataByName(xName);
        ui->plot1->xAxis->setLabel(xName);
        ui->plot1->yAxis->setLabel(yName);
        // configure plot style
        valuePlot->setLineStyle(QCPGraph::lsNone);
        valuePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 3));
        valuePlot->setData(xVals,yVals);
        QPen pen = valuePlot->pen();
        pen.setColor(Qt::red);
        valuePlot->setPen(pen);
    }

    qDebug() << xVals.length() << yVals.length();
    valuePlot->setData(xVals,yVals);

    setupTracer();
    stats = calculateStatisticalData(xVals,yVals);
    populateStatisticsLabels();
    zoomReset();
    ui->rightTabs->setCurrentIndex(0);
    qApp->processEvents();
    loadingSplash->close();

}

void SimpleCsvLogAnalyzer::on_plotSelected_clicked()
{
    // Plots values of selected data name versus recorda/data point
    if(ui->dataListY->selectedItems().isEmpty()){
        ui->statusbar->showMessage("Select a proper data for both x and y axis!");
        return;
    }

    QString name = ui->dataListY->selectedItems().first()->text();

    plotGraph("",name);

}

void SimpleCsvLogAnalyzer::on_plotSelectedXY_clicked()
{
    // Plots values of selected data name versus selected another data field
    if(ui->dataListY->selectedItems().isEmpty() | ui->dataListX->selectedItems().isEmpty()){
        ui->statusbar->showMessage("Select a proper data for both x and y axis!");
        return;
    }

    QString nameY = ui->dataListY->selectedItems().first()->text();
    QString nameX = ui->dataListX->selectedItems().first()->text();

    plotGraph(nameX,nameY);
}

void SimpleCsvLogAnalyzer::slotMouseMove(QMouseEvent *event)
{
    if(plotType == PLOT_NONE){
        return;
    }

    // Find corresponding graph value for both y and x
    mousePlotCoordX = ui->plot1->xAxis->pixelToCoord(event->pos().x());
    mousePlotCoordY = ui->plot1->yAxis->pixelToCoord(event->pos().y());
    //qDebug() << "Mouse plot coordinates. X:" << mousePlotCoordX << "Y:" << mousePlotCoordY;

    QVector<double> x(2), y(2);
    x[0] = mousePlotCoordX;
    y[0] = 0;
    x[1] = mousePlotCoordX;
    y[1] = 0;

    verticalLine->setData(x, y);
    QList<double> ret;
    tracer->setGraphKey(mousePlotCoordX);
    //qDebug() << "Slope of curve at:" << mousePlotCoordX << "is:" << calculateSlopeOfCurve(mousePlotCoordX);
    switch (plotType) {
    case PLOT_Y_VS_POINT_NUM:
        ret = calculateSlopeOfCurve(tracer->position->key());
        ui->statusbar->showMessage(QString("%1: %2, %3: %4, Left Slope: %5, Right Slope:%6")
                                   .arg(ui->plot1->xAxis->label())
                                   .arg(QString::number(tracer->position->key()))
                                   .arg(ui->plot1->yAxis->label())
                                   .arg(QString::number(tracer->position->value()))
                                   .arg(ret[0])
                .arg(ret[1]));
        break;
    case PLOT_Y_VS_X:
        ui->statusbar->showMessage(QString("%1: %2, %3: %4, Y/X: %5, Y/X of Mouse: %6").arg(ui->plot1->xAxis->label())
                                   .arg(QString::number(tracer->position->key()))
                                   .arg(ui->plot1->yAxis->label())
                                   .arg(QString::number(tracer->position->value()))
                                   .arg(QString::number(tracer->position->value()/tracer->position->key()))
                                   .arg(mousePlotCoordY/mousePlotCoordX));
        break;
    case PLOT_Y1_AND_Y2_VS_POINT_NUM:

        break;
    case PLOT_Y1_Y2_VS_X:

        break;
    default:
        break;
    }

    ui->plot1->replot();
}

void SimpleCsvLogAnalyzer::moveRigthPressed()
{
    qDebug() << "moveRigthPressed";
}

void SimpleCsvLogAnalyzer::on_actionZoom_Reset_triggered()
{
    zoomReset();
}

void SimpleCsvLogAnalyzer::on_actionSave_Plot_Image_triggered()
{
    // Ask user to enter image name to save plot
    // It is covenient to generate a date-time and data field name based dafault file name for lazy users
    QString filePath = QFileDialog::getSaveFileName(this,"Select a file...",
                                                    QString("plot_capture_%1_vs_%2_%3.jpg")
                                                    .arg(ui->plot1->xAxis->label().remove(QRegExp("[^a-zA-Z\\d\\s]")))
                                                    .arg(ui->plot1->yAxis->label().remove(QRegExp("[^a-zA-Z\\d\\s]")))
                                                    .arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss")),
                                                    "*.jpg");
    loadingSplash->move(this->x()+this->width()/2,this->y()+this->height()/2);
    loadingSplash->show();
    qApp->processEvents();
    qDebug() << "filePath:" << filePath;
    if(filePath.isEmpty()){
        qDebug() << "Dosya seçilmedi";
        return;
    }
    // Hide tracer, before saving plot as image
    tracer->setVisible(false);
    ui->plot1->replot();
    ui->plot1->saveJpg(filePath,0,0,1,98);
    // Bring back tracer line
    tracer->setVisible(true);
    ui->plot1->replot();

    qApp->processEvents();
    loadingSplash->close();
}


void SimpleCsvLogAnalyzer::on_dataListY_currentRowChanged(int currentRow)
{
    if(currentRow<0){
        return;
    }
    QString s;
    if(currentRow>=m_sampleValues.length()){
        s = QString("No sample value for %1").arg(ui->dataListY->item(currentRow)->text());
    } else {
        s = QString("Sample value for %1 is %2").arg(ui->dataListY->item(currentRow)->text())
                .arg(m_sampleValues.at(currentRow));
    }
    ui->statusbar->showMessage(s);
}

void SimpleCsvLogAnalyzer::on_pushButton_clicked()
{

}

void SimpleCsvLogAnalyzer::on_rightTabs_currentChanged(int index)
{
    // Close loading indicator
    loadingSplash->close();
}

void SimpleCsvLogAnalyzer::on_rightTabs_tabBarClicked(int index)
{
    loadingSplash->move(this->x()+this->width()/2,this->y()+this->height()/2);
    loadingSplash->show();
    qApp->processEvents();
}

void SimpleCsvLogAnalyzer::on_dataTableWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    tableViewContextMenu->exec(QCursor::pos());
}

void SimpleCsvLogAnalyzer::hideThis()
{
    QList<QTableWidgetItem*> items = ui->dataTableWidget->selectedItems();
    if(items.isEmpty()){
        qDebug() << "no selection";
        return;
    }
    qDebug() << "Hide: Selected column:" << items.first()->column()
             << "name:" << ui->dataTableWidget->horizontalHeaderItem(items.first()->column())->text();
    ui->dataTableWidget->hideColumn(items.first()->column());
}

void SimpleCsvLogAnalyzer::plotThis()
{
    QList<QTableWidgetItem*> items = ui->dataTableWidget->selectedItems();
    if(items.isEmpty()){
        qDebug() << "no selection";
        return;
    }
    qDebug() << "Plot: Selected column:" << items.first()->column()
             << "name:" << ui->dataTableWidget->horizontalHeaderItem(items.first()->column())->text();
    plotGraph("",ui->dataTableWidget->horizontalHeaderItem(items.first()->column())->text());

}

void SimpleCsvLogAnalyzer::statisticsForThis()
{
    QList<QTableWidgetItem*> items = ui->dataTableWidget->selectedItems();
    if(items.isEmpty()){
        qDebug() << "no selection";
        return;
    }
    qDebug() << "Statistics: Selected column:" << items.first()->column()
             << "name:" << ui->dataTableWidget->horizontalHeaderItem(items.first()->column())->text();
    statistics tmp = calculateStatisticalData(QVector<double>(),csvFile.getDataByName(ui->dataTableWidget->horizontalHeaderItem(items.first()->column())->text()));
    QString s = QString("Statistics for selected data               \n"
                         "Data name:%1\n"
                         "Min:%2\n"
                         "Max:%3\n"
                         "Average:%4\n"
                         "Span:%5\n"
                         "Length:%6")
            .arg(ui->dataTableWidget->horizontalHeaderItem(items.first()->column())->text())
            .arg(tmp.y.min)
            .arg(tmp.y.max)
            .arg(tmp.y.average)
            .arg(tmp.y.span)
            .arg(tmp.y.length);
    QMessageBox msg;
    msg.setText(s);
    msg.exec();
}


