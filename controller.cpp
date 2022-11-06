#include "controller.h"
#include <QString>

static const char hex_table[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

string BytesToHexStr(vector<uint8_t> &&aInStr)
{
    std::string hex_str;
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
                if(key.size()%32)
                    key.resize(key.size()+(32-key.size()%32));
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
                if(key.size()%16)
                    key.resize(key.size()+(16-key.size()%16));
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

string Controller::encrypt(string &&key, string &&text, bool isKeyHex, bool isPTHex)
{
    if (key.size()==0||text.size()==0)
        throw myexception("Введите данные");
    vector<uint8_t> _key=keyToUint8_t(move(key),isKeyHex);
    twofish->keySchedule(move(_key));

    vector<uint8_t> _text=PTToUint8_t(move(text),isPTHex);
    string out;
    auto it1=_text.begin();
    auto it2=_text.begin()+16;
    for(int i=0; i<_text.size()/16;i++)
    {
        vector<uint8_t> t(it1,it2);
        it1=it2;
        it2=it1+16;
        t=twofish->encrypt(move(t));
        out+=BytesToHexStr(move(t));
    }
    twofish->keyReset();
    return out;
}

string Controller::decrypt(string &&key, string &&text, bool isKeyHex, bool isPTHex)
{
    if (key.size()==0||text.size()==0)
        throw myexception("Введите данные");
    vector<uint8_t> _key=keyToUint8_t(move(key),isKeyHex);
    twofish->keySchedule(move(_key));

    vector<uint8_t> _text=PTToUint8_t(move(text),isPTHex);
    string out;
    auto it1=_text.begin();
    auto it2=_text.begin()+16;
    for(int i=0; i<_text.size()/16;i++)
    {
        vector<uint8_t> t(it1,it2);
        it1=it2;
        it2=it1+16;
        t=twofish->decrypt(move(t));
        out+=BytesToHexStr(move(t));
    }
    twofish->keyReset();
    return out;
}

void Controller::printResults(string &&filename, string &&out)
{
    this->fileManager->saveFile(move(filename),move(out));
}

string Controller::loadKey(string &&filename)
{
    return this->fileManager->loadFile(move(filename));
}

string Controller::loadInputText(string &&filename)
{
    return this->fileManager->loadFile(move(filename));
}
