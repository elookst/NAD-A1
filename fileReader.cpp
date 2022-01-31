


#include <iostream>
#include <fstream>
#include <string>
#include "fileReader.h"

using namespace std;




// constructor
// file type is either binary or ASCII
FileReader::FileReader(string fileName, string fileType)
{

}





// creates metadata or content packets
Packet FileReader::CreatePackets()
{
	
	Packet newPacket;

	newPacket.packetType = 'M';
	newPacket.fileNamePacket = this->fileName;
	
	return newPacket;
}
