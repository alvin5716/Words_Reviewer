#include "customtimer.h"

CustomTimer::CustomTimer()
    :QTimer()
{

}

void CustomTimer::restartWithInterval(int interval) {
    this->stop();
    this->setInterval(interval*1000);
    this->start();
}
