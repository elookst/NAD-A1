// NAME				:	FileCreator.h
// PROJECT			:	Network Application Development A1
// PROGRAMMER		:	Travis Fiander
// FIRST-VERSION	:	February 1, 2022

#ifndef _FILEREADER_H
#define _FILEREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

typedef unsigned char BYTE;

using namespace std;

#pragma warning(disable:4996)

const int DATA_BUFFER = 238; // max number of bytes without protocol headers that can be stored into a data packet + delimiters
const int METADATA_BUFFER = 220;
const char PACKET_TYPE_DATA = 'D';
const char PACKET_TYPE_METADATA = 'M';
const int NULL_CHAR = 1;

// Data packet structure
struct Packet
{

	int packetNumber; // 0  // starts at 0, max value is maxPacketNumber, helps to remember order of packets
	char packetType; // 8 // M for metadata and D for content
	int maxPacketNumber; // 9 // ending packet number of the entire file read
	char data[DATA_BUFFER]; // 17

};

// metadata packet structure
struct MetaDataPacket
{
	char packetType; // 0
	char dataType;   // 1
	int fileSize;    // 2
	char md5hash[16]; // 10
	int maxPacketNumber; // 26
	char fileName[METADATA_BUFFER]; // 34
};


// CLASS	:	FileReader
// PURPOSE	:	This class is used to perform all of the functionality related to reading the file to send, 
//				gathering file info, splitting into packets, and constructing a list of packets to send.
class FileReader
{

private:

	string FileType; //-t for text, -b for binary
	string FileName; // name of file to be sent/read
	int fileSize = 0;
	int packetCounter = 0;
	char packet[256]; // after protocol tags applied, packet content stored in here for sending

public:

	string AllTextFileData; // store the data from text the file here
	vector<char> AllBinaryData; // store all data from binary file here
	string MD5hash;
	list<Packet> packetList;
	MetaDataPacket metadataPacket;
	int lastPacketDataLength;

	FileReader(string fileName, string fileType); // constructor
	Packet CreatePacket(char type, int packetNum, char data[], int maxPacketNum);
	void Read(void);
	void SetFileSize(void);
	void SetMD5hash(void);
	void SplitFileIntoPackets(void);
	void CreateMetadataPacket(int maxPacketNum, char dataType);

};
#endif