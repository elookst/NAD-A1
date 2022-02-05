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


// sets the fileName and fileType attributes for creating the file
FileCreator::FileCreator(const char metadataPacket[])
{
		
	this->fileType = "-t";
	this->recievedHash = "";
	this->fileName = "";
	// copy 16 bytes of metadata packet to receivedHash
	

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
