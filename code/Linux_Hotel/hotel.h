#ifndef LINUX_HOTEL_HOTEL_H
#define LINUX_HOTEL_HOTEL_H

#include <vector>
#include <iostream>
#include "utils.h"

using namespace std;

class Room{
    int * room;
public:
    Room(){
        room = new int[memory_segment_size];   //一个房间是否被占用的大小
    }
    Room(void* addr)   //addr是起始地址
    {
        room = (int*)addr;     //一个房间内存开始的地方
        //初值都设置为0；
        for(int i=0;i<memory_segment_size;i++){
            room[i]=0;   //房间每一天都没有人
        }
        //room[0] = '\0';
    }
};

class hotel {
public:
    //int sid;//信号量，用于控制房间
    //vector<Room*> room;//房间们

    vector<vector<reserve> > reserve_op; //<id,<reserve,reserve...>>  //id 对应用 户， 后面跟这个用户所有的预约操作
    vector<vector<cancel> > cancel_op;  
    vector<vector<reserveblock> > reserveblock_op;
    vector<vector<cancelblock> > cancelblock_op;
    vector<vector<cancelany> > cancelany_op;
    vector<vector<reserveany> > reserveany_op;
    vector<vector<check> > check_op;
    vector<op_time>  op_time_op; //操作时间； <id, op_time> //id是对用， 后面是他的操作时间  //将这些全都保存
    

    int customers_sum; //顾客的数量
    int rooms_sum;     //房间数量
    int memory_size=memory_segment_size;  //每个房间对应的日期
    int opsum[7][7];  //每个用户每种操作的次数 

    hotel(){
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                opsum[i][j]=0;
            }
        }
    };
    //hotel(int s, void* addr);
    void reset_env_ipc(); //释放缓冲区
    void read();  //读数据
    void setup_ipc();  //初始化
    void print_memory(); //打印缓冲区中的东西
    void print(); //打印保存的信息
};

#endif //LINUX_HOTEL_HOTEL_H
