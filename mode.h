#ifndef MODE_H
#define MODE_H

#include <vector>
#include <twofish.h>

using namespace std;

enum MODE { ECB, CBC, CFB };

class IMode
{
public:
    virtual ~IMode() {}
    virtual vector<uint8_t> encrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish) = 0;
    virtual vector<uint8_t> decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish) = 0;
};

class ECB_MODE : public IMode
{
public:
    vector<uint8_t> encrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish) override;
    vector<uint8_t> decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish) override;
};

class CBC_MODE : public IMode
{
private:
    vector<uint8_t> initvec;
public:
    CBC_MODE(vector<uint8_t>&& initvec):initvec(initvec){};
    vector<uint8_t> encrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish) override;
    vector<uint8_t> decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish) override;
};

class CFB_MODE : public IMode
{
private:
  vector<uint8_t> initvec;
public:
    CFB_MODE(vector<uint8_t>&& initvec):initvec(initvec){};
    vector<uint8_t> encrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish) override;
    vector<uint8_t> decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish) override;
};


class Encryptor
{
  public:
    Encryptor( IMode* comp): mode(comp) {twofish=new Twofish;}
   ~Encryptor() { delete mode; delete twofish;}
    vector<uint8_t> encrypt( vector<uint8_t>&& key, vector<uint8_t>&& text)
    {
        return mode->encrypt(move(key), move(text), twofish);
    }

    vector<uint8_t> decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text)
    {
        return mode->encrypt(move(key), move(text), twofish);
    }
  private:
    IMode* mode;
    Twofish* twofish;
};

#endif // MODE_H
