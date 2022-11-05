#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "model.h"
#include "FileManager.h"

class Controller
{
private:
    FileManager* fileManager;
    Twofish* twofish;
    Controller(const Controller& controller);
    Controller& operator=(Controller other);

public:
    Controller(Twofish * _twofish):twofish(_twofish){fileManager=new FileManager;}
    string encrypt(string &&key, string &&text);
    string decrypt(string &&key, string &&text);
    ~Controller(){delete fileManager;}
};

#endif // CONTROLLER_H
