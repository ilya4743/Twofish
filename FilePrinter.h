#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

class FilePrinter
{
private:
    ifstream inFile;
    ifstream keyFile;
    ofstream encryptFile;
    ofstream decryptFile;
public:
    vector<uint8_t> readKeyFile(const char* filename);
    int openInputFile(const char* filename);
    vector<uint8_t> readInputFile32();
    void closeInputFile();
    void openEncryptFile(const char* filename);
    void writeEncryptFile32(vector<uint8_t> encrypt);
    void writeEncryptFile32(vector<uint8_t>&& encrypt);
    void closeEncryptFile();
    void openDecryptFile(const char* filename);
    void writeDecryptFile32(vector<uint8_t> decrypt);
    void writeDecryptFile32(vector<uint8_t>&& decrypt);
    void closeDecryptFile();
};

