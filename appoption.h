#ifndef APPOPTION_H
#define APPOPTION_H


class AppOption
{
public:
    AppOption();
    enum OrderMethod{OrderChronological,OrderRandom};
    static const int OrderMethodCount = 2;
    static OrderMethod intToOrderMethod(int i);
    static OrderMethod nextMethod(OrderMethod old_method);
    int position_x, position_y;
    OrderMethod order;
    int interval;
    bool playing;
    bool staying_on_top;
    int lastword;
};

#endif // APPOPTION_H
