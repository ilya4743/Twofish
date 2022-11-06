#include "twofish.h"

using namespace std;

uint8_t Twofish::q0(uint8_t x)
{
    uint8_t a0, b0, a2, b2, a4, b4, c00;
    a0 = x / 16;
    b0 = x % 16;
    a0 = ((a0 ^ b0) << 4) | a0;
    b0 = (((a0 & 0x0F) ^ (ROR4L(b0)) ^ 8 * (a0 & 0x0F) % 16) << 4) | b0;
    a2 = Tq0[0][(a0 & 0xF0) >> 4];
    b2 = Tq0[1][(b0 & 0xF0) >> 4];
    a2 = ((a2 ^ b2) << 4) | a2;
    b2 = (((a2 & 0x0F) ^ (ROR4L(b2)) ^ 8 * (a2 & 0x0F) % 16) << 4) | b2;
    a4 = Tq0[2][(a2 & 0xF0) >> 4];
    b4 = Tq0[3][(b2 & 0xF0) >> 4];
    return 16 * b4 + a4;
}

uint8_t Twofish::q1(uint8_t x)
{
    uint8_t a0, b0, a2, b2, a4, b4;
    a0 = x / 16;
    b0 = x % 16;
    a0 = ((a0 ^ b0) << 4) | a0;
    b0 = (((a0 & 0x0F) ^ (ROR4L(b0)) ^ 8 * (a0 & 0x0F) % 16) << 4) | b0;
    a2 = Tq1[0][(a0 & 0xF0) >> 4];
    b2 = Tq1[1][(b0 & 0xF0) >> 4];
    a2 = ((a2 ^ b2) << 4) | a2;
    b2 = (((a2 & 0x0F) ^ ROR4L(b2) ^ 8 * (a2 & 0x0F) % 16) << 4) | b2;
    a4 = Tq1[2][(a2 & 0xF0) >> 4];
    b4 = Tq1[3][(b2 & 0xF0) >> 4];
    return 16 * b4 + a4;
}

uint32_t Twofish::h(uint32_t x, const vector<vector<uint8_t>>& l)
{
    uint8_t b[4];
    b[0]= x & 0xFF;
    b[1] = x >> 8 & 0xFF;
    b[2] = x >> 16 & 0xFF;
    b[3] = x >> 24 & 0xFF;
    switch (k)
    {
    case 4:
        b[0] = q1(b[0]) ^ l[3][0];
        b[1] = q0(b[1]) ^ l[3][1];
        b[2] = q0(b[2]) ^ l[3][2];
        b[3] = q1(b[3]) ^ l[3][3];

    case 3:
        b[0] = q1(b[0]) ^ l[2][0];
        b[1] = q1(b[1]) ^ l[2][1];
        b[2] = q0(b[2]) ^ l[2][2];
        b[3] = q0(b[3]) ^ l[2][3];

    default:
        b[0] = q1(q0(q0(b[0]) ^ l[1][0]) ^ l[0][0]);
        b[1] = q0(q0(q1(b[1]) ^ l[1][1]) ^ l[0][1]);
        b[2] = q1(q1(q0(b[2]) ^ l[1][2]) ^ l[0][2]);
        b[3] = q0(q1(q1(b[3]) ^ l[1][3]) ^ l[0][3]);
    }

    return	
        ((M00(b[0]) ^ M01(b[1]) ^ M02(b[2]) ^ M03(b[3]))) ^
        ((M10(b[0]) ^ M11(b[1]) ^ M12(b[2]) ^ M13(b[3])) << 8) ^
        ((M20(b[0]) ^ M21(b[1]) ^ M22(b[2]) ^ M23(b[3])) << 16) ^
        ((M30(b[0]) ^ M31(b[1]) ^ M32(b[2]) ^ M33(b[3])) << 24);
}

inline vector<uint32_t> Twofish::ParseHexDword(int bits, vector<uint8_t>&& srcTxt)
{
    int i;
    uint32_t b = 0;
    char c;
    vector<uint32_t> out;
    out.resize(bits / 32);

    for (i = 0; i * 4 < bits; i++)
    {
        c = srcTxt[i];
        if ((c >= '0') && (c <= '9'))
            b = c - '0';
        else if ((c >= 'a') && (c <= 'f'))
            b = c - 'a' + 10;
        else if ((c >= 'A') && (c <= 'F'))
            b = c - 'A' + 10;

        out[i / 8] |= b << (4 * ((i ^ 1) & 7));
    }
    return out;
}

uint32_t Twofish::RS_MDS_Encode(uint32_t k0, uint32_t k1)
{
    int i, j;
    uint32_t r;

    for (i = r = 0; i < 2; i++)
    {
        r ^= (i) ? k0 : k1;
        for (j = 0; j < 4; j++)
            RS_rem(r);
    }
    return r;
}

void Twofish::printSubkeys()
{
    for (int i = 0; i < sk.size(); i += 4)
    {
        cout << hex << uppercase << sk[i];
        for (int j = 1; j < 4; j++)
            cout << '\t' << sk[i + j];
        cout << endl;
    }
}

void Twofish::keySchedule(vector<uint8_t>&& key)
{
    this->k = (key.size()*8)/64;
    vector<uint32_t> kv;
    kv.reserve(key.size()/4);
    for(int i=0; i<key.size()/4;i++)
        kv.push_back(key[i]<<24 | key[i+1] << 16 | key[i+2] << 8 | key[i+3] << 0);

    Me.reserve(k);
    Mo.reserve(k);
    V.reserve(k);
    me.reserve(k);
    mo.reserve(k);
    for (int i = 0; i < k; i++)
    {
        Me.push_back(kv[2 * i]);
        Mo.push_back(kv[2 * i + 1]);
        V.push_back(RS_MDS_Encode(Me[i], Mo[i]));

        me.push_back(vector<uint8_t>());
        me[i].reserve(4);
        me[i].push_back(kv[2 * i] & 0xFF);
        me[i].push_back(kv[2 * i] >> 8 & 0xFF);
        me[i].push_back(kv[2 * i] >> 16 & 0xFF);
        me[i].push_back(kv[2 * i] >> 24 & 0xFF);

        mo.push_back(vector<uint8_t>());
        mo[i].reserve(4);
        mo[i].push_back(kv[2 * i + 1] & 0xFF);
        mo[i].push_back(kv[2 * i + 1] >> 8 & 0xFF);
        mo[i].push_back(kv[2 * i + 1] >> 16 & 0xFF);
        mo[i].push_back(kv[2 * i + 1] >> 24 & 0xFF);

        v.push_back(vector<uint8_t>());
        v[i].reserve(4);
        v[i].push_back(V[i] & 0xFF);
        v[i].push_back(V[i] >> 8 & 0xFF);
        v[i].push_back(V[i] >> 16 & 0xFF);
        v[i].push_back(V[i] >> 24 & 0xFF);
    }
    reverse(v.begin(), v.end());
    reverse(V.begin(), V.end());
    sk.reserve(40);
    for (uint8_t i = 0; i < 20; i++)
    {
        uint32_t Ai = h(2 * i * p, me);
        uint32_t Bi = ROL(h((2 * i + 1) * p, mo), 8);
        sk.push_back(Ai + Bi);
        sk.push_back(ROL((Ai + 2 * Bi), 9));
    }
}

void Twofish::keyReset()
{
    Me.clear();
    me.clear();
    Mo.clear();
    mo.clear();
    V.clear();
    v.clear();
    sk.clear();
}

vector<uint8_t> Twofish::encrypt(vector<uint8_t>&& in)
{
    uint32_t x[4];
    uint32_t t0, t1;
    vector<uint8_t> out;
    out.reserve(16);

    x[0] = in[0] | in[1] << 8 | in[2] << 16 | in[3] << 24;
    x[1] = in[4] | in[5] << 8 | in[6] << 16 | in[7] << 24;
    x[2] = in[8] | in[9] << 8 | in[10] << 16 | in[11] << 24;
    x[3] = in[12] | in[13] << 8 | in[14] << 16 | in[15] << 24;

    x[0] ^= sk[0];
    x[1] ^= sk[1];
    x[2] ^= sk[2];
    x[3] ^= sk[3];

    for (uint32_t r = 0; r < 16; r++)
    {
        t0 = h(x[0],v);
        t1 = h(ROL(x[1], 8),v);
        x[3] = ROL(x[3], 1);
        x[2] ^= (t0 + t1 + sk[2 * r + 8]);
        x[3] ^= (t0 + 2 * t1 + sk[2 * r + 9]);
        x[2] = ROR(x[2],1);        
        swap(x[0], x[2]);
        swap(x[1], x[3]);
    }
    swap(x[0], x[2]);
    swap(x[1], x[3]);

    x[0] ^= sk[4];
    x[1] ^= sk[5];
    x[2] ^= sk[6];
    x[3] ^= sk[7];

    for (uint8_t i = 0; i < 4; i++)
        for (uint8_t j = 0; j < 4; j++)
            out.push_back(x[i]>>j*8 & 0xFF);

    return out;
}

vector<uint8_t> Twofish::decrypt(vector<uint8_t>&& in)
{
    uint32_t x[4];    
    uint32_t t0, t1;
    uint32_t a, b, c, d;
    vector<uint8_t> out;
    out.reserve(16);

    x[0] = in[0] | in[1] << 8 | in[2] << 16 | in[3] << 24;
    x[1] = in[4] | in[5] << 8 | in[6] << 16 | in[7] << 24;
    x[2] = in[8] | in[9] << 8 | in[10] << 16 | in[11] << 24;
    x[3] = in[12] | in[13] << 8 | in[14] << 16 | in[15] << 24;

    x[0] ^= sk[4];
    x[1] ^= sk[5];
    x[2] ^= sk[6];
    x[3] ^= sk[7];

    for (int32_t r = 15; r >= 0; r--)
    {
        t0 = h(x[0], v);
        t1 = h(ROL(x[1], 8), v);
        x[2] = ROL(x[2], 1);
        x[2] ^= (t0 + t1 + sk[2 * r + 8]);
        x[3] ^= (t0 + 2 * t1 + sk[2 * r + 9]);
        x[3] = ROR(x[3], 1);
        swap(x[0], x[2]);
        swap(x[1], x[3]);
    }
    swap(x[0], x[2]);
    swap(x[1], x[3]);

    x[0] ^= sk[0];
    x[1] ^= sk[1];
    x[2] ^= sk[2];
    x[3] ^= sk[3];

    for (uint8_t i = 0; i < 4; i++)
        for (uint8_t j = 0; j < 4; j++)
            out.push_back(x[i] >> j * 8 & 0xFF);
    
    return out;
}
