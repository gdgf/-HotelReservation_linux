#include "hotel.h"
#include "utils.h"
void hotel::reset_env_ipc() {
    system("sudo ipcrm -a");
}

void hotel::read() {
  cout<<"—————————————————开始读数据——————————————————"<<endl;
    vector<int> roomid;
    cin>>rooms_sum;  //房间总数，默认从1开始顺延
    //cout<<rooms_sum<<endl;
    for(int i=0;i<rooms_sum;i++){
        int id;
        cin>>id;
      //  cout<<id<<endl;
        roomid.push_back(id);
    }
    cin>>customers_sum;   //顾客总数
    //cout<<customers_sum<<endl;
    
    for (int i = 0; i < customers_sum; i++)
    {
        cout<<"顾客id"<<i<<"操作读取"<<endl;

        vector<reserve>* tmp_reserve = new vector<reserve>;  //当前用户的所有预约操作信息保存
        vector<cancel>* tmp_cancel = new vector<cancel>;
        vector<reserveblock>* tmp_reserveblock = new vector<reserveblock>;
        vector<cancelblock>* tmp_cancelblock = new vector<cancelblock>;
        vector<reserveany>* tmp_reserveany = new vector<reserveany>;
        vector<cancelany>* tmp_cancelany = new vector<cancelany>;
        vector<check>* tmp_check = new vector<check>;
        
        
        string op, log;
        string name;
        int roomumber,first_roomnumber,days,num;
        //Date startdate;
        cin>>log;    //操作
        if(log=="customer"){   //表示开始了
             
            int year,month,day;
            int time1,time2,time3;
            cin>>time1>>time2>>time3;
            cout<<"读取"<<i<<"的操作时间"<<endl;
            printf("%d %d %d ",time1,time2,time3);
            //cout<<time1<<time2+" "<<time3+" "<<endl;
            op_time_op.push_back(op_time(time1,time2,time3));  //每个人操作的时间

            while (true)
            {

                cin>>op;           //操作
                cout<<op<<endl;
                if(op=="end."){
                    break;
                }
                else if (op == "reserve")
                {
                    opsum[i][0]++;
                    cin >> roomumber>>year>>month>>day>>days>>name;
                    Date startdate(year,month,day);
                    startdate.show();
                    tmp_reserve->push_back(reserve(roomumber,startdate, days,name));
                }
                else if (op == "cancel")
                {
                    opsum[i][1]++;
                    cin >> roomumber >>year>>month>>day >> days>>name;
                    Date startdate(year,month,day);
                    startdate.show();
                    tmp_cancel->push_back( cancel(roomumber,startdate, days, name));
                }else if (op == "reserveblock")
                {
                    opsum[i][2]++;
                    cin >> num>>first_roomnumber>>year>>month>>day>>days>>name;
                    Date startdate(year,month,day);
                    startdate.show();
                    //cin >> num>>first_roomnumber>> startdate >> days>>name;
                    tmp_reserveblock->push_back(reserveblock(num ,first_roomnumber,startdate, days, name));
                }else if (op == "cancelblock")
                {
                    opsum[i][3]++;
                    cin >> num>>first_roomnumber>>year>>month>>day>>days>>name;
                    Date startdate(year,month,day);
                    startdate.show();
                    //cin >> num>>first_roomnumber>> startdate >> days>>name;
                    tmp_cancelblock->push_back(cancelblock(num ,first_roomnumber,startdate, days, name));
                }else if (op == "reserveany")
                {
                    opsum[i][4]++;
                    cin >> num>>year>>month>>day>>days>>name;
                    Date startdate(year,month,day);
                    startdate.show();
                    //cin >> num >> startdate >>days>>name;
                    tmp_reserveany->push_back(reserveany(num,startdate,days, name));
                }else if (op == "cancelany")
                {
                    opsum[i][5]++;
                    cout<<"hren1 "<<endl;
                    cin >> num>>year>>month>>day>>days>>name;
                    Date startdate(year,month,day);
                    startdate.show();
                    cout<<"hren "<<endl;
                    tmp_cancelany->push_back(cancelany(num,startdate,days, name));

                }else if (op == "check")
                {
                    opsum[i][6]++;
                    cin >> name;
                    tmp_check->push_back(check(name));
                }
            }
        }
        reserve_op.push_back(*tmp_reserve);
        cancel_op.push_back(*tmp_cancel);
        reserveblock_op.push_back(*tmp_reserveblock);
        cancelblock_op.push_back(*tmp_cancelblock);
        reserveany_op.push_back(*tmp_reserveany);
        cancelany_op.push_back(*tmp_cancelany);
        check_op.push_back(*tmp_check);
       // op_time_op.push_back(*tmp_op_time);
    }
}



void hotel::print_memory() {
    int *content = (int *)load_mem(KEY_SHAREDMEM_COUNT);
    printf("*************************************************\n");
        printf("[Info]:             Hotel      ReserveStatus\n");
    for(int i=0;i<rooms_sum;i++){
        //cout<<"test4444"<<endl;
        //cout<<memory_segment_size<<endl;
        printf("[Detail]:           room No.%d:", i+1);
        for(int j=0;j<memory_segment_size;j++){
            /*
            if (content[i*memory_segment_size + j] == 0)
            {
                break;
            }
             */
            printf("%d", content[i*memory_segment_size + j]);
            //cout<<"0";
            //printf("%d",);
            //cout<<
            //cout<<"jornerg";
        }
        cout<<endl;
    }

}

void hotel::print(){
    cout<<"各自操作的次数"<<endl;

    for(int i=0;i<customers_sum;i++){
        for(int j=0;j<7;j++){
            cout<<opsum[i][j]<<" ";
        }
        cout<<endl;
    }

    cout<<" __________操作信息________"<<endl;
    //reserve_op
    int j=0;
    cout<<customers_sum<<" "<<rooms_sum<<endl;
    for(int i=0;i<customers_sum;i++){
        for(int j=0;j<7;j++)
          for(int m=0;m<opsum[i][j];m++)
            cout<<reserve_op[i][m].roomumber<<" "<<reserve_op[i][m].days<<" "<<reserve_op[i][m].name<<" "<<reserve_op[i][m].startdate<<endl;
        //cout<<op_time_op[i][j].time1<<" "<<op_time_op[i][j].time2<<" "<<op_time_op[i][j].time3<<endl;
    }
}

void hotel::setup_ipc() {
    //开一块内存  731× 房间的数量
    create_sharedmem(KEY_SHAREDMEM_COUNT , sizeof(int)*rooms_sum*memory_segment_size);
    create_sem(KEY_SEM_WAITING, rooms_sum); //每个房间的信号量
    create_sem(KEY_SEM_CUSTOMERS, customers_sum); //每个房间的信号量
    int* content=(int*)load_mem(KEY_SHAREDMEM_COUNT);
    for(int i=0;i<memory_segment_size*rooms_sum;i++){
        //cout<<"gjeognnh"<<i<<" ";
        content[i]=0;
    } //初始化为0；

}

