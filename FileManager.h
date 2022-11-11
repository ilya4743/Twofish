#pragma once
#include <fstream>
#include <string>
#include "myexception.h"

using namespace std;

class FileManager
{
private:

public:
    string loadFile(string &&filename);
    void saveFile(string&& filename, string&& text);
};
