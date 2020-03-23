#include "SimpleCsvLogAnalyzer.h"
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

    initPlot();
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
    ui->plot1->yAxis->setRange(yAxisStats.min-0.1*yAxisStats.span,yAxisStats.max+0.1*yAxisStats.span);
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

void SimpleCsvLogAnalyzer::calculateStatisticalData(bool calc4xAxis, bool calc4yAxis){
    if(calc4xAxis){
        xAxisStats.min = std::numeric_limits<double>::max();
        xAxisStats.max = std::numeric_limits<double>::min();
        xAxisStats.span = 0;
        xAxisStats.average = 0;
        int averageArrayLen = xVals.length();
        if(xVals.length()>0){
            double accu = 0;
            double tmp;
            for(int i=0; i<xVals.length(); ++i){
                tmp = xVals[i];
                if(!qIsNaN(tmp)){
                    accu += tmp;
                } else {
                    --averageArrayLen;
                }
                if(tmp<xAxisStats.min){
                    xAxisStats.min = tmp;
                }
                if(tmp>xAxisStats.max){
                    xAxisStats.max = tmp;
                }
            }
            xAxisStats.average = accu/averageArrayLen;
            xAxisStats.span = xAxisStats.max - xAxisStats.min;
        } else {
            qDebug() << "No data in array!";
        }
    }

    if(calc4yAxis){
        yAxisStats.min = std::numeric_limits<double>::max();
        yAxisStats.max = std::numeric_limits<double>::min();
        yAxisStats.span = 0;
        yAxisStats.average = 0;
        int averageArrayLen = yVals.length();
        if(yVals.length()>0){
            double accu = 0;
            double tmp;
            for(int i=0; i<yVals.length(); ++i){
                tmp = yVals[i];
                if(!qIsNaN(tmp)){
                    accu += tmp;
                } else {
                    --averageArrayLen;
                }
                if(tmp<yAxisStats.min){
                    yAxisStats.min = tmp;
                }
                if(tmp>yAxisStats.max){
                    yAxisStats.max = tmp;
                }
            }
            yAxisStats.average = accu/averageArrayLen;
            yAxisStats.span = yAxisStats.max - yAxisStats.min;
        } else {
            qDebug() << "No data in array!";
        }
    }
}

void SimpleCsvLogAnalyzer::populateStatisticsLabels(){
    // X Axis
    ui->minValueX->setText("Min X: " + QString::number(xAxisStats.min));
    ui->maxValueX->setText("Max X: " + QString::number(xAxisStats.max));
    ui->spanValueX->setText("Span X: " + QString::number(xAxisStats.span));
    ui->averageValueX->setText("Average X: " + QString::number(xAxisStats.average));
    // Y Axis
    ui->minValueY->setText("Min Y: " + QString::number(yAxisStats.min));
    ui->maxValueY->setText("Max Y: " + QString::number(yAxisStats.max));
    ui->spanValueY->setText("Span Y: " + QString::number(yAxisStats.span));
    ui->averageValueY->setText("Average Y: " + QString::number(yAxisStats.average));
}

void SimpleCsvLogAnalyzer::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Select a file...","","*.csv");
    qDebug() << "filePath:" << filePath;
    if(filePath.isEmpty()){
        qDebug() << "Dosya seçilmedi";
        return;
    }
    ui->dataListX->clear();
    ui->dataListY->clear();

    clearPlotNDisableTracer();

    csvFile.setFilePath(filePath);
    m_labels = csvFile.getCsvFileLabels();
    m_sampleValues = csvFile.getSampleValuesForLabels();
    ui->dataListY->addItems(m_labels);
    ui->dataListX->addItems(m_labels);

}


void SimpleCsvLogAnalyzer::on_plotSelected_clicked()
{
    // Plots values of selected data name versus recorda/data point
    if(ui->dataListY->selectedItems().isEmpty()){
        ui->statusbar->showMessage("Select a proper data for both x and y axis!");
        return;
    }
    plotType = PLOT_Y_VS_POINT_NUM;
    QString name = ui->dataListY->selectedItems().first()->text();
    qDebug() << "Selected data name:" << name;
    clearPlotNDisableTracer();
    yVals = csvFile.getDataByName(name);
    xVals.resize(yVals.length());
    for(int i=0;i<xVals.length();++i){
        xVals[i] = i;
    }
    qDebug() << xVals.length() << yVals.length();
    valuePlot->setData(xVals,yVals);

    ui->plot1->xAxis->setLabel("Time Points");
    ui->plot1->yAxis->setLabel(name);

    // configure plot style
    valuePlot->setLineStyle(QCPGraph::lsLine);
    valuePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot, 1));
    valuePlot->setData(xVals,yVals);
    QPen pen = valuePlot->pen();
    pen.setColor(Qt::blue);
    valuePlot->setPen(pen);
    setupTracer();
    calculateStatisticalData(false,true);
    populateStatisticsLabels();
    zoomReset();

}

void SimpleCsvLogAnalyzer::on_plotSelectedXY_clicked()
{
    // Plots values of selected data name versus selected another data field
    if(ui->dataListY->selectedItems().isEmpty() | ui->dataListX->selectedItems().isEmpty()){
        ui->statusbar->showMessage("Select a proper data for both x and y axis!");
        return;
    }
    plotType = PLOT_Y_VS_X;
    QString nameY = ui->dataListY->selectedItems().first()->text();
    QString nameX = ui->dataListX->selectedItems().first()->text();
    clearPlotNDisableTracer();
    qDebug() << "X:" << nameX << "Y:" << nameY;
    qDebug() << "Selected data name:" << nameY;
    yVals = csvFile.getDataByName(nameY);
    xVals  = csvFile.getDataByName(nameX);
    qDebug() << xVals.length() << yVals.length();
    //ui->plot1->legend->setVisible(true);

    // configure plot style
    valuePlot->setLineStyle(QCPGraph::lsNone);
    valuePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 3));
    valuePlot->setData(xVals,yVals);
    QPen pen = valuePlot->pen();
    pen.setColor(Qt::red);
    valuePlot->setPen(pen);

    ui->plot1->xAxis->setLabel(nameX);
    ui->plot1->yAxis->setLabel(nameY);
    setupTracer();
    calculateStatisticalData(true,true);
    populateStatisticsLabels();
    zoomReset();
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
