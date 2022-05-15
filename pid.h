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

typedef enum
{
    DATA_TYPE_CONTROL = 0,
    DATA_TYPE_SETPOINT,
    DATA_TYPE_COUNT,
}dataType;

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

    int   setpoint;
    float offset;
    float prevOffset[OFFSET_COUNT];
    float control;

public slots:
    void setKp(float value);
    void setKi(float value);
    void setTd(float value);
    void setTs(int value);
    void setSetpoint(int value);

signals:
    void sendControlAndOffset(float control, float offset);

private:
    void initDataLines(void);
    void timeout(void);
    void calculateOffset(void);
    void calculatePID(void);

    QList<QSplineSeries*> series;

    QTimer timer;
    QValueAxis *axisX;
    QValueAxis *axisY;

    double step;
    double x;
    double y;

};

#endif // PID_H
