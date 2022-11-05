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

vector<uint8_t> string_to_hex(string&& in)
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
        out.push_back(in[i-1]<<4);
        if ((in[i] >= '0') && (in[i] <= '9'))
            in[i] = in[i] - '0';
        else if ((in[i] >= 'a') && (in[i] <= 'f'))
            in[i] = in[i] - 'a' + 10;
        else if ((in[i] >= 'A') && (in[i] <= 'F'))
            in[i] = in[i] - 'A' + 10;
        out[j]|=in[i];
        j++;
    }
    return out;
}

inline vector<uint8_t> ParseHexDword(string&& srcTxt)
{
    int i;
    uint8_t b = 0;
    char c;
    vector<uint8_t> out;
    out.reserve(srcTxt.size()/2);

    for (i = 0; i < srcTxt.size(); i++)
    {
        c = srcTxt[i];
        if ((c >= '0') && (c <= '9'))
            b = c - '0';
        else if ((c >= 'a') && (c <= 'f'))
            b = c - 'a' + 10;
        else if ((c >= 'A') && (c <= 'F'))
            b = c - 'A' + 10;

        out.push_back( b << (4 * ((i ^ 1) & 7)));
    }
    return out;
}


string Controller::encrypt(string &&key, string &&text)
{
    if(key.size()%32)
        key.resize(key.size()+(32-key.size()%32));
    vector<uint8_t> _key(key.begin(),key.end());
    twofish->keySchedule(move(_key));

    if(text.size()%32)
        text.resize(text.size()+(32-text.size()%32));
    vector<uint8_t> _text;
    _text.reserve(text.size()/2);
    int j=0;
    for(int i=1; i<text.size();i+=2)
    {
        _text.push_back(strtoul((char*)&text[i-1],0,16)<<4);
        _text[j]|=strtoul((char*)&text[i],0,16);
        j++;
    }

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
    return out;
}

string Controller::decrypt(string &&key, string &&text)
{
    if(key.size()%32)
        key.resize(key.size()+(32-key.size()%32));
    vector<uint8_t> _key(key.begin(),key.end());
    twofish->keySchedule(move(_key));

    if(text.size()%32)
        text.resize(text.size()+(32-text.size()%32));
    vector<uint8_t> _text=string_to_hex(move(text));

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
    return out;
}
