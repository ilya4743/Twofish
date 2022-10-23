#include <cstring>
#include<cmath>
#include<vector>
#include "Header.h"
#include <fstream>
#include <iostream>
#include <boost/qvm/mat.hpp>
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

boost::qvm::mat<uint8_t, 4, 4> MDS =
{
    0x01, 0xEF, 0x5B, 0x5B,
    0x5B, 0xEF, 0xEF, 0x01,
    0xEF, 0x5B, 0x01, 0xEF,
    0xEF, 0x01, 0xEF, 0x5B
};

boost::qvm::mat<uint8_t, 4, 8> RS =
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

uint8_t q0(uint8_t x)
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

uint8_t q1(uint8_t x)
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
vector<vector<uint8_t>> v;

uint32_t h(uint32_t x, const vector< vector<uint8_t>>& l)
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
        b[0] = q1(b[0]) ^ l[3][0];
        b[1] = q1(b[1]) ^ l[3][1];
        b[2] = q0(b[2]) ^ l[3][2];
        b[3] = q0(b[3]) ^ l[3][3];

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

void encrypt(const uint8_t* in, uint8_t* out, const vector<uint32_t>& sk)
{
    uint32_t a, b, c, d;
    uint32_t t0, t1;
    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);
    cout << hex << uppercase << a << '\t' << b << '\t' << c << '\t' << d << endl;

    a ^= sk[0];
    b ^= sk[1];
    c ^= sk[2];
    d ^= sk[3];
    cout << "whitenning in\n";
    cout << hex << uppercase << a << '\t' << b << '\t' << c << '\t' << d << endl;

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
    cout << "whitenning out\n";
    cout << hex << uppercase << a << '\t' << b << '\t' << c << '\t' << d << endl;
    STORE32L(a, &out[0]);
    STORE32L(b, &out[4]);
    STORE32L(c, &out[8]);
    STORE32L(d, &out[12]);
}

void decrypt(const uint8_t* in, uint8_t* out, const vector<uint32_t>& sk)
{
    uint32_t a, b, c, d;
    uint32_t t0, t1;
    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);
    cout << hex << uppercase << a << '\t' << b << '\t' << c << '\t' << d << endl;

    a ^= sk[4];
    b ^= sk[5];
    c ^= sk[6];
    d ^= sk[7];
    cout << "whitenning in\n";
    cout << hex << uppercase << a << '\t' << b << '\t' << c << '\t' << d << endl;

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
    cout << "whitenning out\n";
    cout << hex << uppercase << a << '\t' << b << '\t' << c << '\t' << d << endl;
    STORE32L(a, &out[0]);
    STORE32L(b, &out[4]);
    STORE32L(c, &out[8]);
    STORE32L(d, &out[12]);
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
    if ((sizeK - 1) * 4 > 256)
        cout << "error key length";
    else
    {
        k = std::ceil((sizeK - 1) * 4 / 64.0);
        int8_t key[32];
        //key = new uint8_t[sizeK-1];
        //memset(key, 0, sizeK-1);
        cout << "Key: ";
        for (int i = 0; i < sizeK - 1; i++)
        {
            char c;
            keyFile >> c;
            cout << c;
            key[i] = strtoul(&c, NULL, 0);
        }
        cout << endl << endl;
        vector<uint32_t> m;
        vector<vector<uint8_t>> me;
        vector<vector<uint8_t>> mo;
        for (int i = 0; i < sizeK / 8; i++)
        {
            uint32_t tmp=0;
            for (int j = 0; j < 4; j++)
            {
                tmp += key[4 * i + j] * (2 << (8 * j));
            }
            m.push_back(tmp);
        }
        

        v.resize(k);

        uint8_t j = 0, b0, b1, b2, b3;
        boost::qvm::mat <uint8_t, 4, 1>res;

        cout << "Me: ";
        for (uint8_t i = 0; i <= 2 * k - 2; i += 2)
        {
            me.push_back(vector<uint8_t>());
            me[j].push_back(key[4 * i]);
            me[j].push_back(key[4 * i + 1]);
            me[j].push_back(key[4 * i + 2]);
            me[j].push_back(key[4 * i + 3]);
            uint32_t hh = (me[j][0]<<24) | (me[j][1] << 16) | (me[j][2] << 8) | (me[j][3]);
            cout << hex << uppercase<< hh << ",\t";
            j++;
        }
        j = 0;
        cout << "\nMo: ";
        for (uint8_t i = 1; i <= 2 * k - 1; i += 2)
        {
            mo.push_back(vector<uint8_t>());
            mo[j].push_back(key[4 * i]);
            mo[j].push_back(key[4 * i + 1]);
            mo[j].push_back(key[4 * i + 2]);
            mo[j].push_back(key[4 * i + 3]);
            uint32_t hh = mo[j][0] | (mo[j][1] << 8) | (mo[j][2] << 16) | (mo[j][3] << 24);
            cout << hex << uppercase<<hh << ",\t";
            j++;
        }
        j = 0;
        cout << "\nV:  ";
        for (int i = k - 1; i >= 0; i--)
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
            res = RS * m;
            v.push_back(vector<uint8_t>());
            for (uint8_t d = 0; d < 4; d++)
            {
                v[j].push_back(res.a[d][0]);
                //v[i] += res.a[d][0] * (2 << (8 * d));//поправить
            }
            j++;
        }
        sk.resize(40);
        cout << "\n\nSubkeys: \n";
        for (uint8_t i = 0; i < 20; i++)
        {
            uint32_t Ai = h(2 * i * p, me);
            uint32_t Bi = ROL(h((2 * i + 1) * p, mo), 8);
            sk[2 * i] = Ai + Bi;
            sk[2 * i + 1] = ROL((Ai + 2 * Bi), 9);
        }

        //delete[] key;
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
    if (!outFile.is_open())
    {
        cout << "error out.txt";
        return 1;
    }
    uint8_t text[32];
    uint8_t out[32];

    while (!inFile.eof())
    {
        cout << "\ntext\n";
        for (int i = 0; i < 32; i++)
        {
            char c;
            inFile >> c;
            cout << c;
            text[i] = c;
        }
        cout << "\n\nencrypt:\n";
        encrypt(text, out, sk);
        for (uint8_t i = 0; i < 16; i++)
        {
            outFile << hex << uppercase << (uint32_t)out[i];
            cout << hex << uppercase << (uint32_t)out[i];
        }
        char c;
        inFile >> c;
    }
    cout << "\n\decrypt:\n";
    decrypt(out, out, sk);
    outFile.close();
    inFile.close();
    cout << "\n";

    return 0;
}
