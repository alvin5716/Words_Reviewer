#include "appoption.h"
#include <QDebug>

AppOption::AppOption():
    position_x(0), position_y(0),
    order(OrderChronological),
    interval(20),
    playing(true),
    staying_on_top(false),
    lastword(0)
{

}

AppOption::OrderMethod AppOption::intToOrderMethod(int i) {
    switch (i) {
    case 0:
        return OrderChronological;
    case 1:
        return OrderRandom;
    default:
        qDebug() << "invalid order method";
        return OrderChronological;
    }
}
AppOption::OrderMethod AppOption::nextMethod(OrderMethod old_method) {
    int order_int = static_cast<int>(old_method);
    if(++order_int >= AppOption::OrderMethodCount)
        order_int = 0;
    return AppOption::intToOrderMethod(order_int);
}
