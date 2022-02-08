// NAME				:	FileCreator.cpp
// PROJECT			:	Network Application Development A1
// PROGRAMMER		:
// FIRST-VERSION	:

#include <iostream>
#include <fstream>
#include <string>
#include "fileReader.h"
#include "fileCreator.h"
#include "md5.h"

using namespace std;

#pragma warning(disable:4996)

// sets blank filename etc.
FileCreator::FileCreator()
{
	this->fileName = "";
	this->fileType = "-t";
	this->recievedHash = "";
	this->fp = NULL;
	this->fileSize = 0;
}


void FileCreator::SetFileSize(int size)
{
	this->fileSize = size;
}

int FileCreator::GetFileSize(void)
{
	return this->fileSize;
}


string FileCreator::GetFileName(void)
{
	return this->fileName;
}

// need to test this
void FileCreator::SetFileName(const char* filename)
{
	this->fileName = string(filename);
}

// need to test
void FileCreator::SetFileType(const char* fileType)
{
	this->fileType = string(fileType);
}

void FileCreator::SetFilePtr()
{
	if (this->fileName != "")
	{
		
		// opens file as text
		// filename needed to be converted from string to c-style string to open
		if (this->fileType == "-t")
		{
			this->fp = fopen(GetFileName().c_str(), "w");
		}
		else
		{
			this->fp = fopen(GetFileName().c_str(), "wb");
		}

		if (this->fp == NULL)
		{
			cout << "File open error"; // error occured
		}
		
		
	}
}

void FileCreator::SetReceivedHash(char* hash)
{
	this->recievedHash = hash;
}


// break down packet into elements
// 
int FileCreator::ParsePacketData(char* packetData)
{
	return 0;
}

// write contents to file that is opened
int FileCreator::AppendToFile(char* packetData)
{
	return 0;
}

// compares the hashes
// received vs generated from final file
// 0 if correct and -1 if not matching
int FileCreator::VerifyHash()
{
	return 0;
}
