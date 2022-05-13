#include "pid.h"

PID::PID(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    reset();

    axisX = new QValueAxis();
    axisY = new QValueAxis();

    //TODO
    series = new QSplineSeries(this);
    QPen green(Qt::red);
    green.setWidth(3);
    series->setPen(green);
    series->append(x, y);

    // TODO
    addSeries(series);
    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    axisX->setTickCount(5);
    axisX->setRange(0, 10);
    axisY->setRange(-5, 10);

    timer.setInterval(DELAY);
    connect(&timer, &QTimer::timeout, this, &PID::timeout);
}

PID::~PID()
{

}

void PID::timeout()
{
    //TODO
    qreal tx = 2;//plotArea().width() / axisX->tickCount();
    qreal ty = (axisX->max() - axisX->min()) / axisX->tickCount();
    x += ty;
    y = QRandomGenerator::global()->bounded(5) - 2.5;
    series->append(x, y);
    scroll(tx, 0);

    calculateOffset();
    calculatePID();
    emit sendControlAndOffset(control, offset);

}

void PID::calculateOffset()
{
    offset = control + desiredValue;
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
    series = 0;
    step = 0;
    x = 5;
    y = 1;


    desiredValue = 50;
    Ts = 20;
    Kp = 2;
    Ki = 24;
    Td = 1;

    offset = 0;
    control = 0;
    memset(prevOffset, 0, sizeof(prevOffset));
}


