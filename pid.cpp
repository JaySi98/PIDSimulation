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
    initValues();

    timer.setInterval(DELAY);
    connect(&timer, &QTimer::timeout, this, &PID::timeout);
}

PID::~PID()
{

}

void PID::initDataLines()
{
    QSplineSeries* controlSeries = new QSplineSeries(this);
    controlSeries->append(0,control);
    controlSeries->setName("Control");
    QPen blue(Qt::blue);
    blue.setWidth(3);
    controlSeries ->setPen(blue);

    QSplineSeries* setpointSeries = new QSplineSeries(this);
    setpointSeries->append(0,setpoint);
    setpointSeries->setName("Setpoint");
    QPen green(Qt::green);
    green.setWidth(3);
    setpointSeries->setPen(green);

    series << controlSeries << setpointSeries;

    for(int i = 0; i < DATA_TYPE_COUNT; i++)
    {
        addSeries(series[i]);
        series[i]->attachAxis(axisX);
        series[i]->attachAxis(axisY);
    }
}

void PID::initValues()
{
    x = 5;

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

void PID::timeout()
{
    calculateOffset();
    calculatePID();

    //TODO
    qreal tx = plotArea().width() / axisX->tickCount();
    qreal ty = (axisX->max() - axisX->min()) / axisX->tickCount();
    x += ty;

    series[DATA_TYPE_CONTROL]->append(x, control);
    series[DATA_TYPE_SETPOINT]->append(x, setpoint);
    scroll(tx, 0);

    emit sendControlAndOffset(x, offset);

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

    if(control > MAX_VALUE)
        control = MAX_VALUE;
    else if(control < MIN_VALUE)
        control = MIN_VALUE;

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
    // I dont know why it doesnt work
    scroll(-axisX->max(),0);

    for(int i = 0; i < DATA_TYPE_COUNT; i++)
    {
        series[i]->clear();
    }

    initValues();
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
