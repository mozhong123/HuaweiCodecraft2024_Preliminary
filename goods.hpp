#ifndef __GOODS_H__
#define __GOODS_H__
class goods
{
private:
    /* data */
public:
    goods(double,double,int,double);
    ~goods();
    double x,y;
    int value;
    double time;
};

goods::goods(double xx,double yy,int _value,double _time):
x(xx),y(yy),value(_value),time(_time){}
goods::~goods()
{
}
#endif