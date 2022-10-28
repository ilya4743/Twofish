#include "FilePrinter.h"
#include <string>
#include <iomanip>

pair<uint8_t*, int> FilePrinter::readKeyFile(const char* filename)
{
    try
    {
        char* key;
        keyFile.open(filename, ios::in | ios::binary);
        if (!keyFile)
            throw exception("file with key not found");
        keyFile.seekg(0, ios::end);
        int length = keyFile.tellg();
        keyFile.seekg(0, ios::beg);
        if ((length - 1) * 4 > 256)
            throw exception("error key length");
        else
        {
            key = new char[length];
            //memset(key, 0, sizeK-1);
            for (int i = 0; i < length; i++)
            {
                char c;
                keyFile >> c;
                key[i] = c;
            }
        }
        keyFile.close();
        return pair<uint8_t*, int>((uint8_t*)key, (length * 4)/64);
    }
    catch (exception e)
    {
        cout<<e.what();
    }
}

int FilePrinter::openInputFile(const char* filename)
{
    try
    {
        inFile.open(filename, ios::in | ios::binary);
        if (!inFile.is_open())
            throw exception(filename);
        inFile.seekg(0, ios::end);
        int length = inFile.tellg();
        inFile.seekg(0, ios::beg);
        return length;
    }
    catch (exception e)
    {
        e.what();
    }
}

const uint8_t* FilePrinter::readInputFile32()
{
    string s;
    
    uint8_t *text, c;
    text = new uint8_t[16];
    int j = 0;
    while (!inFile.eof()&&j!= 16)
    {
            inFile >>c;
            text[j] = strtoul((char*)&c,0,16)<<4;
            inFile >> c;
            text[j++] |= strtoul((char*)&c, 0, 16);

            cout << c;
    }
    cout << '\n';
    return text;
}

void FilePrinter::closeInputFile()
{
    inFile.close();
}

void FilePrinter::openEncryptFile(const char* filename)
{
    try
    {
        encryptFile.open(filename, ios::out | ios::binary);
        if (!encryptFile.is_open())
            throw exception(filename);
    }
    catch (exception e)
    {
        e.what();
    }
}

void FilePrinter::writeEncryptFile32(const uint8_t* encrypt)
{
    for (int i = 0; i < 16; i++)
        encryptFile << hex << uppercase << setw(2)<< setfill('0') <<(uint32_t)encrypt[i];
}

void FilePrinter::closeEncryptFile()
{
    encryptFile.close();
}

void FilePrinter::openDecryptFile(const char* filename)
{
    try
    {
        decryptFile.open(filename, ios::out | ios::binary);
        if (!decryptFile.is_open())
            throw exception(filename);
    }
    catch (exception e)
    {
        e.what();
    }
}

void FilePrinter::writeDecryptFile32(const uint8_t* decrypt)
{
    for (int i = 0; i < 16; i++)
    {
        decryptFile << hex << uppercase << (uint32_t)decrypt[i];
        cout << (uint32_t)decrypt[i];
    }
}

void FilePrinter::closeDecryptFile()
{
    decryptFile.close();
}