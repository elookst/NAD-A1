#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Net.h"


using namespace std;


// this class is used to create packets for sending to the receiving client

class FileReader
{

private:

	string fileType; //-t for text, -b for binary
	string fileName; // name of file to be sent/read
	const int DATA_BUFFER = 225; // max number of bytes without protocol headers that can be stored into packet

	// list for storing packets created? might not be necessary
	list<Packet> packetList;

	char packet[256]; // after protocol tags applied, packet content stored in here for sending

public:

	FileReader(string fileName, string fileType); // constructor

	Packet CreatePackets();

};


// struct for packet creation?
// then can use the contents to assemble the char[] array according to protocol
struct Packet
{
	
	int packetNumber; // starts at 0, max value is maxPacketNumber, helps to remember order of packets
	char packetType; // M for metadata and D for content
	string fileNamePacket; // filename
	int maxPacketNumber; // ending packet number of the entire file read

};
