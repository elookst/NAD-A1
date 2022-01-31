#pragma once



#include <iostream>
#include <fstream>
#include <string>
#include "Net.h"



using namespace std;

class FileReader
{

private:

	string fileType; //-t for text, -b for binary

	// list for storing packets created?

public:

	FileReader(char* fileName, char* fileType); // constructor

	int CreatePackets();

};
