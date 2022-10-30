#include "FilePrinter.h"

vector<uint8_t> FilePrinter::readKeyFile(const char* filename)
{
    try
    {    
        vector<uint8_t> key;
        keyFile.open(filename, ios::in | ios::binary);
        if (!keyFile)
            throw exception();
        keyFile.seekg(0, ios::end);
        int length = keyFile.tellg();
        keyFile.seekg(0, ios::beg);
        if ((length - 1) * 4 > 256)
            throw exception();
        //if ((length-1)%64)

        else
        {        
            key.reserve(length);
            for (int i = 0; i < length; i++)
            {
                char c=0;
                keyFile >> c;
                key.push_back(c);
            }
        }
        keyFile.close();
        return key;
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
            throw exception();
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

vector<uint8_t> FilePrinter::readInputFile32()
{
    vector<uint8_t> text;
    text.reserve(16);
    int j = 0;
    while (!inFile.eof()&&j!= 16)
    {    
        uint8_t c = 0;
        inFile >>c;
        text.push_back(strtoul((char*)&c, 0, 16) << 4);
        text[j] = strtoul((char*)&c,0,16)<<4;
        c = 0;
        inFile >> c;
        text[j] |= strtoul((char*)&c, 0, 16);
        j++;
    }
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
            throw exception();
    }
    catch (exception e)
    {
        e.what();
    }
}

void FilePrinter::writeEncryptFile32(vector<uint8_t>&& encrypt)
{
    for (int i = 0; i < encrypt.size(); i++)
        encryptFile << hex << uppercase << setw(2)<< setfill('0') << static_cast<uint32_t>(encrypt[i]);
}

void FilePrinter::writeEncryptFile32(vector<uint8_t> encrypt)
{
    for (int i = 0; i < encrypt.size(); i++)
        encryptFile << hex << uppercase << setw(2) << setfill('0') << static_cast<uint32_t>(encrypt[i]);
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
            throw exception();
    }
    catch (exception e)
    {
        e.what();
    }
}

void FilePrinter::writeDecryptFile32(vector<uint8_t> decrypt)
{
    for (int i = 0; i < decrypt.size(); i++)
        decryptFile << hex << uppercase << static_cast<uint32_t>(decrypt[i]);
}

void FilePrinter::writeDecryptFile32(vector<uint8_t>&& decrypt)
{
    for (int i = 0; i < decrypt.size(); i++)
        decryptFile << hex << uppercase << static_cast<uint32_t>(decrypt[i]);
}

void FilePrinter::closeDecryptFile()
{
    decryptFile.close();
}
