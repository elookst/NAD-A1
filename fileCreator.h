// NAME				:	FileCreator.h
// PROJECT			:	Network Application Development A1
// PROGRAMMER		:
// FIRST-VERSION	:


#ifndef _FILE_CREATOR_H
#define _FILE_CREATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>


using namespace std;

#pragma warning(disable:4996)

// CLASS	:	FileCreator
// PURPOSE	:	This class is used to create and write to a new file that is received from many packets.
//				Attributes include
//				Methods include
class FileCreator
{

private:

	string fileName; // file to write packet contents to
	string fileType; // text or binary file
	int fileSize; // file size
	FILE* fp; // will confirm if needed, will use to write to file
	string recievedHash; // MD5 hash received from the metadata packet, used to compare
	

public:

	// default constructor
	// leaves everything blank
	FileCreator();

	string GetFileName(void);

	void SetFileSize(int filesize);

	int GetFileSize(void);
	
	void SetFileName(const char* filename);

	void SetFileType(const char* fileType);

	void SetFilePtr();

	void SetReceivedHash(char* hash);
	

	// get file data from the data packets
	// track the packet number achieved or look for EOF indicator
	int ParsePacketData(char* packetData);
	
	// write to file opened
	int AppendToFile(char* packetData);


	int VerifyHash(void);

};




#endif