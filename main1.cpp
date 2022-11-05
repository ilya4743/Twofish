//#include "twofish.h"
//#include "FilePrinter.h"
//#include <iostream>
//#include <vector>

//using namespace std;
//int main1(int argc, char* argv[])
//{
//    try
//    {
//        vector<uint8_t>in;
//        vector<uint8_t>encrypt;
//        vector<uint8_t>decrypt;

//        Twofish twofish;
//        FilePrinter filePrinter;
//        vector<uint8_t> key=filePrinter.readKeyFile("key.txt");
//        int sizeInputFile=filePrinter.openInputFile("in.txt");
//        filePrinter.openEncryptFile("encrypt.txt");
//        filePrinter.openDecryptFile("decrypt.txt");
//        twofish.keySchedule(move(key));
//        for (int i = 0; i < sizeInputFile; i += 32)
//        {
//            in = filePrinter.readInputFile32();
//            encrypt=twofish.encrypt(move(in));
//            filePrinter.writeEncryptFile32(encrypt);
//            decrypt=twofish.decrypt(move(encrypt));
//            filePrinter.writeDecryptFile32(decrypt);
//        }
//        filePrinter.closeInputFile();
//        filePrinter.closeEncryptFile();
//        filePrinter.closeDecryptFile();
//    }
//    catch(exception e)
//    {
//        cout << e.what();
//    }
//    return 0;
//}
