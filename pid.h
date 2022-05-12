#ifndef PID_H
#define PID_H

#include <QObject>
#include <QTimer>

#define OFFSET_COUNT 2
#define DELAY 1000

class PID : public QObject
{
    Q_OBJECT
public:
    explicit PID(QObject *parent = nullptr);
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

};

#endif // PID_H
