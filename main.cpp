#include <QCoreApplication>
#include<QFile>
#include<QDebug>
#include<cmath>

#define STORE32L(x, y)  { ulong32 __t = (x); memcpy(y, &__t, 4); }
#define LOAD32L(x, y)   { memcpy(&(x), y, 4); x &= 0xFFFFFFFF; }

#define ROL(x, y) ( (((unsigned long)(x)<<(unsigned long)((y)&31)) | (((unsigned long)(x)&0xFFFFFFFFUL)>>(unsigned long)(32-((y)&31)))) & 0xFFFFFFFFUL)
#define ROR(x, y) ( ((((unsigned long)(x)&0xFFFFFFFFUL)>>(unsigned long)((y)&31)) | ((unsigned long)(x)<<(unsigned long)(32-((y)&31)))) & 0xFFFFFFFFUL)

void encrypt(const unsigned char* in, unsigned char* out)
{
    unsigned long a,b,c,d,ta,tb,tc,td,t1,t2, *k;
    LOAD32L(a, &in[0]);
    LOAD32L(b, &in[4]);
    LOAD32L(c, &in[8]);
    LOAD32L(d, &in[12]);

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    unsigned long A=0x0000000f;
    unsigned long B=0xfffffffF;
    unsigned long g;
    A^=B;
    unsigned char M1[4][4]=
    {
        { 0x01, 0xEF, 0x5B, 0x5B},
        { 0x5B, 0xEF, 0xEF, 0x01},
        { 0xEF, 0X5B, 0X01, 0XEF},
        { 0xEF, 0X01, 0XEF, 0X5B}
    };

    unsigned char M2[4][8]=
    {
        { 0x01, 0xA4, 0x55, 0x87, 0x5A, 0x58, 0xDB, 0x9E },
        { 0xA4, 0x56, 0x82, 0xF3, 0X1E, 0XC6, 0X68, 0XE5 },
        { 0X02, 0XA1, 0XFC, 0XC1, 0X47, 0XAE, 0X3D, 0X19 },
        { 0XA4, 0X55, 0X87, 0X5A, 0X58, 0XDB, 0X9E, 0X03 }
    };

    unsigned char Tq0[4][16]=
    {
       { 0x8, 0x1, 0x7, 0xD, 0x6, 0xF, 0x3, 0x2, 0x0, 0xB, 0x5, 0x9, 0xE, 0xC, 0xA, 0x4 },
       { 0xE, 0XC, 0XB, 0X8, 0X1, 0X2, 0X3, 0X5, 0XF, 0X4, 0XA, 0X6, 0X7, 0X0, 0X9, 0XD },
       { 0XB, 0XA, 0X5, 0XE, 0X6, 0XD, 0X9, 0X0, 0XC, 0X8, 0XF, 0X3, 0X2, 0X4, 0X7, 0X1 },
       { 0XD, 0X7, 0XF, 0X4, 0X1, 0X2, 0X6, 0XE, 0X9, 0XB, 0X3, 0X0, 0X8, 0X5, 0XC, 0XA }
    };

    unsigned char Tq1[4][16]=
    {
       { 0X2, 0X8, 0XB, 0XD, 0XF, 0X7, 0X6, 0XE, 0X3, 0X1, 0X9, 0X4, 0X0, 0XA, 0XC, 0X5 },
       { 0X1, 0XE, 0X2, 0XB, 0X4, 0XC, 0X3, 0X7, 0X6, 0XD, 0XA, 0X5, 0XF, 0X9, 0X0, 0X8 },
       { 0X4, 0XC, 0X7, 0X5, 0X1, 0X6, 0X9, 0XA, 0X0, 0XE, 0XD, 0X8, 0X2, 0XB, 0X3, 0XF },
       { 0xB, 0X9, 0X5, 0X1, 0XC, 0X3, 0XD, 0XE, 0X6, 0X4, 0X7, 0XF, 0X2, 0X0, 0X8, 0XA }
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

        unsigned char* me=new unsigned char[4*k];
        unsigned char* mo=new unsigned char[4*k];
        unsigned long* v =new unsigned long[k];
        int j=0;
        for(int i=0; i<2*k; i+=1)
        {
            me[4*j]=key[4*i];
            me[4*j+1]=key[4*i+1];
            me[4*j+2]=key[4*i+2];
            me[4*j+3]=key[4*i+3];
            i++;
            mo[4*j]=key[4*i];
            mo[4*j+1]=key[4*i+1];
            mo[4*j+2]=key[4*i+2];
            mo[4*j+3]=key[4*i+3];
            j++;
        }
        delete [] v;
        delete [] me;
        delete [] mo;
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
