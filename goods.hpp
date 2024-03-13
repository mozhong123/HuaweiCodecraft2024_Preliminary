#ifndef __GoodsS_H__
#define __GoodsS_H__
class Goods
{
private:
    /* data */
public:
    Goods(){}
    Goods(double,double,int,double);
    ~Goods();
    double x,y;
    int value;
    double time;//生命剩余
};

Goods::Goods(double xx,double yy,int _value,double _time):
x(xx),y(yy),value(_value),time(_time){}
Goods::~Goods()
{
}

#endif