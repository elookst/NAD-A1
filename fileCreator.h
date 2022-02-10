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
#include <chrono>

#define FILE_TYPE_INDEX 2
#define FILE_SIZE_BYTE_MAX 8
#define FILE_SIZE_INDEX 3
#define HASH_INDEX 11
#define HASH_LENGTH 16

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
	ofstream fp; // will confirm if needed, will use to write to file
	string recievedHash; // MD5 hash received from the metadata packet, used to compare
	int currentPacketNumber; // to track packets received
	int maxPacketNumber; // ensures all packets received
	string binaryData; // all data received that was in binary file
	string textData; // all data received that was in a text file
	string createdFileHash;


public:

	// default constructor
	// leaves everything blank
	FileCreator();

	string GetFileName(void);

	void SetFileSize(int filesize);

	int GetFileSize(void);

	void SetCurrentPacketNumber(int packetNumber);

	int GetCurrentPacketNumber(void);

	void SetMaxPacketNumber(int packetNumber);

	int GetMaxPacketNumber(void);

	void SetFileName(string filename);

	void SetFileType(const char* fileType);

	void SetFilePtr();

	void SetReceivedHash(string hash);

	void SetBinaryData(char* binaryData);

	string GetBinaryData(void);

	void SetTextData(char* textData);

	string GetTextData(void);

	void SetCreatedFileHash(void);

	// get file data from the data packets
	// track the packet number achieved or look for EOF indicator
	int ParseMetadataPacket(unsigned char* packetData);
	
	// write to file opened
	// updates current packet number until max packet number reached
	int AppendToFile(unsigned char* packetData);

	
	int Close(void);

	int ReadCreatedFileContents(void);

	int VerifyHash(void);

	void DisplayTransferTime(std::chrono::seconds time);

};




#endif