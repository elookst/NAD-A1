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
	this->fileSize = 0;
	this->currentPacketNumber = 0;
	this->maxPacketNumber = 1;
}


void FileCreator::SetFileSize(int size)
{
	this->fileSize = size;
}

int FileCreator::GetFileSize(void)
{
	return this->fileSize;
}

void FileCreator::SetCurrentPacketNumber(int packetNumber)
{
	this->currentPacketNumber = packetNumber;
}

int FileCreator::GetCurrentPacketNumber(void)
{
	return this->currentPacketNumber;
}

void FileCreator::SetMaxPacketNumber(int packetNumber)
{
	this->maxPacketNumber = packetNumber;
}

int FileCreator::GetMaxPacketNumber(void)
{
	return this->maxPacketNumber;
}


string FileCreator::GetFileName(void)
{
	return this->fileName;
}

// need to test this
void FileCreator::SetFileName(string filename)
{
	this->fileName = filename;
}

// need to test
void FileCreator::SetFileType(const char* fileType)
{
	this->fileType = string(fileType);
}

// opens the file for writing from pointer
void FileCreator::SetFilePtr()
{
	if (this->fileName != "")
	{
		
		// opens file as text
		// filename needed to be converted from string to c-style string to open
		// need to open for read and write to compare hashes later
		if (this->fileType == "-t")
		{
			this->fp.open(this->fileName, ios::in | ios::out );
		}
		else
		{
			this->fp.open(this->fileName, ios::in | ios::out | ios::binary);
		}

		if (!this->fp.is_open())
		{
			cout << "Unable to open file for writing";
		}
		
		
	}
}


void FileCreator::SetReceivedHash(string hash)
{
	this->recievedHash = hash;
}


// break down packet into elements
// 
int FileCreator::ParseMetadataPacket(unsigned char* packetData)
{
	
	// convert to a C++ string for convenience
	string packetStr = string((char*)packetData);


	// set which type of file to write
	if (packetData[FILE_TYPE_INDEX] == 't')
	{
		SetFileType("-t"); 
	}
	else
	{
		SetFileType("-b");
	}

	// set file size
	// get substring from the packet string
	string fileSize = packetStr.substr(FILE_SIZE_INDEX, FILE_SIZE_BYTE_MAX);
	SetFileSize(stoi(fileSize));

	
	// set hash
	SetReceivedHash(packetStr.substr(HASH_INDEX, HASH_LENGTH));

	// set file name
	SetFileName(packetStr.erase(0, HASH_INDEX + HASH_LENGTH));

	
	
	return 0;
}



// write contents to file that is opened
int FileCreator::AppendToFile(unsigned char* packetData)
{
	
	string packetStr = string((char*)packetData);

	// update current packet number
	SetCurrentPacketNumber(stoi(packetStr.substr(0, 1)));

	// set max packet number
	// set to 10 for now (needs updating to match data packet structure)
	SetMaxPacketNumber(stoi(packetStr.substr(3, 10)));

	// has not reached the last packet
	if (this->currentPacketNumber != this->maxPacketNumber)
	{
		
		string dataToWrite = packetStr.erase(0, 10);
		this->fp << (dataToWrite.c_str());
		return 0;

	}
	// this is the last packet
	// return 1 to indicate to outer loop to exit
	else
	{
		string dataToWrite = packetStr.erase(0, 10);
		this->fp << (dataToWrite.c_str());
		return 1;
	}



	
}

// compares the hashes
// received vs generated from final file
// 0 if correct and -1 if not matching
int FileCreator::VerifyHash()
{
	
	// generate hash from end file




	// compare both hashes
	
	return 0;
}




// closes fp
int FileCreator::Close()
{
	if (this->fp.is_open())
	{
		fp.close();
	}
}
