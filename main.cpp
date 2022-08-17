#include <QCoreApplication>
#include<QFile>
#include<QDebug>
#include <QGenericMatrix>

#include<cmath>

#define STORE32L(x, y)  { ulong32 __t = (x); memcpy(y, &__t, 4); }
#define LOAD32L(x, y)   { memcpy(&(x), y, 4); x &= 0xFFFFFFFF; }

#define ROL(x, y) ( (((unsigned long)(x)<<(unsigned long)((y)&31)) | (((unsigned long)(x)&0xFFFFFFFFUL)>>(unsigned long)(32-((y)&31)))) & 0xFFFFFFFFUL)
#define ROR(x, y) ( ((((unsigned long)(x)&0xFFFFFFFFUL)>>(unsigned long)((y)&31)) | ((unsigned long)(x)<<(unsigned long)(32-((y)&31)))) & 0xFFFFFFFFUL)

#define ROR4(x, y) ( ((((unsigned char)(x)&0xFFFFFFFFUL)>>(unsigned char)((y)&31)) | ((unsigned long)(x)<<(unsigned long)(32-((y)&31)))) & 0xFFFFFFFFUL)
#define ROL4(x, y) ( (((unsigned char)(x)<<(unsigned char)((y)&15)) | (((unsigned char)(x)&0xF)>>(unsigned char)(16-((y)&15)))) & 0xF)

void encrypt(const unsigned char* in, unsigned char* out)
{
    unsigned long a,b,c,d,ta,tb,tc,td,t1,t2, *k;
    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);

}

unsigned char shift1 (unsigned char b) {
 unsigned char bit=b&0x08?1:0;
 unsigned char right=((b<<1)&0x0E)|bit;
 bit=b&0x10?0x80:0;
 unsigned char left=bit|((b>>1)&0x70);
 return ~(left|right);
}

unsigned char q0(unsigned char x)
{
    unsigned char a0,b0,a2,b2;
    a0=x/16;
    b0=x%16;
    a0=(a0^b0)<<4;
    unsigned char b=17;
    b=ROL4(b,1);

    unsigned char bit=17;
    unsigned char leftL = 17 << 1 & 0x0F | 17 & 0xF0;
    unsigned char leftH = 17 << 1 & 0xF0 | 17 & 0x0F;

    unsigned char rightL = 17 >> 1 & 0x0F | 17 & 0xF0;
    unsigned char rightH = 17 >> 1 & 0xF0;
//http://blog.kislenko.net/show.php?id=1081
}

void q1()
{

}

void h(unsigned long d, QVector<unsigned char>& me)
{
    QVector<unsigned char> D;
    D.reserve(4);
    D.append(d&0x000000FF);
    D.append(d>>8&0x000000FF);
    D.append(d>>16&0x000000FF);
    D.append(d>>24&0x000000FF);
    q0(22);

}

 #include <QtMath>
int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QVector<unsigned char>l;
    h(260,l);
    QGenericMatrix<4,4,unsigned char> m;
    m=(unsigned char)5*m;
    unsigned long g;

    unsigned char M1[16]=
    {
        0x01, 0xEF, 0x5B, 0x5B,
        0x5B, 0xEF, 0xEF, 0x01,
        0xEF, 0X5B, 0X01, 0XEF,
        0xEF, 0X01, 0XEF, 0X5B
    };

    unsigned char M2[32]=
    {
        0x01, 0xA4, 0x55, 0x87, 0x5A, 0x58, 0xDB, 0x9E,
        0xA4, 0x56, 0x82, 0xF3, 0X1E, 0XC6, 0X68, 0XE5,
        0X02, 0XA1, 0XFC, 0XC1, 0X47, 0XAE, 0X3D, 0X19,
        0XA4, 0X55, 0X87, 0X5A, 0X58, 0XDB, 0X9E, 0X03
    };
    QGenericMatrix<4,8,unsigned char> QM1(M1);
    QGenericMatrix<4,8,unsigned char> QM2(M2);
    unsigned char Tq0[64]=
    {
        0x8, 0x1, 0x7, 0xD, 0x6, 0xF, 0x3, 0x2, 0x0, 0xB, 0x5, 0x9, 0xE, 0xC, 0xA, 0x4,
        0xE, 0XC, 0XB, 0X8, 0X1, 0X2, 0X3, 0X5, 0XF, 0X4, 0XA, 0X6, 0X7, 0X0, 0X9, 0XD,
        0XB, 0XA, 0X5, 0XE, 0X6, 0XD, 0X9, 0X0, 0XC, 0X8, 0XF, 0X3, 0X2, 0X4, 0X7, 0X1,
        0XD, 0X7, 0XF, 0X4, 0X1, 0X2, 0X6, 0XE, 0X9, 0XB, 0X3, 0X0, 0X8, 0X5, 0XC, 0XA
    };

    unsigned char Tq1[64]=
    {
        0X2, 0X8, 0XB, 0XD, 0XF, 0X7, 0X6, 0XE, 0X3, 0X1, 0X9, 0X4, 0X0, 0XA, 0XC, 0X5,
        0X1, 0XE, 0X2, 0XB, 0X4, 0XC, 0X3, 0X7, 0X6, 0XD, 0XA, 0X5, 0XF, 0X9, 0X0, 0X8,
        0X4, 0XC, 0X7, 0X5, 0X1, 0X6, 0X9, 0XA, 0X0, 0XE, 0XD, 0X8, 0X2, 0XB, 0X3, 0XF,
        0xB, 0X9, 0X5, 0X1, 0XC, 0X3, 0XD, 0XE, 0X6, 0X4, 0X7, 0XF, 0X2, 0X0, 0X8, 0XA
    };

    QFile inFile("in.txt"), keyFile("key.txt"), outFile("out.txt");
    unsigned char text[16];

    //читаем файл с ключом
    if(!keyFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"error key file";
        return a.exec();
    }
    if(keyFile.size()>256)
        qDebug()<<"error key length";
    else
    {
        int k=std::ceil((keyFile.size()-1)/64.0);
        unsigned char* key;
        key=new unsigned char[k*64];
        memset(key, 0, k*64);

        keyFile.read((char*)key,keyFile.size()-1);

        for(int i=0; i<k*64; i++)
            qDebug()<<key[i];

        QVector<unsigned char> me;
        QVector<unsigned char> mo;
        QVector<unsigned long> v;
        me.reserve(4*k);
        mo.reserve(4*k);
        v.resize(k);

        int j=0;
        for(int i=0; i<2*k; i++)
        {
            me.append(key[4*i]);
            me.append(key[4*i+1]);
            me.append(key[4*i+2]);
            me.append(key[4*i+3]);

            i++;
            mo.append(key[4*i]);
            mo.append(key[4*i+1]);
            mo.append(key[4*i+2]);
            mo.append(key[4*i+3]);

            QVector<unsigned char> vec;
            vec.reserve(k);

            vec.push_back(me[4*j]);
            vec.push_back(me[4*j+1]);
            vec.push_back(me[4*j+2]);
            vec.push_back(me[4*j+3]);
            vec.push_back(mo[4*j]);
            vec.push_back(mo[4*j+1]);
            vec.push_back(mo[4*j+2]);
            vec.push_back(mo[4*j+3]);

            QGenericMatrix<8,1,unsigned char> m(vec.constBegin());
            QGenericMatrix<4,1,unsigned char> res=m*QM2;

            for(int d=0;d<4;d++)
                v[k-j-1]+=res(0,d)*qPow(2,8*d);
            j++;
        }
        delete [] key;
    }
    keyFile.close();

    //читаем входной файл для дальнейшего шифрования
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"error in.txt";
        return a.exec();
    }
    while(!inFile.atEnd())
    {
        int j=inFile.read((char*)text,16);
        for(int i=0; i<j; i++)
            qDebug()<<text[i];
    }
    inFile.close();

    return a.exec();
}
