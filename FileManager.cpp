#include "FileManager.h"

string FileManager::loadFile(string&& filename)
{
    ifstream inFile;
    try
    {    
        string data;
        inFile.open(filename, ios::in | ios::binary);
        if (!inFile)
        {
            string msg(filename+"\nошибка открытия, возможно файл не найден");
            throw myexception(msg);
        }

        inFile.seekg(0, ios::end);
        int length = inFile.tellg();
        inFile.seekg(0, ios::beg);
        data.reserve(length);
        for (int i = 0; i < length-1; i++)
        {
            char c=0;
            inFile >> c;
            data.push_back(c);
        }

        inFile.close();
        return data;
    }
    catch (myexception& e)
    {
        throw   e;
    }
}

void FileManager::saveFile(string&& filename, string&& text)
{
    ofstream outFile;
    try
    {
        outFile.open(filename, ios::out | ios::binary);
        if (!outFile.is_open())
        {
            string msg=filename+"\nошибка открытия файла";
            throw myexception(msg);
        }

        for (int i = 0; i < text.size(); i++)
            outFile <<text[i];

        outFile.close();
    }
    catch (myexception& e)
    {
        throw myexception();
    }
}
