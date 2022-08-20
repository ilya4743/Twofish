#include <cstring>
#include<cmath>
#include<vector>
#include <fstream>
#include <iostream>
#include <boost/qvm/mat.hpp>
#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>

using namespace std;
typedef unsigned char byte;
typedef unsigned long byte4;
byte k;
#define STORE32L(x, y)  { ulong32 __t = (x); memcpy(y, &__t, 4); }
#define LOAD32L(x, y)   { memcpy(&(x), y, 4); x &= 0xFFFFFFFF; }

#define ROL(x, y) ( (((unsigned long)(x)<<(unsigned long)((y)&31)) | (((unsigned long)(x)&0xFFFFFFFFUL)>>(unsigned long)(32-((y)&31)))) & 0xFFFFFFFFUL)
#define ROR(x, y) ( ((((unsigned long)(x)&0xFFFFFFFFUL)>>(unsigned long)((y)&31)) | ((unsigned long)(x)<<(unsigned long)(32-((y)&31)))) & 0xFFFFFFFFUL)

#define ROL4L(x) (((x << 1) & 0x0F | ((x & 0x08) >> 3)) | (x & 0xF0))
#define ROL4H(x) (((x << 1) & 0xF0 | ((x & 0x80) >> 3)) | (x & 0x0F))

#define ROR4L(x) (((x >> 1) & 0x0F | ((x & 0x01) << 3)) | (x & 0xF0))
#define ROR4H(x) (((x >> 1) & 0xF0 | ((x & 0x10) << 3)) | (x & 0x0F))

void encrypt(const unsigned char* in, unsigned char* out)
{
    byte4 a, b, c, d;
    //ta, tb, tc, td, t1, t2, * k;

    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);
    for (byte r = 0; r < 16; r++)
    {
        b = ROL(b, 8);
        a = g(a);
        b = g(b);
        a = a + b % (byte4)pow(2, 32);
        b = a + b % (byte4)pow(2, 32);
        a;
        b;
        c = c ^ a;
        d = ROL(d, 1);
        d = d ^ b;
        c = ROR(c, 1);
    }
}

const byte4 p = 16843009;

boost::qvm::mat<byte, 4, 4> M1 =
{
    0x01, 0xEF, 0x5B, 0x5B,
    0x5B, 0xEF, 0xEF, 0x01,
    0xEF, 0X5B, 0X01, 0XEF,
    0xEF, 0X01, 0XEF, 0X5B
};

boost::qvm::mat<byte, 4, 8> M2 =
{
    0x01, 0xA4, 0x55, 0x87, 0x5A, 0x58, 0xDB, 0x9E,
    0xA4, 0x56, 0x82, 0xF3, 0X1E, 0XC6, 0X68, 0XE5,
    0X02, 0XA1, 0XFC, 0XC1, 0X47, 0XAE, 0X3D, 0X19,
    0XA4, 0X55, 0X87, 0X5A, 0X58, 0XDB, 0X9E, 0X03
};

boost::qvm::mat<byte, 4, 16> Tq0 =
{
    0x8, 0x1, 0x7, 0xD, 0x6, 0xF, 0x3, 0x2, 0x0, 0xB, 0x5, 0x9, 0xE, 0xC, 0xA, 0x4,
    0xE, 0XC, 0XB, 0X8, 0X1, 0X2, 0X3, 0X5, 0XF, 0X4, 0XA, 0X6, 0X7, 0X0, 0X9, 0XD,
    0XB, 0XA, 0X5, 0XE, 0X6, 0XD, 0X9, 0X0, 0XC, 0X8, 0XF, 0X3, 0X2, 0X4, 0X7, 0X1,
    0XD, 0X7, 0XF, 0X4, 0X1, 0X2, 0X6, 0XE, 0X9, 0XB, 0X3, 0X0, 0X8, 0X5, 0XC, 0XA
};

boost::qvm::mat<byte, 4, 16> Tq1 =
{
    0X2, 0X8, 0XB, 0XD, 0XF, 0X7, 0X6, 0XE, 0X3, 0X1, 0X9, 0X4, 0X0, 0XA, 0XC, 0X5,
    0X1, 0XE, 0X2, 0XB, 0X4, 0XC, 0X3, 0X7, 0X6, 0XD, 0XA, 0X5, 0XF, 0X9, 0X0, 0X8,
    0X4, 0XC, 0X7, 0X5, 0X1, 0X6, 0X9, 0XA, 0X0, 0XE, 0XD, 0X8, 0X2, 0XB, 0X3, 0XF,
    0xB, 0X9, 0X5, 0X1, 0XC, 0X3, 0XD, 0XE, 0X6, 0X4, 0X7, 0XF, 0X2, 0X0, 0X8, 0XA
};
//res = (n1 << 24) | (n2 << 16) | (n3 << 8) | n4;

byte q0(byte x)
{
    byte a0, b0, a2, b2, a4, b4;
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

byte q1(byte x)
{
    byte a0, b0, a2, b2, a4, b4;
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
    byte4 y = 16 * b4 + a4;
    return 16 * b4 + a4;
}
vector<byte> v;
byte4 g(byte4 b)
{
    byte b0, b1, b2, b3;
    b = h(b, v);
    b0 = b & 0xFF;
    b1 = b >> 8 & 0xFF;
    b2 = b >> 16 & 0xFF;
    b3 = b >> 24 & 0xFF;
    boost::qvm::mat <byte, 4, 1>s = { b0,b1,b2,b3 };
    boost::qvm::mat <byte, 4, 1>res = M1 * s;
    return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}

byte4 h(byte4 b, vector<unsigned char>& me)
{
    byte b0, b1, b2, b3;

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
        byte4 Me3 = (me[15] << 24) | (me[14] << 16) | (me[13] << 8) | me[12];
        b ^= Me3;

        b0 = b & 0xFF;
        b1 = b >> 8 & 0xFF;
        b2 = b >> 16 & 0xFF;
        b3 = b >> 24 & 0xFF;
        b0 = q1(b0);
        b1 = q1(b1);
        b2 = q0(b2);
        b3 = q0(b3);
        b = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
        byte4 Me2 = (me[11] << 24) | (me[10] << 16) | (me[9] << 8) | me[8];
        b ^= Me2;
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
        byte4 Me2 = (me[11] << 24) | (me[10] << 16) | (me[9] << 8) | me[8];
        b ^= Me2;
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
    byte4 Me1 = (me[7] << 24) | (me[6] << 16) | (me[5] << 8) | me[4];
    b ^= Me1;

    b0 = b & 0xFF;
    b1 = b >> 8 & 0xFF;
    b2 = b >> 16 & 0xFF;
    b3 = b >> 24 & 0xFF;
    b0 = q0(b0);
    b1 = q0(b1);
    b2 = q1(b2);
    b3 = q1(b3);
    b = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
    byte4 Me0 = (me[3] << 24) | (me[2] << 16) | (me[1] << 8) | me[0];
    b ^= Me0;

    b0 = b & 0xFF;
    b1 = b >> 8 & 0xFF;
    b2 = b >> 16 & 0xFF;
    b3 = b >> 24 & 0xFF;
    b0 = q1(b0);
    b1 = q0(b1);
    b2 = q1(b2);
    b3 = q0(b3);

    boost::qvm::mat <byte, 4, 1>y = { b0,b1,b2,b3 };
    boost::qvm::mat <byte, 4, 1>res = M1 * y;
    byte4 H = 0;
    for (byte i = 0; i < 4; i++)
        H += res.a[0][i] * pow(2, 8 * i);
    return H;
}

int main(int argc, char* argv[])
{
    ifstream inFile;
    ifstream keyFile;

    byte text[16];
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
        k = std::ceil((sizeK) / 64.0);
        byte* key;
        key = new byte[k * 64];
        memset(key, 0, k * 64);

        keyFile.read((char*)key, k * 64);

        for (int i = 0; i < k * 64; i++)
            cout << key[i];

        vector<byte> me;
        vector<byte> mo;
        me.resize(4 * k);
        mo.resize(4 * k);
        v.resize(k);

        int j = 0;
        for (byte i = 0; i < 2 * k; i++)
        {
            me[4 * j] = key[4 * i];
            me[4 * j + 1] = key[4 * i + 1];
            me[4 * j + 2] = key[4 * i + 2];
            me[4 * j + 3] = key[4 * i + 3];

            i++;
            mo[4 * j] = key[4 * i];
            mo[4 * j + 1] = key[4 * i + 1];
            mo[4 * j + 2] = key[4 * i + 2];
            mo[4 * j + 3] = key[4 * i + 3];

            boost::qvm::mat<byte, 8, 1> m =
            {
                me[4 * j],
                me[4 * j + 1] ,
                me[4 * j + 2],
                me[4 * j + 3],
                mo[4 * j],
                mo[4 * j + 1],
                mo[4 * j + 2],
                mo[4 * j + 3]
            };
            boost::qvm::mat <byte, 4, 1>res = M2 * m;

            for (int d = 0; d < 4; d++)
                v[k - j - 1] += res.a[0][d] * pow(2, 8 * d);

            j++;
        }
        vector<byte4> sk;
        sk.resize(40);
        for (byte i = 1; i < 20; i++)
        {
            byte4 Ai = h(2 * i * p, me);
            byte4 Bi = ROL(h((2 * i + 1) * p, mo), 8);
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
    // while (!inFile.atEnd())
     //{
     //    int j = inFile.read((char*)text, 16);
     //    for (int i = 0; i < j; i++)
     //        cout << text[i];
     //}
    inFile.close();


    return 0;
}
