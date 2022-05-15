#include "pid.h"

PID::PID(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    axisX = new QValueAxis();
    axisY = new QValueAxis();
    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisY,Qt::AlignLeft);
    axisX->setTickCount(10);
    axisX->setRange(0, 100);
    axisY->setRange(0, 100);

    initDataLines();
    reset();

    timer.setInterval(DELAY);
    connect(&timer, &QTimer::timeout, this, &PID::timeout);
}

PID::~PID()
{

}

void PID::initDataLines()
{
    QSplineSeries* controlSeries = new QSplineSeries(this);
    QPen blue(Qt::blue);
    blue.setWidth(3);
    controlSeries ->setPen(blue);

    QSplineSeries* setpointSeries = new QSplineSeries(this);
    QPen green(Qt::green);
    green.setWidth(3);
    setpointSeries->setPen(green);

    series << controlSeries << setpointSeries;

    for(int i = 0; i < DATA_TYPE_COUNT; i++)
    {
        series[i] = new QSplineSeries(this);
        series[i]->append(x,y);
        addSeries(series[i]);
        series[i]->attachAxis(axisX);
        series[i]->attachAxis(axisY);
    }
}

void PID::timeout()
{
    //TODO
    qreal tx = plotArea().width() / axisX->tickCount();
    qreal ty = (axisX->max() - axisX->min()) / axisX->tickCount();
    x += ty;
    y = QRandomGenerator::global()->bounded(100);
    series[DATA_TYPE_CONTROL]->append(x, y);
    series[DATA_TYPE_SETPOINT]->append(x, setpoint);
    scroll(tx, 0);

    calculateOffset();
    calculatePID();
    emit sendControlAndOffset(control, offset);

}

void PID::calculateOffset()
{
    offset = control + setpoint;
}

void PID::calculatePID()
{
    const float r0 = Kp * (1.0f + (float)Ts / (2.0f * Ki) + Td / (float)Ts);
    const float r1 = Kp * ((float)Ts / (2.0f * Ki) - 2.0f * Td / (float)Ts - 1.0f);
    const float r2 = Kp * Td / (float)Ts;

    control = (control + r0 * offset + r1 * prevOffset[1] + r2 * prevOffset[0]);

    prevOffset[0] = prevOffset[1];
    prevOffset[1] = offset;
}

void PID::start()
{
    timer.start();
}

void PID::stop()
{
    timer.stop();
}

void PID::reset()
{
    //TODO
    scroll(0, 0);
    for(int i = 0; i < DATA_TYPE_COUNT; i++)
    {
        series[i]->clear();
    }

    step = 0;
    x = 5;
    y = 0;

    // PID
    setpoint = 50;
    Ts = 20;
    Kp = 2;
    Ki = 24;
    Td = 1;

    offset = 0;
    control = 0;
    memset(prevOffset, 0, sizeof(prevOffset));
}

//
void PID::setKp(float value)
{
    this->Kp = value;
}

void PID::setKi(float value)
{
    this->Ki = value;
}

void PID::setTd(float value)
{
    this->Td = value;
}

void PID::setTs(int value)
{
    this->Ts = value;
}

void PID::setSetpoint(int value)
{
    this->setpoint = value;
}
