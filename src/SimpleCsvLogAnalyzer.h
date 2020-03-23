#ifndef SIMPLECSVLOGANALYZER_H
#define SIMPLECSVLOGANALYZER_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QDialog>
#include <QFileDialog>
#include <QShortcut>

#include "QCustomPlot/qcustomplot.h"
#include "CsvFileProcessor.h"

#include <limits>

enum{
    PLOT_NONE,
    PLOT_Y_VS_POINT_NUM,
    PLOT_Y_VS_X,
    PLOT_Y1_AND_Y2_VS_POINT_NUM,
    PLOT_Y1_Y2_VS_X
};

typedef struct{
    double min = 0;
    double max = 0;
    double span = 0;
    double average = 0;
    double median = 0;
}statistics;

QT_BEGIN_NAMESPACE
namespace Ui { class SimpleCsvLogAnalyzer; }
QT_END_NAMESPACE

class SimpleCsvLogAnalyzer : public QMainWindow
{
    Q_OBJECT

public:
    // variables

    // methods
    SimpleCsvLogAnalyzer(QWidget *parent = nullptr);
    ~SimpleCsvLogAnalyzer();

private slots:
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_plotSelected_clicked();
    void on_plotSelectedXY_clicked();
    void slotMouseMove(QMouseEvent * event);
    void moveRigthPressed();
    void on_actionZoom_Reset_triggered();
    void on_actionSave_Plot_Image_triggered();
    void on_dataListY_currentRowChanged(int currentRow);

private:
    // variables
    Ui::SimpleCsvLogAnalyzer *ui;
    CsvFileProcessor csvFile;
    QCPItemTracer *tracer;
    QCPCurve *verticalLine;
    QShortcut *moveRight,*moveLeft;
    double mousePlotCoordX = 0;
    double mousePlotCoordY = 0;
    QVector<double> xVals;
    QVector<double> yVals;
    statistics xAxisStats,yAxisStats;
    QStringList m_labels,m_sampleValues;
    int plotType = PLOT_NONE;

    // Plots
    QCPGraph *valuePlot;
    QCPGraph *cursorPlot;

    // methods
    void cleanUpAndExit();
    void initPlot();
    void zoomReset();
    QList<double> calculateSlopeOfCurve(int valueIndex);
    void clearPlotNDisableTracer();
    void setupTracer();
    void calculateStatisticalData(bool calc4xAxis, bool calc4yAxis);
    void populateStatisticsLabels();
};
#endif // 
