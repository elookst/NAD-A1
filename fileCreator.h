// NAME				:	FileCreator.h
// PROJECT			:	Network Application Development A1
// PROGRAMMER		:	Erica Luksts
// FIRST-VERSION	:	February 1, 2022



#ifndef _FILE_CREATOR_H
#define _FILE_CREATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <chrono>

#define FILE_TYPE_INDEX 1
#define FILE_SIZE_BYTE_MAX 8
#define FILE_SIZE_INDEX 2
#define HASH_INDEX 10
#define HASH_LENGTH 16
#define MAX_PACKET_NUM_INDEX 26
#define MAX_PACKET_BYTE_MAX 8
#define FILENAME_INDEX 34
#define MAX_PACKET_NUM_DATA_INDEX 9

using namespace std;


#pragma warning(disable:4996)

// CLASS	:	FileCreator
// PURPOSE	:	This class is used to create and write to a new file that is received from many packets.
//				Attributes include filename, filetype (binary or text), file size, and data variables for writing to files.
//				Methods included for file writing and reading from packets, verifying files, and calculating transfer time.
class FileCreator
{

private:

	string fileName; // file to write packet contents to
	string fileType; // text or binary file
	int fileSize; // file size
	string recievedHash; // MD5 hash received from the metadata packet, used to compare
	int currentPacketNumber; // to track packets received
	int maxPacketNumber; // ensures all packets received


public:

	char* binaryData; // all data received that was in binary file
	string createdFileHash;
	string textData; // all data received that was in a text file


	// default constructor
	// leaves everything blank
	FileCreator(string fileName, string fileType);

	string GetFileName(void);

	void SetFileSize(int filesize);

	int GetFileSize(void);

	void SetCurrentPacketNumber(int packetNumber);

	int GetCurrentPacketNumber(void);

	void SetMaxPacketNumber(int packetNumber);

	int GetMaxPacketNumber(void);

	void SetFileName(string filename);

	void SetFileType(const char* fileType);

	void SetReceivedHash(string hash);

	void SetBinaryData(char* binaryData);

	string GetBinaryData(void);

	string GetTextData(void);

	void SetCreatedFileHash(void);

	// get file data from the data packets
	// track the packet number achieved or look for EOF indicator
	int ParseMetadataPacket(unsigned char* packetData);

	// write to file opened
	// updates current packet number until max packet number reached
	int AppendToFile(unsigned char* packetData, int lastPacketDataLength);


	void ReadCreatedFileContents(void);

	int VerifyHash(void);

	void DisplayTransferTime(std::chrono::milliseconds time);

	static void TestErrorDetection(void);

};




#endif