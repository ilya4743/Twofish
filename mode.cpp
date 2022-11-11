#include "mode.h"

vector<uint8_t> ECB_MODE::encrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish)
{
    twofish->keySchedule(move(key));
    vector<uint8_t> out;
    out.reserve(text.size());
    auto it1=text.begin();
    auto it2=text.begin()+16;
    for(int i=0; i<text.size()/16;i++)
    {
        vector<uint8_t> t(it1,it2);
        it1=it2;
        it2=it1+16;
        t=twofish->encrypt(move(t));
        out.insert(out.cend(),t.begin(), t.end());
    }
    twofish->keyReset();
    return out;
}

vector<uint8_t> ECB_MODE::decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish)
{
    twofish->keySchedule(move(key));
    vector<uint8_t> out;
    out.reserve(text.size());
    auto it1=text.begin();
    auto it2=text.begin()+16;
    for(int i=0; i<text.size()/16;i++)
    {
        vector<uint8_t> t(it1,it2);
        it1=it2;
        it2=it1+16;
        t=twofish->decrypt(move(t));
        out.insert(out.cend(),t.begin(), t.end());
    }
    twofish->keyReset();
    return out;
}

vector<uint8_t> CBC_MODE::encrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish)
{
    twofish->keySchedule(move(key));
    vector<uint8_t> out;
    out.reserve(text.size());
    auto it1=text.begin();
    auto it2=text.begin()+16;
    vector<uint8_t> t(it1,it2);
    for(int i=0; i<16; i++)
        t[i]^=initvec[i];
    it1=it2;
    it2=it1+16;
    t=twofish->encrypt(move(t));
    out.insert(out.cend(),t.begin(), t.end());
    for(int i=16; i<text.size()/16;i++)
    {
        for(int i=0; i<16; i++)
            t[i]^=initvec[i];
        t.assign(it1,it2);
        it1=it2;
        it2=it1+16;
        t=twofish->encrypt(move(t));
        out.insert(out.cend(),t.begin(), t.end());
    }
    twofish->keyReset();
    return out;
}

vector<uint8_t> CBC_MODE::decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish)
{
    twofish->keySchedule(move(key));
    vector<uint8_t> out;
    out.reserve(text.size());
    auto it1=text.begin();
    auto it2=text.begin()+16;
    vector<uint8_t> t(it1,it2);
    for(int i=0; i<16; i++)
        t[i]^=initvec[i];
    it1=it2;
    it2=it1+16;
    t=twofish->decrypt(move(t));
    out.insert(out.cend(),t.begin(), t.end());
    for(int i=16; i<text.size()/16;i++)
    {
        for(int i=0; i<16; i++)
            t[i]^=initvec[i];
        t.assign(it1,it2);
        it1=it2;
        it2=it1+16;
        t=twofish->decrypt(move(t));
        out.insert(out.cend(),t.begin(), t.end());
    }
    twofish->keyReset();
    return out;
}

vector<uint8_t> CFB_MODE::encrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish)
{
    twofish->keySchedule(move(key));
    vector<uint8_t> out;
    out.reserve(text.size());
    vector<uint8_t> _initvec(initvec.begin(),initvec.end());
    _initvec=twofish->encrypt(move(_initvec));
    auto it1=text.begin();
    auto it2=text.begin()+16;
    vector<uint8_t> t(it1, it2);
    for(int i=0; i<16; i++)
        _initvec[i]^=t[i];
    out.insert(out.cend(),_initvec.begin(), _initvec.end());
    for(int i=16; i<text.size()/16;i++)
    {
        it1=it2;
        it2=it1+16;
        t=twofish->encrypt(move(t));
        t.assign(it1,it2);
        for(int i=0; i<16; i++)
            _initvec[i]^=t[i];
        out.insert(out.cend(),t.begin(), t.end());
    }
    twofish->keyReset();
    return out;
}

vector<uint8_t> CFB_MODE::decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish)
{
    twofish->keySchedule(move(key));
    vector<uint8_t> out;
    out.reserve(text.size());
    vector<uint8_t> _initvec(initvec.begin(),initvec.end());
    _initvec=twofish->decrypt(move(_initvec));
    auto it1=text.begin();
    auto it2=text.begin()+16;
    vector<uint8_t> t(it1, it2);
    for(int i=0; i<16; i++)
        _initvec[i]^=t[i];
    out.insert(out.cend(),_initvec.begin(), _initvec.end());
    for(int i=16; i<text.size()/16;i++)
    {
        it1=it2;
        it2=it1+16;
        t=twofish->decrypt(move(t));
        t.assign(it1,it2);
        for(int i=0; i<16; i++)
            _initvec[i]^=t[i];
        out.insert(out.cend(),t.begin(), t.end());
    }
    twofish->keyReset();
    return out;
}
