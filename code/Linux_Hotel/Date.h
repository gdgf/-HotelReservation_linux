#ifndef DATE_H_INCLUDED
#define DATE_H_INCLUDED

#include <iostream>
using namespace std;

class Date
{
private:
    int year,month,day;
public:
    Date(){}
    Date(int year,int month,int day);
    Date(const Date &d):year(d.year),month(d.month),day(d.day){}
    ~Date(){}

    //得到年月日
    int getYear()const{return year;}
    int getMonth()const{return month;}
    int getDay()const{return day;}

    void setDate(int year,int month,int day);   //设置日期
    bool isLeapYear(int year)const;   //判断是否为闰年
    int daysOfMonth(int year,int month)const;   //得到某个月的天数
    void show()const;   //显示日期
    Date changeDays(const int days)const;   //改变日期
    int distance(const Date &d)const;   //计算两个日期之间的天数

    /*重载运算符*/

    //日期加上days个天数
    friend Date operator +(const Date &d,const int days);
    friend Date operator +(const int days,const Date &d);
    Date& operator +=(int days);
    //日期自增一天
    Date& operator ++();
    Date operator ++(int);

    //日期减去days个天数
    friend Date operator -(const Date &d,const int days);
    friend int operator -(const Date &d1,const Date &d2);
    Date& operator -=(int days);
    //日期自减一天
    Date& operator --();
    Date operator --(int);

    //日期大小比较
    friend bool operator >(const Date &d1,const Date &d2);
    friend bool operator >=(const Date &d1,const Date &d2);
    friend bool operator <(const Date &d1,const Date &d2);
    friend bool operator <=(const Date &d1,const Date &d2);
    friend bool operator ==(const Date &d1,const Date &d2);
    friend bool operator !=(const Date &d1,const Date &d2);

    //输出，输入日期
    friend ostream& operator <<(ostream &out,const Date &d);
    friend istream& operator >>(istream &in,Date &d);
};

#endif // DATE_H_INCLUDED
