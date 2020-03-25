#ifndef SIMPLECSVLOGANALYZER_H
#define SIMPLECSVLOGANALYZER_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QDialog>
#include <QFileDialog>
#include <QShortcut>
#include <QMenu>

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
    int length;
}_statistics;

typedef struct{
    _statistics x;
    _statistics y;
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

    void plotGraph(QString xName, QString yName);

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
    void on_pushButton_clicked();
    void on_rightTabs_currentChanged(int index);
    void on_rightTabs_tabBarClicked(int index);


    void on_dataTableWidget_customContextMenuRequested(const QPoint &pos);

public slots:
    // Slots for data table view custom context menu options
    void hideThis();
    void plotThis();
    void statisticsForThis();


private:
    // variables
    Ui::SimpleCsvLogAnalyzer *ui;
    CsvFileProcessor csvFile;
    QCPItemTracer *tracer;
    QCPCurve *verticalLine;
    QShortcut *moveRight,*moveLeft;
    QSplashScreen *loadingSplash;
    QMenu *tableViewContextMenu;
    double mousePlotCoordX = 0;
    double mousePlotCoordY = 0;
    QVector<double> xVals;
    QVector<double> yVals;
    statistics stats;

    QStringList m_labels,m_sampleValues;
    int plotType = PLOT_NONE;

    // Plots
    QCPGraph *valuePlot;
    QCPGraph *cursorPlot;

    // methods
    void cleanUpAndExit();
    void initPlot();
    void initDataTable();
    void zoomReset();
    QList<double> calculateSlopeOfCurve(int valueIndex);
    void clearPlotNDisableTracer();
    void setupTracer();
    statistics calculateStatisticalData(QVector<double> x, QVector<double> y);
    void populateStatisticsLabels();
    void clearStatisticsLabels();

};
#endif // 
