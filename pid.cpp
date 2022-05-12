#include "pid.h"

PID::PID(QObject *parent)
    : QObject{parent}
{
    reset();

    timer.setInterval(DELAY);
    connect(&timer, &QTimer::timeout, this, &PID::timeout);
}

void PID::timeout()
{
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
    desiredValue = 50;

    Ts = 20;
    Kp = 2;
    Ki = 24;
    Td = 1;

    offset = 0;
    control = 0;
    memset(prevOffset, 0, sizeof(prevOffset));
}
