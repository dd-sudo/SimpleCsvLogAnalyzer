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

    QPixmap pixmap(":/images/images/timer.png");
    loadingSplash = new QSplashScreen(pixmap,Qt::SplashScreen);

    // init dataTableView custom context menu
    tableViewContextMenu = new QMenu();
    tableViewContextMenu->addAction("Plot",this,SLOT(plotThis_forTable()));
    tableViewContextMenu->addAction("Statistics",this,SLOT(statisticsForThis_forTable()));
    tableViewContextMenu->addAction("Hide Selected Columns",this,SLOT(hideTableColumns_forTable()));
    tableViewContextMenu->addAction("Show Selected Columns Only",this,SLOT(showTableColumns_forTable()));
    tableViewContextMenu->addAction("Show All Columns",this,SLOT(restoreTableColumns_forTable()));

    // init dataTableView custom context menu
    labelListContextMenu = new QMenu();
    labelListContextMenu->addAction("Plot vs Record Number",this,SLOT(plotThis_forList()));
    labelListContextMenu->addAction("Plot vs Another Data Label",this,SLOT(plotThisVsAnotherDataLabel_forList()));
    labelListContextMenu->addAction("Statistics",this,SLOT(statisticsForThis_forList()));

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

statistics SimpleCsvLogAnalyzer::calculateStatisticalData(QVector<double> x, QVector<double> y, bool showReportMessage){
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
    if(showReportMessage){
        QString s = QString("Statistics for selected data\n"
                            "Min:%1\n"
                            "Max:%2\n"
                            "Average:%3\n"
                            "Span:%4\n"
                            "Length:%5")
                .arg(ret.y.min)
                .arg(ret.y.max)
                .arg(ret.y.average)
                .arg(ret.y.span)
                .arg(ret.y.length);
        QMessageBox msg;
        msg.setText(s);
        msg.exec();
    }
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

void SimpleCsvLogAnalyzer::startBusy()
{
    loadingSplash->move(this->x()+this->width()/2,this->y()+this->height()/2);
    loadingSplash->show();
    qApp->processEvents();
}

void SimpleCsvLogAnalyzer::stopBusy()
{
    qApp->processEvents();
    loadingSplash->close();
}

QVector<double> SimpleCsvLogAnalyzer::createDerivedDataLabel(QStringList formula)
{
    QVector<double> dd,tmp;
    if(formula.isEmpty()){
        ui->statusbar->showMessage("Empty formula for derived data!");
        return dd;
    }
    qDebug() << "Formula:" << formula;
    if(mathOperatorsList.contains(formula[0])){
        ui->statusbar->showMessage("Illegal formula for derived data!");
        return dd;
    }
    QStringList labelList;
    for(int i=0;i<ui->dataListY->count();++i){
        labelList << ui->dataListY->item(i)->text();
    }
    // Derive data using formula
    for (int i=0;i<formula.length();++i) {
        if(mathOperatorsList.contains(formula[i])){
            qDebug() << "We got math op:" << formula[i];
            switch (mathOperatorsList.indexOf(formula[i])) {
            case 0: // +
                if(++i<formula.length()){
                    tmp = csvFile.getDataByName(formula[i]);
                }
                if(tmp.length() != dd.length()){
                    qDebug() << "Data length error!";
                    return dd;
                }
                // sum dd and tmp
                for(int i=0;i<dd.length();++i){
                    dd[i] += tmp[i];
                }
                break;
            case 1: // -
                if(++i<formula.length()){
                    tmp = csvFile.getDataByName(formula[i]);
                }
                if(tmp.length() != dd.length()){
                    qDebug() << "Data length error!";
                    return dd;
                }
                // subtract dd and tmp
                for(int i=0;i<dd.length();++i){
                    dd[i] -= tmp[i];
                }
                break;
            case 2: // *
                if(++i<formula.length()){
                    tmp = csvFile.getDataByName(formula[i]);
                }
                if(tmp.length() != dd.length()){
                    qDebug() << "Data length error!";
                    return dd;
                }
                // multiply dd and tmp
                for(int i=0;i<dd.length();++i){
                    dd[i] *= tmp[i];
                }
                break;
            case 3: // /
                if(++i<formula.length()){
                    tmp = csvFile.getDataByName(formula[i]);
                }
                if(tmp.length() != dd.length()){
                    qDebug() << "Data length error!";
                    return dd;
                }
                // divide dd and tmp
                for(int i=0;i<dd.length();++i){
                    dd[i] /= tmp[i];
                }
                break;
            }

        } else if (labelList.contains(formula[i])){
            qDebug() << "We got value label:" << formula[i];
            tmp = csvFile.getDataByName(formula[i]);
            if(i==0){
                dd.append(tmp);
            }
        }
    }
    return dd;
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
    startBusy();

    ui->dataListX->clear();
    ui->dataListY->clear();

    clearPlotNDisableTracer();
    clearStatisticsLabels();
    qApp->processEvents();

    csvFile.setFilePath(filePath);
    m_labels = csvFile.getCsvFileLabels();
    m_sampleValues = csvFile.getSampleValuesForLabels();
    ui->dataListY->addItems(m_labels);
    ui->dataListX->addItems(m_labels);
    csvFile.file2TableWidget(ui->dataTableWidget);

    stopBusy();

}

void SimpleCsvLogAnalyzer::plotGraph(QString xName, QString yName)
{
    if(xName.isEmpty() && yName.isEmpty()){
        qDebug() << "Select label(s) before plotting...";
        return;
    }
    startBusy();
    clearPlotNDisableTracer();

    if(xName.isEmpty()){
        if(yName.startsWith(mathStringStart)){
            qDebug() << "We got derived data here, name:" << yName;
            qDebug() << "Available derived data labels:" << derivedDataVectorLabels;
            qDebug() << yName << "index in derivedDataVectorLabels" << derivedDataVectorLabels.indexOf(yName);
            // Get derived data from buffer
            if(derivedDataVectorLabels.indexOf(yName)>=0){
                qDebug() << "We have label for " << yName;
                if(derivedDataVector.length()>derivedDataVectorLabels.indexOf(yName)){
                    qDebug() << "We have data for " << yName;
                    yVals = derivedDataVector[derivedDataVectorLabels.indexOf(yName)];
                }
            }
        } else {
            yVals = csvFile.getDataByName(yName);
        }
        if(yVals.isEmpty()){
            ui->statusbar->showMessage("No Y axis data to plot!");
            return;
        }
        plotType = PLOT_Y_VS_POINT_NUM;
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
        yVals = csvFile.getDataByName(yName);
        xVals  = csvFile.getDataByName(xName);
        if(yVals.isEmpty()){
            ui->statusbar->showMessage("No Y axis data to plot!");
            return;
        }
        if(xVals.isEmpty()){
            ui->statusbar->showMessage("No X axis data to plot!");
            return;
        }
        plotType = PLOT_Y_VS_X;
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
    stats = calculateStatisticalData(xVals,yVals,false);
    populateStatisticsLabels();
    zoomReset();
    ui->rightTabs->setCurrentIndex(0);
    stopBusy();

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
    startBusy();
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

    stopBusy();
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
    //math işini hallet
    QStringList labelList;
    for(int i=0;i<ui->dataListY->count();++i){
        labelList << ui->dataListY->item(i)->text();
    }
    DataMath *math = new DataMath(labelList,mathOperatorsList);
    int ret = math->exec();
    qDebug() << "Return:" << ret;
    qDebug() << "Math ops: " << math->mathOps;
    if(ret){
        QString derivedDataLabel;
        derivedDataLabel += mathStringStart;
        for (int i=0;i<math->mathOps.length();++i) {
            derivedDataLabel += math->mathOps[i];
        }
        derivedDataLabel += mathStringEnd;
        ui->dataListY->addItem(derivedDataLabel);
        QVector<double> values = createDerivedDataLabel(math->mathOps);
        qDebug() << "Derived data  name:" << derivedDataLabel << "data length:" << values.length();
        derivedDataVector.append(values);
        derivedDataVectorLabels.append(derivedDataLabel);
    }

    math->deleteLater();
}

void SimpleCsvLogAnalyzer::on_rightTabs_currentChanged(int index)
{
    // Close loading indicator
    stopBusy();
}

void SimpleCsvLogAnalyzer::on_rightTabs_tabBarClicked(int index)
{
    startBusy();
}

void SimpleCsvLogAnalyzer::on_dataTableWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    tableViewContextMenu->exec(QCursor::pos());
}

void SimpleCsvLogAnalyzer::hideTableColumns_forTable()
{
    QItemSelectionModel *select = ui->dataTableWidget->selectionModel();

    if(!select->hasSelection()){ //check if has selection
        qDebug() << "no selection";
        return;
    }
    startBusy();
    QModelIndexList indexList = select->selectedColumns(); // return selected column(s)

    qDebug() << "Selected columns length:" << indexList.length();
    for(int i=0; i<indexList.length(); ++i){
        qDebug() << "Hide column:" << indexList[i].column();
        ui->dataTableWidget->hideColumn(indexList[i].column());
    }
    ui->dataTableWidget->clearSelection();
    stopBusy();
}

void SimpleCsvLogAnalyzer::showTableColumns_forTable()
{
    QItemSelectionModel *select = ui->dataTableWidget->selectionModel();

    if(!select->hasSelection()){ //check if has selection
        qDebug() << "no selection";
        return;
    }
    startBusy();
    QModelIndexList indexList = select->selectedColumns(); // return selected column(s)

    // Hide all
    for (int i=0; i<ui->dataTableWidget->columnCount(); ++i) {
        ui->dataTableWidget->hideColumn(i);
    }
    ui->dataTableWidget->clearSelection();
    // Show selected columns
    qDebug() << "Selected columns length:" << indexList.length();
    for(int i=0; i<indexList.length(); ++i){
        qDebug() << "Hide column:" << indexList[i].column();
        ui->dataTableWidget->showColumn(indexList[i].column());
    }
    stopBusy();
}

void SimpleCsvLogAnalyzer::restoreTableColumns_forTable()
{
    startBusy();
    for (int i=0; i<ui->dataTableWidget->columnCount(); ++i) {
        ui->dataTableWidget->showColumn(i);
    }
    ui->dataTableWidget->clearSelection();
    stopBusy();
}

void SimpleCsvLogAnalyzer::plotThis_forTable()
{
    QItemSelectionModel *select = ui->dataTableWidget->selectionModel();

    if(!select->hasSelection()){ //check if has selection
        qDebug() << "no selection";
        return;
    }
    startBusy();
    QModelIndexList indexList = select->selectedColumns(); // return selected column(s)
    qDebug() << "Plot: Selected column:" << indexList.first().column()
             << "name:" << ui->dataTableWidget->horizontalHeaderItem(indexList.first().column())->text();
    plotGraph("",ui->dataTableWidget->horizontalHeaderItem(indexList.first().column())->text());
    ui->dataTableWidget->clearSelection();
    stopBusy();
}

void SimpleCsvLogAnalyzer::statisticsForThis_forTable()
{
    QItemSelectionModel *select = ui->dataTableWidget->selectionModel();

    if(!select->hasSelection()){ //check if has selection
        qDebug() << "no selection";
        return;
    }
    startBusy();
    QModelIndexList indexList = select->selectedColumns(); // return selected column(s)
    qDebug() << "Statistics: Selected column:" << indexList.first().column()
             << "name:" << ui->dataTableWidget->horizontalHeaderItem(indexList.first().column())->text();
    calculateStatisticalData(QVector<double>(),csvFile.getDataByName(ui->dataTableWidget->horizontalHeaderItem(indexList.first().column())->text()),true);
    stopBusy();
    ui->dataTableWidget->clearSelection();
}

void SimpleCsvLogAnalyzer::plotThis_forList()
{
    QItemSelectionModel *select = ui->dataListY->selectionModel();

    if(!select->hasSelection()){ //check if has selection
        qDebug() << "no selection";
        return;
    }
    startBusy();
    QModelIndexList indexList = select->selectedRows(); // return selected column(s)
    qDebug() << "Plot: Selected column:" << indexList.first().row()
             << "name:" << ui->dataListY->item(indexList.first().row())->text();
    plotGraph("",ui->dataListY->item(indexList.first().row())->text());
    ui->dataTableWidget->clearSelection();
    stopBusy();
}

void SimpleCsvLogAnalyzer::plotThisVsAnotherDataLabel_forList()
{
    QItemSelectionModel *select = ui->dataListY->selectionModel();

    if(!select->hasSelection()){ //check if has selection
        qDebug() << "no selection";
        return;
    }
    QStringList labelList;
    for(int i=0;i<ui->dataListY->count();++i){
        labelList << ui->dataListY->item(i)->text();
    }
    QString selectedXAxisLabel = QInputDialog::getItem(this,"Select data label for X axis","X axis data label:",labelList,0);
    startBusy();
    QModelIndexList indexList = select->selectedRows(); // return selected column(s)
    qDebug() << "Plot: Selected column:" << indexList.first().row()
             << "name:" << ui->dataListY->item(indexList.first().row())->text();
    plotGraph(selectedXAxisLabel,ui->dataListY->item(indexList.first().row())->text());
    ui->dataTableWidget->clearSelection();
    stopBusy();
}

void SimpleCsvLogAnalyzer::statisticsForThis_forList()
{
    QItemSelectionModel *select = ui->dataListY->selectionModel();

    if(!select->hasSelection()){ //check if has selection
        qDebug() << "no selection";
        return;
    }
    startBusy();
    QModelIndexList indexList = select->selectedRows(); // return selected column(s)
    qDebug() << "Statistics: Selected column:" << indexList.first().row()
             << "name:" << ui->dataListY->item(indexList.first().row())->text();
    calculateStatisticalData(QVector<double>(),csvFile.getDataByName(ui->dataListY->item(indexList.first().row())->text()),true);
    stopBusy();
    ui->dataTableWidget->clearSelection();
}


void SimpleCsvLogAnalyzer::on_dataListY_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    labelListContextMenu->exec(QCursor::pos());
}
