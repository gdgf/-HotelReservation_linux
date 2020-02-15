#ifndef LINUX_HOTEL_CUSTOMER_H
#define LINUX_HOTEL_CUSTOMER_H

#include "hotel.h"
#include "utils.h"
#include <vector>
#include <sys/types.h>
class customer {
    int idx_reverse, idx_cancel,idx_reserveblock,idx_cancelblock,
        idx_reserveany,idx_cancelany,idx_check,idx_optime;
    vector<reserve> my_reverse;
    vector<cancel> my_cancel;
    vector<reserveblock> my_reverseblock;
    vector<cancelblock> my_cancelblock;
    vector<reserveany> my_reverseany;
    vector<cancelany> my_cancelany;
    vector<check> my_check;
    op_time my_optime;
    int id;
    hotel *H;
    int *content;
     //    *rooms_pointers;
    int sem_mutex_room,sem_mutex_customer;

public:
    customer(hotel &H,int id);
    void reserve_room(reserve);
    void cancel_room(cancel);
    void reservebolck_room(reserveblock);
    void cancelblock_room(cancelblock);
    void reserveany_room(reserveany);
    void cancelany_room(cancelany);
    void check_room(check);
    void start();
};

#endif //LINUX_HOTEL_CUSTOMER_H
