#include "controller.h"

static const char hex_table[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

string hex_to_string(vector<uint8_t> &&aInStr)
{
    string hex_str;
    hex_str.reserve(32);
    for (int i = 0; i < aInStr.size(); ++i)
    {
        hex_str.append(&hex_table[(aInStr[i] & 0xF0) >> 4], 1);
        hex_str.append(&hex_table[aInStr[i] & 0xF], 1);
    }
    return hex_str;
}

inline vector<uint8_t> string_to_hex(string&& in)
{
    vector<uint8_t> out;
    out.reserve(in.size()/2);
    int j=0;
    out.reserve(in.size());
    for(int i=1; i<in.size(); i+=2)
    {
        if ((in[i-1] >= '0') && (in[i-1] <= '9'))
            in[i-1] = in[i-1] - '0';
        else if ((in[i-1] >= 'a') && (in[i-1] <= 'f'))
            in[i-1] = in[i-1] - 'a' + 10;
        else if ((in[i-1] >= 'A') && (in[i-1] <= 'F'))
            in[i-1] = in[i-1] - 'A' + 10;
        else if(in[i-1]!=0)
            throw myexception("Входные данные не 16-ричное числа");
        out.push_back(in[i-1]<<4);
        if ((in[i] >= '0') && (in[i] <= '9'))
            in[i] = in[i] - '0';
        else if ((in[i] >= 'a') && (in[i] <= 'f'))
            in[i] = in[i] - 'a' + 10;
        else if ((in[i] >= 'A') && (in[i] <= 'F'))
            in[i] = in[i] - 'A' + 10;
        else if(in[i]!=0)
            throw myexception("Входные данные не 16-ричное числа");
        out[j]|=in[i];
        j++;
    }
    return out;
}

inline vector<uint8_t>Controller::keyToUint8_t(string &&key, bool isKeyHex)
{
    if(isKeyHex)
    {
        if(key.size()>64)
            throw myexception("Слишком большой ключ (>256 бит)");
        else
            if(key.size()<32)
                key.resize(32);
            else
                if(key.size()%16)
                    key.resize(key.size()+(16-key.size()%16));
        return string_to_hex(move(key));
    }
    else
    {
        if(key.size()>32)
            throw myexception("Слишком большой ключ (>256 бит)");
        else
            if(key.size()<16)
                key.resize(16);
            else
                if(key.size()%8)
                    key.resize(key.size()+(8-key.size()%8));
        return vector<uint8_t>(key.begin(),key.end());
    }
}

inline vector<uint8_t>Controller::PTToUint8_t(string &&text, bool isPTHex)
{
    if(isPTHex)
    {
        if(text.size()%32)
            text.resize(text.size()+(32-text.size()%32));
        return string_to_hex(move(text));
    }
    else
    {
        if(text.size()%16)
            text.resize(text.size()+(16-text.size()%16));
        return vector<uint8_t>(text.begin(),text.end());
    }

}

string Controller::encrypt(string &&key, string &&text, string &&initvec, bool isKeyHex, bool isPTHex, int mode)
{
    if (key.size()==0||text.size()==0)
        throw myexception("Введите данные");
    vector<uint8_t> _key=keyToUint8_t(move(key),isKeyHex);
    vector<uint8_t> _text=PTToUint8_t(move(text),isPTHex);
    vector<uint8_t> _initvec=string_to_hex(move(initvec));

    Encryptor* encryptor;
    switch(mode)
    {
        case ECB:
        encryptor=new Encryptor(new ECB_MODE);
            break;
        case CBC:
        encryptor=new Encryptor(new CBC_MODE(move(_initvec)));
            break;
        case CFB:
        encryptor=new Encryptor(new CFB_MODE(move(_initvec)));
            break;
    }
    vector<uint8_t> out=encryptor->encrypt(move(_key), move(_text));
    delete encryptor;
    return hex_to_string(move(out));
}

string Controller::decrypt(string &&key, string &&text, string &&initvec, bool isKeyHex, bool isPTHex, int mode)
{
    if (key.size()==0||text.size()==0)
        throw myexception("Введите данные");
    vector<uint8_t> _key=keyToUint8_t(move(key),isKeyHex);
    vector<uint8_t> _text=PTToUint8_t(move(text),isPTHex);
    vector<uint8_t> _initvec=string_to_hex(move(initvec));

    Encryptor* encryptor;
    switch(mode)
    {
        case ECB:
        encryptor=new Encryptor(new ECB_MODE);
            break;
        case CBC:
        encryptor=new Encryptor(new CBC_MODE(move(_initvec)));
            break;
        case CFB:
        encryptor=new Encryptor(new CFB_MODE(move(_initvec)));
            break;
    }
    vector<uint8_t> out=encryptor->decrypt(move(_key), move(_text));
    delete encryptor;
    return hex_to_string(move(out));
}

void Controller::safeToFile(string &&filename, string &&out)
{
    this->fileManager->saveFile(move(filename),move(out));
}

string Controller::loadFromFile(string &&filename)
{
    return this->fileManager->loadFile(move(filename));
}
