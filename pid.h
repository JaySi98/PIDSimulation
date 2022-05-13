#ifndef PID_H
#define PID_H

#include <QObject>
#include <QtCharts/QChart>
#include <QTimer>
#include <QSplineSeries>
#include <QValueAxis>
#include <QRandomGenerator>

#define OFFSET_COUNT 2
#define DELAY 1000

class PID : public QChart
{
    Q_OBJECT
public:
    explicit PID(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    virtual ~PID();
    void start();
    void stop();
    void reset();

    int   Ts;
    float Kp;
    float Ki;
    float Td;

    int   desiredValue;
    float offset;
    float prevOffset[OFFSET_COUNT];
    float control;

signals:
    void sendControlAndOffset(float control, float offset);

private:
    void timeout(void);
    void calculateOffset(void);
    void calculatePID(void);

    QTimer timer;

    QSplineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    double step;
    double x;
    double y;

};

#endif // PID_H
