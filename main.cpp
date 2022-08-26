#include <cstring>
#include<cmath>
#include<vector>
#include <fstream>
#include <iostream>
#include <boost/qvm/mat.hpp>
#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>

using namespace std;

uint8_t k;
#define STORE32L(x, y)  { uint32_t __t = (x); memcpy(y, &__t, 4); }
#define LOAD32L(x, y)   { memcpy(&(x), y, 4); x &= 0xFFFFFFFF; }

#define ROL(x, y) ( (((uint32_t)(x)<<(uint32_t)((y)&31)) | (((uint32_t)(x)&0xFFFFFFFFUL)>>(uint32_t)(32-((y)&31)))) & 0xFFFFFFFFUL)
#define ROR(x, y) ( ((((uint32_t)(x)&0xFFFFFFFFUL)>>(uint32_t)((y)&31)) | ((uint32_t)(x)<<(uint32_t)(32-((y)&31)))) & 0xFFFFFFFFUL)

#define ROL4L(x) (((x << 1) & 0x0F | ((x & 0x08) >> 3)) | (x & 0xF0))
#define ROL4H(x) (((x << 1) & 0xF0 | ((x & 0x80) >> 3)) | (x & 0x0F))

#define ROR4L(x) (((x >> 1) & 0x0F | ((x & 0x01) << 3)) | (x & 0xF0))
#define ROR4H(x) (((x >> 1) & 0xF0 | ((x & 0x10) << 3)) | (x & 0x0F))

const uint32_t p = 16843009;

boost::qvm::mat<uint8_t, 4, 4> M1 =
{
    0x01, 0xEF, 0x5B, 0x5B,
    0x5B, 0xEF, 0xEF, 0x01,
    0xEF, 0X5B, 0X01, 0XEF,
    0xEF, 0X01, 0XEF, 0X5B
};

boost::qvm::mat<uint8_t, 4, 8> M2 =
{
    0x01, 0xA4, 0x55, 0x87, 0x5A, 0x58, 0xDB, 0x9E,
    0xA4, 0x56, 0x82, 0xF3, 0X1E, 0XC6, 0X68, 0XE5,
    0X02, 0XA1, 0XFC, 0XC1, 0X47, 0XAE, 0X3D, 0X19,
    0XA4, 0X55, 0X87, 0X5A, 0X58, 0XDB, 0X9E, 0X03
};

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
//res = (n1 << 24) | (n2 << 16) | (n3 << 8) | n4;

uint8_t q0(uint8_t x)
{
    uint8_t a0, b0, a2, b2, a4, b4;
    a0 = x / 16;
    b0 = x % 16;
    a0 = (a0 ^ b0) << 4;
    b0 = ((a0 & 0x0F) ^ ROR4L(b0) ^ 8 * (a0 & 0x0F) % 16) << 4;
    a2 = Tq0.a[0][a0 & 0xF0];
    b2 = Tq0.a[1][b0 & 0xF0];
    a2 = ((a2 & 0xF0) ^ (b2 & 0xF0)) << 4;
    b2 = ((a2 & 0xF0) ^ ROR4L(b2) ^ 8 * (a2 & 0xF0) % 16) << 4;
    a4 = Tq0.a[2][a2 & 0xF0];
    b4 = Tq0.a[2][b2 & 0xF0];
    return 16 * b4 + a4;
}

uint8_t q1(uint8_t x)
{
    uint8_t a0, b0, a2, b2, a4, b4;
    a0 = x / 16;
    b0 = x % 16;
    a0 = (a0 ^ b0) << 4;
    b0 = ((a0 & 0x0F) ^ ROR4L(b0) ^ 8 * (a0 & 0x0F) % 16) << 4;
    a2 = Tq1.a[0][a0 & 0xF0];
    b2 = Tq1.a[1][b0 & 0xF0];
    a2 = ((a2 & 0xF0) ^ (b2 & 0xF0)) << 4;
    b2 = ((a2 & 0xF0) ^ ROR4L(b2) ^ 8 * (a2 & 0xF0) % 16) << 4;
    a4 = Tq1.a[2][a2 & 0xF0];
    b4 = Tq1.a[2][b2 & 0xF0];
    return 16 * b4 + a4;
}
vector<uint32_t> v;

uint32_t h(uint32_t b, const vector<uint32_t>& M)
{
    uint8_t b0, b1, b2, b3;

    b0 = b & 0xFF;
    b1 = b >> 8 & 0xFF;
    b2 = b >> 16 & 0xFF;
    b3 = b >> 24 & 0xFF;

    if (k == 4)
    {
        b0 = b & 0xFF;
        b1 = b >> 8 & 0xFF;
        b2 = b >> 16 & 0xFF;
        b3 = b >> 24 & 0xFF;
        b0 = q1(b0);
        b1 = q0(b1);
        b2 = q0(b2);
        b3 = q1(b3);
        b = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
        b ^= M[3];

        b0 = b & 0xFF;
        b1 = b >> 8 & 0xFF;
        b2 = b >> 16 & 0xFF;
        b3 = b >> 24 & 0xFF;
        b0 = q1(b0);
        b1 = q1(b1);
        b2 = q0(b2);
        b3 = q0(b3);
        b = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
        b ^= M[2];
    }
    else if (k == 3)
    {
        b0 = b & 0xFF;
        b1 = b >> 8 & 0xFF;
        b2 = b >> 16 & 0xFF;
        b3 = b >> 24 & 0xFF;
        b0 = q1(b0);
        b1 = q1(b1);
        b2 = q0(b2);
        b3 = q0(b3);
        b = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
        b ^= M[2];
    }

    b0 = b & 0xFF;
    b1 = b >> 8 & 0xFF;
    b2 = b >> 16 & 0xFF;
    b3 = b >> 24 & 0xFF;
    b0 = q0(b0);
    b1 = q1(b1);
    b2 = q0(b2);
    b3 = q1(b3);
    b = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
    b ^= M[1];

    b0 = b & 0xFF;
    b1 = b >> 8 & 0xFF;
    b2 = b >> 16 & 0xFF;
    b3 = b >> 24 & 0xFF;
    b0 = q0(b0);
    b1 = q0(b1);
    b2 = q1(b2);
    b3 = q1(b3);
    b = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
    b ^= M[0];

    b0 = b & 0xFF;
    b1 = b >> 8 & 0xFF;
    b2 = b >> 16 & 0xFF;
    b3 = b >> 24 & 0xFF;
    b0 = q1(b0);
    b1 = q0(b1);
    b2 = q1(b2);
    b3 = q0(b3);

    boost::qvm::mat <uint8_t, 4, 1>y = { b0,b1,b2,b3 };
    boost::qvm::mat <uint8_t, 4, 1>res = M1 * y;
    uint32_t H = 0;
    for (uint8_t i = 0; i < 4; i++)
        H += res.a[0][i] * pow(2, 8 * i);
    return H;
}

uint32_t g(uint32_t b)
{
    uint8_t b0, b1, b2, b3;
    b = h(b, v);
    b0 = b & 0xFF;
    b1 = b >> 8 & 0xFF;
    b2 = b >> 16 & 0xFF;
    b3 = b >> 24 & 0xFF;
    boost::qvm::mat <uint8_t, 4, 1>s = { b0,b1,b2,b3 };
    boost::qvm::mat <uint8_t, 4, 1>res = M1 * s;
    return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}

void encrypt(const uint8_t* in, uint8_t* out, const vector<uint32_t>& sk)
{
    uint32_t a, b, c, d;
    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);
    a=a^sk[0];
    b^=sk[1];
    c^=sk[2];
    d^=sk[3];
    for (uint8_t r = 0; r < 16; r++)
    {
        b = ROL(b, 8);
        a = g(a);
        b = g(b);
        a = a + b % (uint64_t)pow(2, 32);
        b = a + b % (uint64_t)pow(2, 32);
        a+=sk[2*r+8] % (uint64_t)pow(2, 32);
        b+=sk[2*r+9] % (uint64_t)pow(2, 32);
        c = c ^ a;
        d = ROL(d, 1);
        d = d ^ b;
        c = ROR(c, 1);
        swap(a,c);
        swap(b,d);
    }
    swap(a,c);
    swap(b,d);
    a^=sk[4];
    b^=sk[5];
    c^=sk[6];
    d^=sk[7];
    STORE32L(a,&out[0]);
    STORE32L(b,&out[4]);
    STORE32L(c,&out[8]);
    STORE32L(d,&out[12]);
}

void decrypt(const uint8_t* in, uint8_t* out, const vector<uint32_t>& sk)
{
    uint32_t a, b, c, d;
    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);
    a^=sk[4];
    b^=sk[7];
    c^=sk[6];
    d^=sk[5];
    for (uint8_t r = 0; r < 16; r++)
    {
        b = ROL(b, 8);
        a = g(a);
        b = g(b);
        a = a + b % (uint64_t)pow(2, 32);
        b = a + b % (uint64_t)pow(2, 32);
        a+=sk[2*r+8] % (uint64_t)pow(2, 32);
        b+=sk[2*r+9] % (uint64_t)pow(2, 32);
        c = c ^ a;
        d = ROL(d, 1);
        d = d ^ b;
        c = ROR(c, 1);
        swap(a,c);
        swap(b,d);
    }
    swap(a,c);
    swap(b,d);
    a^=sk[0];
    b^=sk[1];
    c^=sk[2];
    d^=sk[3];
    STORE32L(a,&out[0]);
    STORE32L(b,&out[4]);
    STORE32L(c,&out[8]);
    STORE32L(d,&out[12]);
}

int main(int argc, char* argv[])
{
    ifstream inFile;
    ifstream keyFile;
    ofstream outFile;
    vector<uint32_t> sk;

    keyFile.open("key.txt", ios::in | ios::binary);
    //читаем файл с ключом
    if (!keyFile)
    {
        cout << "error key file";
        return 1;
    }
    keyFile.seekg(0, ios::end);
    int sizeK = keyFile.tellg();
    keyFile.seekg(0, ios::beg);
    if (sizeK > 256)
        cout << "error key length";
    else
    {
        uint8_t text[16];
        k = std::ceil((sizeK*8) / 64.0);
        uint8_t* key;
        key = new uint8_t[k * 8];
        memset(key, 0, k * 8);

        keyFile.read((char*)key, k * 8);

        cout<<"key:\n";
        for (int i = 0; i < k * 8; i++)
            cout << hex << uppercase << (uint32_t)key[i];
        vector<uint32_t> me;
        vector<uint32_t> mo;

        me.resize(k);
        mo.resize(k);
        v.resize(k);

        uint8_t j = 0,b0,b1,b2,b3;
        boost::qvm::mat <uint8_t, 4, 1>res;
        for (uint8_t i = 0; i <= 2*k-2; i+=2)
        {
            b0 = key[4 * i];
            b1 = key[4 * i + 1];
            b2 = key[4 * i + 2];
            b3 = key[4 * i + 3];
            me[j]=(b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
            j++;
        }
        j = 0;
        for (uint8_t i=1;i<=2*k-1;i+=2)
        {
            b0 = key[4 * i];
            b1 = key[4 * i + 1];
            b2 = key[4 * i + 2];
            b3 = key[4 * i + 3];
            mo[j]=(b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
            j++;
        }

        for(int i=k-1; i>=0; i--)
        {
            boost::qvm::mat<uint8_t, 8, 1> m =
            {
                key[8 * i],
                key[8 * i + 1] ,
                key[8 * i + 2],
                key[8 * i + 3],
                key[8 * i + 4],
                key[8 * i + 5],
                key[8 * i + 6],
                key[8 * i + 7]
            };
            res = M2 * m;

            for (uint8_t d = 0; d < 4; d++)
            {
                v[i] += res.a[0][d] * pow(2, 8 * d);//поправить
                cout<<endl<<v[i];
            }
        }
        sk.resize(40);
        for (uint8_t i = 1; i < 20; i++)
        {
            uint32_t Ai = h(2 * i * p, me);
            uint32_t Bi = ROL(h((2 * i + 1) * p, mo), 8);
            sk[2 * i] = Ai + Bi % int(pow(2, 32));
            sk[2 * i + 1] = ROL((Ai + 2 * Bi % int(pow(2, 32))), 9);
        }

        delete[] key;
    }
    keyFile.close();

    inFile.open("in.txt", ios::in | ios::binary);
    //читаем входной файл для дальнейшего шифрования
    if (!inFile.is_open())
    {
        cout << "error in.txt";
        return 1;
    }
    outFile.open("out.txt", ios::in | ios::binary);
    if(!outFile.is_open())
    {
        cout << "error out.txt";
        return 1;
    }
    uint8_t text[16];
    uint8_t out[16];

    while(!inFile.eof())
    {
        inFile.read((char*)text, 16);
        encrypt(text,out,sk);
        cout<<"\nencrypt:\n";
        for(uint8_t i=0; i<16;i++)
        {
            outFile<< hex << uppercase<<(uint32_t)out[i];
            cout<< hex << uppercase <<(uint32_t)out[i];
        }
    }
    decrypt(out,out,sk);
    outFile.close();
    inFile.close();
    cout<<"\n";

    return 0;
}
