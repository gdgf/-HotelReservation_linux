#pragma once
#ifndef UTILS
#define UTILS

#include <string>
#include <vector>
#include <stdlib.h>  
#include <stdio.h>
#include <sys/shm.h>  
#include <sys/sem.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "Date.h"

using namespace std;
#define KEY_SHAREDMEM_COUNT 11100     //共享内存
#define KEY_SEM_WAITING     11300    //房间信号量
#define KEY_SEM_CUSTOMERS   10200     //客户信号量
#define memory_segment_size 40


extern void create_sem(int key, int number);       //创建信号量
extern int load_sem(int key);
extern void create_sharedmem(int key, int number); //共享内存
extern void *load_mem(int key);
extern int P(int semid, int which);
extern int V(int semid, int which);
extern string getTime();

//预约类
class reserve
{
public:
    string name;
	int days,roomumber;
	Date startdate;
	reserve(int roomumber,Date startdate, int days, string name)
	{
		this->roomumber = roomumber;
		this->days = days;
		this->startdate =startdate;
		this->name = name;
	}
};


//取消
class cancel
{
public:
    string name;
    int days,roomumber;
    Date startdate;
    
    cancel(int roomumber,Date startdate, int days, string name)
    {
        this->roomumber = roomumber;
        this->days = days;
        this->startdate =startdate;
        this->name = name;
    }
};

//连续预约
class reserveblock
{
public:
    int num,days,first_roomnumber;
    Date startdate;
    string name;
    reserveblock(int num ,int first_roomnumber,Date startdate, int days, string name)
    {
        this->num = num;
        this->first_roomnumber =first_roomnumber ;
        this->startdate =startdate;
        this->days=days;
        this->name = name;
    }
};
//连续取消
class cancelblock
{
public:
    int num,days,first_roomnumber;
    Date startdate;
    string name;
    cancelblock(int num ,int first_roomnumber,Date startdate, int days, string name)
    {
        this->num = num;
        this->first_roomnumber =first_roomnumber ;
        this->startdate =startdate;
        this->days=days;
        this->name = name;
    }
};

//全部预约
class reserveany
{
public:
    int num;
    string name;
    int days;
    Date startdate;
    reserveany(int num,Date startdate, int days, string name)
    {
        this->num=num;
        this->days = days;
        this->startdate =startdate;
        this->name = name;
    }
};

//取消
class cancelany
{
public:
    int num;
    string name;
    int days;
    Date startdate;
    cancelany(int num,Date startdate, int days, string name)
    {
        this->num=num;
        this->days = days;
        this->startdate =startdate;
        this->name = name;
    }
};

//取消
class check
{
public:
    string name;
    check(string name)
    {
        this->name=name;
    }
};

//存操作时间
class op_time{
public:
    int time1,time2,time3;
    op_time(){}
    op_time(int time1,int time2,int time3){
        this->time1=time1;  //预约
        this->time2=time2;  //取消
        this->time3=time3;  //检查
    }
};
#endif // Utils
