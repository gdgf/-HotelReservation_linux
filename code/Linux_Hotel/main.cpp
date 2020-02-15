#include <iostream>
#include <sys/wait.h>
#include "hotel.h"
#include "utils.h"
#include "customer.h"
int main(int argc,char *argv[]) {

    if(argc!=2)
    {
        cout<<"应该输入一文件名字,请检查!"<<endl;
        exit(1);
    }
    //string filename;
    freopen(argv[1],"r",stdin);

    hotel H;
    pid_t mypid;
    int customer_number=0;
    H.reset_env_ipc();
    H.read();
    //H.print();
    H.setup_ipc();

    //每个顾客是一个进程
    for(int i = 0; i < H.customers_sum; i++) {

        mypid = fork();
        //cout<<"mypid"<<mypid<<endl;

        if (mypid == 0) {
            customer_number = i;
            //printf("[Info, Timestamp:%s]:  Customer %d is coming\n", getTime().c_str(), customer_number);
            break;
        }


    }
    if (mypid == 0)//child
    {
        //cout<<"____________开始写入表_____________"<<endl;
        customer customers(H,customer_number);
        customers.start();

    }else{

        for (int i = 0; i < H.customers_sum;i++)
        {
            pid_t t= wait(NULL);
        }
        H.print_memory();
    }
    return 0;
}