#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QtCharts>
#include "pid.h"

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setKp(float value);
    void setKi(float value);
    void setTd(float value);
    void setTs(int value);
    void setDesiredValue(int value);
    void startSimulation(void);
    void stopSimulation(void);
    void resetSimulation(void);
    //
    void displayValues(float control, float offset);

private:
    void InitGUI(void);
    void InitChart(void);

    Ui::MainWindow *ui;
    QChartView *chartView;
    QChart *chart;
    PID* pid;
};
#endif // MAINWINDOW_H
