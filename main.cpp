#include "Twofish.h"
#include "FilePrinter.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
int main(int argc, char* argv[])
{
    try
    {
        uint8_t out[16];

        Twofish twofish;
        FilePrinter filePrinter;
        pair<uint8_t*, int> key=filePrinter.readKeyFile("key.txt");
        int sizeInputFile=filePrinter.openInputFile("in.txt");
        filePrinter.openEncryptFile("encrypt.txt");
        filePrinter.openDecryptFile("decrypt.txt");
        twofish.keySchedule(key.first, key.second);
        for (int i = 0; i < sizeInputFile; i += 32)
        {
            twofish.encrypt(filePrinter.readInputFile32(), out);
            filePrinter.writeEncryptFile32(out);
            twofish.decrypt(out, out);
            filePrinter.writeDecryptFile32(out);
        }
        filePrinter.closeInputFile();
        filePrinter.closeEncryptFile();
        filePrinter.closeDecryptFile();
    }
    catch(exception e)
    {
        cout << e.what();
    }
    return 0;
}
