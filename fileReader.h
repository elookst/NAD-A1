#ifndef _FILEREADER_H
#define _FILEREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

using namespace std;

const int DATA_BUFFER = 246; // max number of bytes without protocol headers that can be stored into a data packet   // 247
const int METADATA_BUFFER = 231;
const char PACKET_TYPE_DATA = 'D';
const char PACKET_TYPE_METADATA = 'M';
const int NULL_CHAR = 1;

// struct for packet creation?
// then can use the contents to assemble the char[] array according to protocol
struct Packet
{

	int packetNumber; // starts at 0, max value is maxPacketNumber, helps to remember order of packets
	char packetType; // M for metadata and D for content
	int maxPacketNumber; // ending packet number of the entire file read
	char data[DATA_BUFFER + NULL_CHAR];

};

struct MetaDataPacket
{
	char packetType;
	int fileSize;
	char md5hash[16];
	int maxPacketNumber;
	char fileName[METADATA_BUFFER];
};


// this class is used to create packets for sending to the receiving client

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

	FileReader(string fileName, string fileType); // constructor
	Packet CreatePacket(char type, int packetNum, char data[], int maxPacketNum);
	void Read(void);
	void SetFileSize(void);
	void SetMD5hash(void);
	void SplitFileIntoPackets(void);

};
#endif