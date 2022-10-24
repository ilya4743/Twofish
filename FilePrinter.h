#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class FilePrinter
{
private:
    ifstream inFile;
    ifstream keyFile;
    ofstream encryptFile;
    ofstream decryptFile;
public:
    pair<uint8_t*,int> readKeyFile(const char* filename);
    int openInputFile(const char* filename);
    const uint8_t* readInputFile32();
    void closeInputFile();
    void openEncryptFile(const char* filename);
    void writeEncryptFile32(const uint8_t* encrypt);
    void closeEncryptFile();
    void openDecryptFile(const char* filename);
    void writeDecryptFile32(const uint8_t* decrypt);
    void closeDecryptFile();
};

