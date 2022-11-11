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
    vector<uint8_t> _text(it1,it2);
    for(int i=0; i<16; i++)
        _text[i]^=initvec[i];
    vector<uint8_t> _text1=twofish->encrypt(move(_text));
    out.insert(out.cend(),_text1.begin(), _text1.end());
    for(int i=1; i<text.size()/16;i++)
    {
        it1=it2;
        it2=it1+16;
        _text.assign(it1,it2);
        for(int i=0; i<16; i++)
            _text1[i]^=_text[i];
        _text1=twofish->encrypt(move(_text1));
        out.insert(out.cend(),_text1.begin(), _text1.end());
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
    vector<uint8_t> _text(it1,it2);
    vector<uint8_t> _text1=twofish->decrypt(move(_text));
    for(int i=0; i<16; i++)
        _text1[i]^=initvec[i];
    out.insert(out.cend(),_text1.begin(), _text1.end());
    for(int i=1; i<text.size()/16;i++)
    {
        it1=it2;
        it2=it1+16;
        _text1.assign(it1,it2);
        _text1=twofish->decrypt(move(_text1));
            for(int i=0; i<16; i++)
                _text1[i]^=_text[i];
            _text.assign(it1,it2);
        out.insert(out.cend(),_text1.begin(), _text1.end());

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
    for(int i=1; i<text.size()/16;i++)
    {
        it1=it2;
        it2=it1+16;
        t.assign(it1,it2);
        _initvec=twofish->encrypt(move(_initvec));
        for(int i=0; i<16; i++)
            _initvec[i]^=t[i];
        out.insert(out.cend(),_initvec.begin(), _initvec.end());
    }
    twofish->keyReset();
    return out;
}

vector<uint8_t> CFB_MODE::decrypt( vector<uint8_t>&& key, vector<uint8_t>&& text, Twofish* twofish)
{
    twofish->keySchedule(move(key));
    vector<uint8_t> out;
    out.reserve(text.size());
    vector<uint8_t> t1(initvec.begin(),initvec.end());
    t1=twofish->encrypt(move(t1));
    auto it1=text.begin();
    auto it2=text.begin()+16;
    vector<uint8_t> t2(it1, it2);
    for(int i=0; i<16; i++)
        t1[i]^=t2[i];
    out.insert(out.cend(),t1.begin(), t1.end());
    for(int i=1; i<text.size()/16;i++)
    {
        t1=twofish->encrypt(move(t2));
        it1=it2;
        it2=it1+16;
        t2.assign(it1,it2);
        for(int i=0; i<16; i++)
            t1[i]^=t2[i];
        out.insert(out.cend(),t1.begin(), t1.end());
    }
    twofish->keyReset();
    return out;
}
