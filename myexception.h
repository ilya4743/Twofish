#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H
#include<iostream>

class myexception : public std::exception
{
private:
    std::string msg;
public:
    myexception():std::exception(){}
    myexception(std::string& msg):std::exception(),msg(msg){}
    myexception(const char* cmsg):std::exception(),msg(cmsg){}
    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override{return this->msg.c_str();}
};

#endif // MYEXCEPTION_H
