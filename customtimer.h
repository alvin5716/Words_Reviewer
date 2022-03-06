#ifndef CUSTOMTIMER_H
#define CUSTOMTIMER_H

#include <QTimer>

class CustomTimer: public QTimer
{
    Q_OBJECT
public:
    CustomTimer();
public slots:
    void setIntervalSec(int interval);
};

#endif // CUSTOMTIMER_H
