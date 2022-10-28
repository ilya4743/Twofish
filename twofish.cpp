#include "twofish.h"

using namespace std;

uint8_t k;

const uint32_t p = 16843009;

boost::qvm::mat<uint8_t, 4, 16> Tq0 =
{
    0x8, 0x1, 0x7, 0xD, 0x6, 0xF, 0x3, 0x2, 0x0, 0xB, 0x5, 0x9, 0xE, 0xC, 0xA, 0x4,
    0xE, 0XC, 0XB, 0X8, 0X1, 0X2, 0X3, 0X5, 0XF, 0X4, 0XA, 0X6, 0X7, 0X0, 0X9, 0XD,
    0XB, 0XA, 0X5, 0XE, 0X6, 0XD, 0X9, 0X0, 0XC, 0X8, 0XF, 0X3, 0X2, 0X4, 0X7, 0X1,
    0XD, 0X7, 0XF, 0X4, 0X1, 0X2, 0X6, 0XE, 0X9, 0XB, 0X3, 0X0, 0X8, 0X5, 0XC, 0XA
};

boost::qvm::mat<uint8_t, 4, 16> Tq1 =
{
    0X2, 0X8, 0XB, 0XD, 0XF, 0X7, 0X6, 0XE, 0X3, 0X1, 0X9, 0X4, 0X0, 0XA, 0XC, 0X5,
    0X1, 0XE, 0X2, 0XB, 0X4, 0XC, 0X3, 0X7, 0X6, 0XD, 0XA, 0X5, 0XF, 0X9, 0X0, 0X8,
    0X4, 0XC, 0X7, 0X5, 0X1, 0X6, 0X9, 0XA, 0X0, 0XE, 0XD, 0X8, 0X2, 0XB, 0X3, 0XF,
    0xB, 0X9, 0X5, 0X1, 0XC, 0X3, 0XD, 0XE, 0X6, 0X4, 0X7, 0XF, 0X2, 0X0, 0X8, 0XA
};

uint8_t Twofish::q0(uint8_t x)
{
    uint8_t a0, b0, a2, b2, a4, b4, c00;
    a0 = x / 16;
    b0 = x % 16;
    a0 = ((a0 ^ b0) << 4) | a0;
    b0 = (((a0 & 0x0F) ^ (ROR4L(b0)) ^ 8 * (a0 & 0x0F) % 16) << 4) | b0;
    a2 = Tq0.a[0][(a0 & 0xF0) >> 4];
    b2 = Tq0.a[1][(b0 & 0xF0) >> 4];
    a2 = ((a2 ^ b2) << 4) | a2;
    b2 = (((a2 & 0x0F) ^ (ROR4L(b2)) ^ 8 * (a2 & 0x0F) % 16) << 4) | b2;
    a4 = Tq0.a[2][(a2 & 0xF0) >> 4];
    b4 = Tq0.a[3][(b2 & 0xF0) >> 4];
    return 16 * b4 + a4;
}

uint8_t Twofish::q1(uint8_t x)
{
    uint8_t a0, b0, a2, b2, a4, b4;
    a0 = x / 16;
    b0 = x % 16;
    a0 = ((a0 ^ b0) << 4) | a0;
    b0 = (((a0 & 0x0F) ^ (ROR4L(b0)) ^ 8 * (a0 & 0x0F) % 16) << 4) | b0;
    a2 = Tq1.a[0][(a0 & 0xF0) >> 4];
    b2 = Tq1.a[1][(b0 & 0xF0) >> 4];
    a2 = ((a2 ^ b2) << 4) | a2;
    b2 = (((a2 & 0x0F) ^ ROR4L(b2) ^ 8 * (a2 & 0x0F) % 16) << 4) | b2;
    a4 = Tq1.a[2][(a2 & 0xF0) >> 4];
    b4 = Tq1.a[3][(b2 & 0xF0) >> 4];
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

vector<uint32_t> Twofish::ParseHexDword(int bits, char* srcTxt)
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
        else
        {
        }

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

void Twofish::keySchedule(uint8_t* key, uint32_t k)
{
    this->k = k;
    vector<uint32_t> kv;
    kv = ParseHexDword(k*64, (char*)key);

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


void Twofish::encrypt(const uint8_t* in, uint8_t* out)
{
    uint32_t a, b, c, d;
    uint32_t t0, t1;
    vector<uint32_t> vec;
    vec=ParseHexDword(64*k, (char*)in);
    a = vec[0];
    b = vec[1];
    c = vec[2];
    d = vec[3];
    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);
    a ^= sk[0];
    b ^= sk[1];
    c ^= sk[2];
    d ^= sk[3];

    for (uint32_t r = 0; r < 16; r++)
    {
        t0 = h(a,v);
        t1 = h(ROL(b, 8),v);
        d = ROL(d, 1);
        c ^= (t0 + t1 + sk[2 * r + 8]);
        d ^= (t0 + 2 * t1 + sk[2 * r + 9]);
        c = ROR(c,1);        
        swap(a, c);
        swap(b, d);
    }
    swap(a, c);
    swap(b, d);
    a ^= sk[4];
    b ^= sk[5];
    c ^= sk[6];
    d ^= sk[7];
    string s;

    STORE32L(a, &out[0]);
    STORE32L(b, &out[4]);
    STORE32L(c, &out[8]);
    STORE32L(d, &out[12]);
}

void Twofish::decrypt(const uint8_t* in, uint8_t* out)
{
    uint32_t a, b, c, d;
    uint32_t t0, t1;
    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);

    a ^= sk[4];
    b ^= sk[5];
    c ^= sk[6];
    d ^= sk[7];

    for (int32_t r = 15; r >= 0; r--)
    {
        t0 = h(a, v);
        t1 = h(ROL(b, 8), v);
        c = ROL(c, 1);
        c ^= (t0 + t1 + sk[2 * r + 8]);
        d ^= (t0 + 2 * t1 + sk[2 * r + 9]);
        d = ROR(d, 1);
        swap(a, c);
        swap(b, d);
    }
    swap(a, c);
    swap(b, d);
    a ^= sk[0];
    b ^= sk[1];
    c ^= sk[2];
    d ^= sk[3];

    STORE32L(a, &out[0]);
    STORE32L(b, &out[4]);
    STORE32L(c, &out[8]);
    STORE32L(d, &out[12]);
}