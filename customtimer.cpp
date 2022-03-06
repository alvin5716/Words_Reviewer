#include "customtimer.h"

CustomTimer::CustomTimer()
    :QTimer()
{

}

void CustomTimer::setIntervalSec(int interval) {
    this->setInterval(interval*1000);
}
