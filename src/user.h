//
// Created by 18163 on 2021/1/27.
//

#ifndef BOOKSTORE_2021_MAIN_USER_H
#define BOOKSTORE_2021_MAIN_USER_H
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;
class user{
public:
    char user_id[31] = {'\0'};
    char name[31] = {'\0'};

public:
    char passwd[31] = {'\0'};
    int select = 0;
    int priority = 0;

    user();

    user(const string &USER_ID , const string &PASSWD , const int &PRIOR , const string &NAME , const int &SELECT);

    user(const user &other);

    user &operator=(const user &other);

    friend class user_system;
};

class user_system{
public:
    fstream scanner;

    void numberplus();

    int getnumber();

public:
    user_system() = default;

    ~user_system();

    int offset(const string &USER_ID);

    void push_back(const user& ex);

    user get_user(const int &offset);

    void change_passwd(const int &offset,const string &new_passwd);

    void open();

    void reopen();
};

class oper_element{
public:
    int order = -1;

    char userid[31]{'\0'};

    char operation[152]{'\0'};

    oper_element() = default;

    oper_element(const string &USERID , const string &OPERATION, int ORDER);

    oper_element &operator=(const oper_element &other);

    bool operator<(const oper_element &other);

    void show();

    friend class oper_block;
};

class oper_block{
    oper_element array[2*320+4];
    int cur_size = 0;
    int prev = -1;
    int next = -1;

    void insert(const oper_element &ele);

    void show_all();
public:
    oper_block() = default;

    oper_block(int PREV, int NEXT);

    friend class oper_linklist;
};

class oper_linklist{
    fstream scanner;

    int total_num = 0;

    void split_block();

    void numberplus();

    int getnumber();
public:
    oper_linklist() = default;

    ~oper_linklist();

    void add_operation(const string &USERID, const string &OPER);

    void show_all();

    void show(const string &USERID);

    void real_show(const string &USERID, int offset);

    void open();

    void reopen();
};


#endif //BOOKSTORE_2021_MAIN_USER_H
