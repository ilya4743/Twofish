#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "mode.h"
#include "FileManager.h"
#include "myexception.h"

class Controller
{
private:
    FileManager* fileManager;
    Twofish* twofish;
    Controller(const Controller& controller);
    Controller& operator=(Controller other);
    inline vector<uint8_t>keyToUint8_t(string &&key, bool isKeyHex);
    inline vector<uint8_t>PTToUint8_t(string &&text, bool isPTHex);

public:
    Controller(Twofish * _twofish):twofish(_twofish){fileManager=new FileManager;}
    ~Controller(){delete fileManager;}
    string encrypt(string &&key, string &&text, string &&initvec, bool isKeyHex, bool isPTHex, int mode);
    string decrypt(string &&key, string &&text, string &&initvec, bool isKeyHex, bool isPTHex, int mode);
    void safeToFile(string &&filename, string &&out);
    string loadFromFile(string &&filename);
};

#endif // CONTROLLER_H
