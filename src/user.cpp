//
// Created by 18163 on 2021/1/27.
//
#include "user.h"
user::user() {
    memset(user_id,'\0', sizeof(user_id));
    memset(name,'\0', sizeof(name));
    memset(passwd,'\0', sizeof(passwd));
    select = 0;
    priority = 0;
}

user::user(const string &USER_ID, const string &PASSWD, const int &PRIOR, const string &NAME, const int &SELECT) {
    for(int i = 0 ; i < sizeof(user_id) ; ++i)user_id[i] = USER_ID[i];
    for(int i = 0 ; i < sizeof(passwd) ;  ++i)passwd[i] = PASSWD[i];
    priority = PRIOR;
    for(int i = 0 ; i < sizeof(name) ; ++i)name[i] = NAME[i];
    select = SELECT;
}

user::user(const user &other) {
    if (this == &other)return;
    for(int i = 0 ; i < sizeof(user_id); ++i)user_id[i] = other.user_id[i];
    for(int i = 0 ; i < sizeof(passwd) ; ++i)passwd[i] = other.passwd[i];
    priority = other.priority;
    for(int i = 0 ; i < sizeof(name) ; ++i)name[i] = other.name[i];
    select = other.select;
}

user &user::operator=(const user &other) {
    if (this == &other)return *this;
    for(int i = 0 ; i < sizeof(user_id); ++i)user_id[i] = other.user_id[i];
    for(int i = 0 ; i < sizeof(passwd) ; ++i)passwd[i] = other.passwd[i];
    priority = other.priority;
    for(int i = 0 ; i < sizeof(name) ; ++i)name[i] = other.name[i];
    select = other.select;
    return *this;
}

user_system::~user_system(){
    scanner.close();
}

int user_system::offset(const string &USER_ID) {
    int pointer = sizeof(int);
    user a;
    scanner.seekg(pointer);
    while (pointer != getnumber()* sizeof(a)+ sizeof(int)){
        scanner.seekg(pointer);
        scanner.read(reinterpret_cast<char *>(&a), sizeof(a));
        if (a.user_id == USER_ID) {
            return pointer;
        }
        pointer += sizeof(a);
    }
    return -1;
}

void user_system::push_back(const user& ex) {
    reopen();
    user use(ex);//拷贝初始化
    scanner.seekp(0,ios::end);
    scanner.write(reinterpret_cast<char *>(&use), sizeof(use));
    numberplus();
}

void user_system::numberplus() {
    int number;
    scanner.seekg(0);
    scanner.read(reinterpret_cast<char *>(&number), sizeof(number));
    ++number;
    scanner.seekp(0);
    scanner.write(reinterpret_cast<char *>(&number), sizeof(number));
    reopen();
}

int user_system::getnumber() {
    int number;
    scanner.seekg(0);
    scanner.read(reinterpret_cast<char *>(&number), sizeof(number));
    return number;
}

user user_system::get_user(const int &offset) {
    user answer;
    scanner.seekg(offset);
    scanner.read(reinterpret_cast<char *>(&answer), sizeof(answer));
    return answer;
}

void user_system::change_passwd(const int &offset, const string &new_passwd) {
    user a;
    scanner.seekg(offset);
    scanner.read(reinterpret_cast<char *>(&a), sizeof(a));
    for(int i = 0 ; i < sizeof(a.passwd) ; ++i)a.passwd[i] = new_passwd[i];
    scanner.seekp(offset);
    scanner.write(reinterpret_cast<char *>(&a), sizeof(a));
    reopen();
}

void user_system::open() {
    scanner.open("user");
}

void user_system::reopen() {
    scanner.close();
    scanner.open("user");
}

oper_element::oper_element(const string &USERID, const string &OPERATION, int ORDER) {
    for(int i = 0 ; i < sizeof(userid) ; ++i)userid[i] = USERID[i];
    for(int i = 0 ; i < sizeof(operation) ; ++i)operation[i] = OPERATION[i];
    order = ORDER;
}

oper_element &oper_element::operator=(const oper_element &other) {
    if (this == &other)return *this;
    for(int i = 0 ; i < sizeof(userid) ; ++i)userid[i] = other.userid[i];
    for(int i = 0 ; i < sizeof(operation) ; ++i)operation[i] = other.operation[i];
    order = other.order;
    return *this;
}

bool oper_element::operator<(const oper_element &other) {
    string useri = userid;
    string ouseri =  other.userid;
    if (useri < ouseri)return true;
    if (useri > ouseri)return false;
    if (order < other.order)return true;
    return false;
}

void oper_element::show() {
    cout<<userid<<" "<<operation<<endl;
}

oper_block::oper_block(int PREV, int NEXT) {
    prev = PREV;
    next = NEXT;
}

void oper_block::insert(const oper_element &ele) {
    int cnt;
    for(cnt = 0 ; cnt < cur_size && array[cnt] < ele ; ++cnt);
    for(int i = cur_size-1 ; i >= cnt ; --i)
        array[i+1] = array[i];
    array[cnt] = ele;
    ++cur_size;
}

void oper_block::show_all() {
    for(int i = 0 ; i < cur_size ; ++i)
        array[i].show();
}

oper_linklist::~oper_linklist() {
    scanner.close();
}

void oper_linklist::add_operation(const string &USERID, const string &OPER) {
    int number = total_num + 1;
    oper_element x(USERID,OPER,number);
    oper_block tmp;
    if (getnumber() == 0){
        numberplus();
        oper_block a(-1,-1);
        a.insert(x); ++total_num;
        scanner.seekp(sizeof(int));
        scanner.write(reinterpret_cast<char *>(&a), sizeof(a));
        reopen();
        return;
    }
    int save = -1;
    int off = sizeof(int);
    while (off != -1){
        save = off;
        scanner.seekg(off);
        scanner.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        if (x < tmp.array[0]){
            if (off == sizeof(int)){
                tmp.insert(x); ++total_num;
                scanner.seekp(off);
                scanner.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
                open();
                split_block();
                return;
            }
            off = tmp.prev;
            scanner.seekg(off);
            scanner.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            tmp.insert(x); ++total_num;
            scanner.seekp(off);
            scanner.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            reopen();
            split_block();
            return;
        }
        off = tmp.next;
    }
    tmp.insert(x); ++total_num;
    scanner.seekp(save);
    scanner.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    reopen();
    split_block();
}

void oper_linklist::split_block() {
    if (getnumber() == 0)return;
    int number;
    int off = sizeof(int);
    oper_block a, b, c;\
    while (off != -1){
        scanner.seekg(off);
        scanner.read(reinterpret_cast<char *>(&a), sizeof(a));
        if (a.cur_size >= 2*320){
            int i ;
            for(i = a.cur_size-1 ; i >= a.cur_size-320 ; --i)
                b.array[i-a.cur_size+320] = a.array[i];
            b.prev = off;
            b.cur_size = 320;
            b.next = a.next;
            a.cur_size -= 320;
            scanner.seekp(0,ios::end);
            scanner.write(reinterpret_cast<char *>(&b), sizeof(b));
            reopen();
            numberplus();
            number = getnumber();
            a.next = sizeof(int) + (number-1)* sizeof(oper_block);
            scanner.seekp(b.prev);
            scanner.write(reinterpret_cast<char *>(&a), sizeof(a));
            reopen();
            if (b.next != -1){
                scanner.seekg(b.next);
                scanner.read(reinterpret_cast<char *>(&c), sizeof(c));
                c.prev = a.next;
                scanner.seekp(b.next);
                scanner.write(reinterpret_cast<char *>(&c), sizeof(c));
                reopen();
            }
            off = b.next;
            continue;
        }
        off = a.next;
    }
}

void oper_linklist::show_all() {
    if (getnumber() == 0){
        cout<<endl;return;
    }
    oper_block blk;
    int pointer = sizeof(int);
    while (pointer != getnumber()* sizeof(oper_block) + sizeof(int) ){
        scanner.seekg(pointer);
        scanner.read(reinterpret_cast<char *>(&blk), sizeof(blk));
        blk.show_all();
        pointer += sizeof(oper_block);
    }
}

void oper_linklist::show(const string &USERID) {
    oper_block blk;
    int offset = sizeof(int);
    int save;
    while (offset != -1){
        save = offset;
        scanner.seekg(offset);
        scanner.read(reinterpret_cast<char *>(&blk), sizeof(blk));
        if (USERID <= blk.array[0].userid){
            if (USERID < blk.array[0].userid){
                if (offset == sizeof(int)){
                    cout<<endl;return;
                }
                save = blk.prev;
                real_show(USERID,save);
                return;
            }
            if (USERID == blk.array[0].userid){
                if (offset == sizeof(int)){
                    save = offset;
                    real_show(USERID,save);
                    return;
                }
                save = blk.prev;
                real_show(USERID,save);
                return;
            }
        }
        offset = blk.next;
    }
    real_show(USERID,save);
}

void oper_linklist::real_show(const string &USERID, int offset) {
    bool flag = false;
    oper_block blk;
    int off;
    int pointer = offset;
    while (pointer != -1){
        scanner.seekg(pointer);
        scanner.read(reinterpret_cast<char *>(&blk), sizeof(blk));
        if (USERID < blk.array[0].userid)break;
        for(int i = 0 ; i < blk.cur_size ; ++i){
            if (USERID == blk.array[i].userid){
                flag = true;
                blk.array[i].show();
            }
        }
        pointer = blk.next;
    }
    if (!flag)cout<<endl;
}

void oper_linklist::numberplus() {
    int number;
    scanner.seekg(0);
    scanner.read(reinterpret_cast<char *>(&number), sizeof(number));
    ++number;
    scanner.seekp(0);
    scanner.write(reinterpret_cast<char *>(&number), sizeof(number));
    reopen();
}

int oper_linklist::getnumber() {
    int number;
    scanner.seekg(0);
    scanner.read(reinterpret_cast<char *>(&number), sizeof(number));
    return number;
}

void oper_linklist::open() {
    scanner.open("operation_log");
}

void oper_linklist::reopen() {
    scanner.close();
    scanner.open("operation_log");
}