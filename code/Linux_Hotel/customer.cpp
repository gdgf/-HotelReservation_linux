#include "customer.h"

customer::customer(hotel &H, int id){

    this->id = id;
    this->H = &H;
    my_reverse = H.reserve_op[id];

    my_cancel= H.cancel_op[id];
    my_reverseblock=H.reserveblock_op[id];
    my_cancelblock=H.cancelblock_op[id];
    my_reverseany=H.reserveany_op[id];
    my_cancelany=H.cancelany_op[id];
    my_check=H.check_op[id];
    my_optime= H.op_time_op[id];

    content=(int*)load_mem(KEY_SHAREDMEM_COUNT);
    sem_mutex_room=load_sem(KEY_SEM_WAITING);  //房间信号量集合
    sem_mutex_customer=load_sem(KEY_SEM_CUSTOMERS); //客户信号量

    idx_reverse=idx_cancel=idx_reserveblock=idx_cancelblock
               =idx_reserveany=idx_cancelany=idx_check=idx_optime=0;

}

void customer::reserve_room(reserve reserve_msg) {

    //cou<<"  "<<reserve_msg.roomumber<<endl;
    P(sem_mutex_customer,id);
    P(sem_mutex_room,reserve_msg.roomumber);
    int j=0;
    j=(reserve_msg.roomumber-1)*H->memory_size; //存储的初始地址.这个房间的订阅信息的存储其实位置
    Date date(2019,1,1);
    int start=reserve_msg.startdate-date;     //日期变成下标
    //cout<<"roomnumber"<<reserve_msg.roomumber<<"  起始地址j"<<j<<endl;
    //cout<<"id"<<id<<"  time "<<start<<endl;
    //cout<<"days "<<reserve_msg.days<<endl;
    int testlog=1;
    for(int i=0;i<reserve_msg.days;i++){   //将这写房间房间
        if(content[j+start+i]!=0){
            cout<<"顾客"<<id+1<<":房间"<<reserve_msg.roomumber<<"在这个时间段内已经被预约了，不能预约!"<<endl;
            testlog=0;
            break;
        }

        content[j+start+i]=id+1;  //用1表示
    }
    if(testlog) {
        cout << "顾客" << id + 1 << "预约了房间" << reserve_msg.roomumber << endl;
        cout << "期限:" << reserve_msg.startdate << "~" << reserve_msg.days + reserve_msg.startdate;
    }
    //cout<<my_optime.time1<<endl;
    sleep(my_optime.time1);  //预约时间
    V(sem_mutex_customer,id);
    V(sem_mutex_room,reserve_msg.roomumber);
}
void customer::cancel_room(cancel cancel_msg) {
    //int
    P(sem_mutex_customer,id);
    P(sem_mutex_room,cancel_msg.roomumber);
    int j=0;
    j=(cancel_msg.roomumber-1)*H->memory_size; //存储的初始地址.这个房间的订阅信息的存储其实位置
    Date date(2019,1,1);
    int start=cancel_msg.startdate-date; //日期变成下标
    //cout<<"time "<<start<<endl;
    //cout<<"days "<<cancel_msg.days<<endl;
    int testlog=1;
    for(int i=0;i<cancel_msg.days;i++){   //将这写房间房间
        if(content[j+i+start]==0){
            cout<<"顾客"<<id+1<<":你还没有预约房间"<<cancel_msg.roomumber<<"，因此不能取消预约!"<<endl;
            testlog=0;
            break;
        }else{
            content[i+start]=0;  //用0表示
        }
    }
    if(testlog) {
        cout << "顾客" << id + 1 << "取消预约了房间" << cancel_msg.roomumber << endl;
        cout << "期限:" << cancel_msg.startdate << "~" << cancel_msg.days + cancel_msg.startdate;
    }
    //cout<<my_optime.time1<<endl;
    sleep(my_optime.time1);  //预约时间
    V(sem_mutex_customer,id);
    V(sem_mutex_room,cancel_msg.roomumber);
}

void customer::reservebolck_room(reserveblock reserveblock_msg) {
    //int
    P(sem_mutex_customer,id);
    P(sem_mutex_room,reserveblock_msg.first_roomnumber);
    int j=0;

    Date date(2019,1,1);
    int start=reserveblock_msg.startdate-date;    //日期变成下标
    int x=reserveblock_msg.first_roomnumber;    //第一个房间号
    int m=0;
    int testlog=1;
    m=(x-1)*H->memory_size;                     //第一个房间的初始下标记
    for(int i=0;i<reserveblock_msg.num;i++){    //对num个房间进行操作
        //cout<<"mm"<<m<<endl;
        if(testlog==0){
            cout<<"因此预约失败"<<endl;
            break;
        }
        for(int j=0;j<reserveblock_msg.days;j++){   //预约n天
            if(content[m+start+j]!=0){
                testlog=0;
                cout<<"顾客"<<id+1<<":房间"<<reserveblock_msg.first_roomnumber+i<<"在这个时间段内已经被预约了，不能预约!"<<endl;
                break;
            }
            content[m+start+j]=id+1;
        }
        if(testlog) {
            cout << "顾客" << id + 1 << "预约了房间" << reserveblock_msg.first_roomnumber + i << endl;
            cout << "期限:" << reserveblock_msg.startdate << "~" << reserveblock_msg.days + reserveblock_msg.startdate;
            m = m + H->memory_size;
        }
    }
    //cout<<my_optime.time1<<endl;
    sleep(my_optime.time1);    //预约时间
    V(sem_mutex_customer,id);
    V(sem_mutex_room,reserveblock_msg.first_roomnumber);
}
void customer::cancelblock_room(cancelblock cancelblock_msg) {
    P(sem_mutex_customer,id);
    P(sem_mutex_room,cancelblock_msg.first_roomnumber);
    int j=0;
    Date date(2019,1,1);
    int start=cancelblock_msg.startdate-date;    //日期变成下标
    int x=cancelblock_msg.first_roomnumber;     //第一个要取消的房间号
    int m=0;

    int testlog=1;
    m=(x-1)*H->memory_size;                    //第一个房间的初始下标记
    for(int i=0;i<cancelblock_msg.num;i++){     //对num个房间进行操作
        //cout<<"mm"<<m<<endl;
        if(testlog==0){
            cout<<"因此取消预约失败"<<endl;
            break;
        }
        for(int j=0;j<cancelblock_msg.days;j++){   //预约n天
            if(content[m+start+j]==0){
                testlog=0;
                cout<<"顾客"<<id+1<<":房间"<<cancelblock_msg.first_roomnumber+i<<"在这个时间段内还没有被预约，不能取消预约!"<<endl;
                break;
            }
            content[m+start+j]=0;
        }
        if(testlog) {
            cout << "顾客" << id + 1 << "取消预约了房间" << cancelblock_msg.first_roomnumber + i << endl;
            cout << "期限:" << cancelblock_msg.startdate << "~" << cancelblock_msg.days + cancelblock_msg.startdate;
            m = m + H->memory_size;
        }
    }
    //cout<<my_optime.time1<<endl;
    sleep(my_optime.time2);    //取消时间
    V(sem_mutex_customer,id);
    V(sem_mutex_room,cancelblock_msg.first_roomnumber);
}

void customer::reserveany_room(reserveany reserveany_msg) {
    P(sem_mutex_customer,id);
    P(sem_mutex_room,reserveany_msg.num);
    int j=0;
    Date date(2019,1,1);
    int start=reserveany_msg.startdate-date;       //日期变成下标

    cout<<"hello1"<<endl;
    int m=0;
    int sum=0; //房间总数
    int testlog=1;
    //m=(x-1)*H->memory_size;                     //第一个房间的初始下标记
    for(int i=0;i<H->rooms_sum;i++){              //试一下每一个房间
        m=i*H->memory_size;                    //第一个房间的初始下标记
        //cout<<"m1"<<m<<endl;
        for(int j=0;j<reserveany_msg.days;j++){
            if(content[m+start+j]!=0){
                testlog=0;
                break;
            }
            content[m+start+j]=id+1;
            testlog=1;   //重新设置为1, 跳过不能预约的那些房间
        }
        if(testlog) {
            cout << "顾客" << id + 1 << "预约了房间" << i+1 << endl;
            cout << "期限:" << reserveany_msg.startdate << "~" << reserveany_msg.days + reserveany_msg.startdate<<endl;
            if(sum==reserveany_msg.num){  //房间数量够了直接退出
                break;
            }
            sum++;
        }
    }
    sleep(my_optime.time1);
    V(sem_mutex_customer,id);
    V(sem_mutex_room,reserveany_msg.num);
}

void customer::cancelany_room(cancelany cancelany_msg) {
    P(sem_mutex_customer,id);
    P(sem_mutex_room,cancelany_msg.num);
    int j=0;
    Date date(2019,1,1);
    int start=cancelany_msg.startdate-date;       //日期变成下标
    cout<<"hello2"<<endl;
    int m=0;
    int sum=0; //房间总数
    int testlog=1;
    //m=(x-1)*H->memory_size;                     //第一个房间的初始下标记
    for(int i=0;i<H->rooms_sum;i++){              //试一下每一个房间
        m=i*H->memory_size;                       //第一个房间的初始下标记
        //cout<<"m1"<<m<<endl;
        for(int j=0;j<cancelany_msg.days;j++){
            if(content[m+start+j]==0){
                testlog=0;
                break;
            }
            content[m+start+j]=0;
            testlog=1;                        //重新设置为1, 跳过不能预约的那些房间
        }
        if(testlog) {
            cout << "顾客" << id + 1 << "取消了房间" << i+1 << endl;
            cout << "期限:" << cancelany_msg.startdate << "~" <<cancelany_msg.days + cancelany_msg.startdate<<endl;
            sum++;
            if(sum==cancelany_msg.num){  //房间数量够了直接退出
                break;
            }

        }
    }
    sleep(my_optime.time2);
    V(sem_mutex_customer,id);
    V(sem_mutex_room,cancelany_msg.num);
}

void customer::check_room(check) {

}
void customer::start() {
    //单个房间预约
    while(idx_reverse<my_reverse.size()){

        //cout<<"idX"<<id<<"idx_reverse"<<idx_reverse<<endl;
        reserve_room(my_reverse[idx_reverse]);
        idx_reverse++;
    }
    //单个房间取消
    while(idx_cancel<my_cancel.size()){

        //cout<<"idX"<<id<<"idx_reverse"<<idx_reverse<<endl;
        cancel_room(my_cancel[idx_cancel]);
        idx_cancel++;
    }
    //指定连续的多个房间预约
    while(idx_reserveblock<my_reverseblock.size()){

        //cout<<"idX"<<id<<"idx_reverse"<<idx_reverse<<endl;
        reservebolck_room(my_reverseblock[idx_reserveblock]);
        idx_reserveblock++;
    }

    //指定连续的多个房间取消预约
    while(idx_cancelblock<my_cancelblock.size()){

        //cout<<"idX"<<id<<"idx_reverse"<<idx_reverse<<endl;
        cancelblock_room(my_cancelblock[idx_cancelblock]);
        idx_cancelblock++;
    }

    //任意个房间预约
    while(idx_reserveany<my_reverseany.size()){

        //cout<<"idX"<<id<<"idx_reverse"<<idx_reverse<<endl;
        reserveany_room(my_reverseany[idx_reserveany]);
        idx_reserveany++;
    }
    //任意个房间取消
    while(idx_cancelany<my_cancelany.size()){

        //cout<<"idX"<<id<<"idx_reverse"<<idx_reverse<<endl;
        cancelany_room(my_cancelany[idx_cancelany]);
        idx_cancelany++;
    }

}

