#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pid = new PID();
    connect(pid, &PID::sendControlAndOffset, this, &MainWindow::displayValues, Qt::QueuedConnection);

    resetSimulation();
    InitGUI();
    InitChart();
}

MainWindow::~MainWindow()
{
    delete pid;
    delete chartView;
    delete chart;
    delete ui;
}

void MainWindow::InitGUI()
{
    // control
    connect(ui->horizontalSlider_desiredValue,  &QSlider::valueChanged,  ui->spinBox_desiredValue,          &QSpinBox::setValue);
    connect(ui->spinBox_desiredValue,           &QSpinBox::valueChanged, ui->horizontalSlider_desiredValue, &QSlider::setValue);

    // pid parameters
    connect(ui->spinBox_Ki, &QDoubleSpinBox::valueChanged, this, &MainWindow::setKi);
    connect(ui->spinBox_Kp, &QDoubleSpinBox::valueChanged, this, &MainWindow::setKp);
    connect(ui->spinBox_Td, &QDoubleSpinBox::valueChanged, this, &MainWindow::setTd);
    connect(ui->spinBox_Ts, &QSpinBox::valueChanged,       this, &MainWindow::setTs);

    // buttons
    connect(ui->buttonStart, &QPushButton::pressed, this, &MainWindow::startSimulation);
    connect(ui->buttonStop,  &QPushButton::pressed, this, &MainWindow::stopSimulation);
    connect(ui->buttonReset, &QPushButton::pressed, this, &MainWindow::resetSimulation);
}

void MainWindow::InitChart()
{
    chart = new QChart();
    chart->setTitle("Line chart");

//    QString name("Series ");
//    int nameIndex = 0;
//    for (const DataList &list : m_dataTable) {
//        QLineSeries *series = new QLineSeries(chart);
//        for (const Data &data : list)
//            series->append(data.first);
//        series->setName(name + QString::number(nameIndex));
//        nameIndex++;
//        chart->addSeries(series);
//    }

//    chart->createDefaultAxes();
//    chart->axes(Qt::Horizontal).first()->setRange(0, m_valueMax);
//    chart->axes(Qt::Vertical).first()->setRange(0, m_valueCount);

    // Add space to label to add space between labels and axis
//    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
//    Q_ASSERT(axisY);
//    axisY->setLabelFormat("%.1f  ");

    chartView = new QChartView(chart);
    ui->groupBox_Chart->layout()->addWidget(chartView);
}


void MainWindow::displayValues(float control, float offset)
{
    ui->ControlValue->setText(QString::number(control));
    ui->OffsetValue->setText(QString::number(offset));
}

void MainWindow::startSimulation()
{
    pid->start();
}

void MainWindow::stopSimulation()
{
    pid->stop();
}

void MainWindow::resetSimulation()
{
    pid->reset();

    ui->horizontalSlider_desiredValue->setValue(pid->desiredValue);
    ui->spinBox_desiredValue->setValue(pid->desiredValue);
    ui->spinBox_Ki->setValue(pid->Ki);
    ui->spinBox_Kp->setValue(pid->Kp);
    ui->spinBox_Td->setValue(pid->Td);
    ui->spinBox_Ts->setValue(pid->Ts);
}

//
void MainWindow::setKp(float value)
{
    this->pid->Kp = value;
}

void MainWindow::setKi(float value)
{
    this->pid->Ki = value;
}

void MainWindow::setTd(float value)
{
    this->pid->Td = value;
}

void MainWindow::setTs(int value)
{
    this->pid->Ts = value;
}

void MainWindow::setDesiredValue(int value)
{
    this->pid->desiredValue = value;
}

