#ifndef __GoodsS_H__
#define __GoodsS_H__
class Goods
{
private:
    /* data */
public:
    Goods(){}
    Goods(int,int,int,int);
    ~Goods();
    int x,y;
    int value;
    int time;//生命剩余
    int chosed;//0表示被选择，1表示未被选择。
};

Goods::Goods(int xx,int yy,int _value,int _time):
x(xx),y(yy),value(_value),time(_time){}
Goods::~Goods()
{
}

#endif