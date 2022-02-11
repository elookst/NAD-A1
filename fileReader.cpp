// NAME				:	FileCreator.cpp
// PROJECT			:	Network Application Development A1
// PROGRAMMER		:   Travis Fiander
// FIRST-VERSION	:	January 28th, 2022


#include <iostream>
#include <fstream>
#include <string>
#include "fileReader.h"
#include "md5.h"

using namespace std;

#pragma warning(disable:4996)

// Function	   :	FileReader (constructor)
// Description :	Instantiates a FileReader class, which automatically performs all of the necesary 
//					file analysis
FileReader::FileReader(string fileName, string fileType)
{
	FileName = fileName;
	FileType = fileType;

	// call the read method upon instantiation. This will set data needed for further processing
	Read();

	// determine size of the file and set it
	SetFileSize();

	// get the md5 hash of the file and set it
	SetMD5hash();

	// split the file into packets and construct a packet list, ordered by packet number
	// this includes metadata construction
	SplitFileIntoPackets();

}


// creates metadata or content packets
Packet FileReader::CreatePacket(char type, int packetNum, char data[], int maxPacketNum)
{
	Packet newPacket;

	newPacket.packetType = type;
	newPacket.packetNumber = packetNum;
	newPacket.maxPacketNumber = maxPacketNum;
	strcpy(newPacket.data, data);

	return newPacket;
}

void FileReader::Read()
{
	if (FileType == "-t")
	{
		// read file contents char by char and store it in AllFileData
		ifstream file(FileName);
		char ch;
		if (file.is_open())
		{
			while (file.get(ch))
			{
				AllTextFileData.push_back(ch);

			}
			file.close();
		}
		else
		{
			cout << "Unable to open the file provided.";
		}
	}
	else if (FileType == "-b")
	{
		streampos size;

		ifstream file(FileName, ios::binary);
		if (file.is_open())
		{
			file.unsetf(ios::skipws);
			file.seekg(0, ios::end);
			size = file.tellg();
			file.seekg(0, ios::beg);

			AllBinaryData.reserve(size);

			AllBinaryData.insert(AllBinaryData.begin(),
				std::istream_iterator<char>(file),
				std::istream_iterator<char>());

			file.close();
		}
		else cout << "Unable to open file";
	}
}

void FileReader::SetFileSize()
{
	if (FileType == "-t")
	{
		fileSize = AllTextFileData.length();
	}
	else if (FileType == "-b")
	{
		fileSize = AllBinaryData.size();
	}
}

void FileReader::SetMD5hash()
{
	if (FileType == "-t")
	{
		MD5hash = md5(AllTextFileData);
	}
	else if (FileType == "-b")
	{
		string binaryString(AllBinaryData.begin(), AllBinaryData.end());
		MD5hash = md5(binaryString);
	}
}

void FileReader::SplitFileIntoPackets()
{
	if (FileType == "-t")
	{

		// figure out how many packets will be needed based on the size of the data in the file
		// divide the length of the data by the DATA_BUFFER
		int textDataLength = AllTextFileData.length();
		int totalPacketsRequired = (textDataLength / DATA_BUFFER) + 1; // +1 becuase it doesn't divide evenly

		if (packetCounter == 0)
		{
			// construct the metadata packet. This will be handled separately since it's of a differnet structure than 
			// the Packet Struct

			CreateMetadataPacket(totalPacketsRequired, 't');

			packetCounter++;
		}

		// iterate over the file data, incrementing i by DATA_BUFFER each time
		for (int i = 0; i < textDataLength; i += DATA_BUFFER)
		{
			// grab the substring from i to i + DATA_BUFFER and store in a substring
			// this will be the data for the individual packet
			string substring = AllTextFileData.substr(i, DATA_BUFFER);

			// convert string to char array
			char data[DATA_BUFFER + NULL_CHAR] = { 0 }; // +1 to account for the null terminator from c_str()
			strcpy(data, substring.c_str());

			// create the packet and add to the packet list
			Packet newPacket = CreatePacket(PACKET_TYPE_DATA, packetCounter, data, totalPacketsRequired);
			packetList.push_back(newPacket);

			packetCounter++;

			if (packetCounter > totalPacketsRequired)
			{
				break;
			}
		}
	}
	else if (FileType == "-b")
	{
		int binaryDataLength = AllBinaryData.size();
		int totalPacketsRequired = (AllBinaryData.size() / DATA_BUFFER) + 1; // + 1 because it doesn't divide evenly
		int counter = 0; // used to iterate over the binary data

		if (packetCounter == 0)
		{
			// construct the metadata packet. This will be handled separately since it's of a differnet structure than 
			// the Packet Struct

			CreateMetadataPacket(totalPacketsRequired, 'b');

			packetCounter++;
		}

		for (int i = 0; i < totalPacketsRequired; i++)
		{
			char tmp[DATA_BUFFER + NULL_CHAR] = { 0 };

			for (int j = 0; j < DATA_BUFFER; j++)
			{
				if (counter < binaryDataLength)
				{
					char tmp2[2] = { AllBinaryData.at(counter) };
					strcat(tmp, tmp2);
					counter++;
				}
				else
				{
					// before we break out of the constuction of the last packet, let's grab the length of
					// the actual data in the last packet to account for unused buffer space. We dont' want
					// to write this excess space to the new file

					lastPacketDataLength = j + 1;

					// this prevents the loop from accessing elements beyond the length of the 
					// vector in the last packet
					break;
				}
			}

			Packet newPacket = CreatePacket(PACKET_TYPE_DATA, packetCounter, tmp, totalPacketsRequired);
			packetList.push_back(newPacket);

			packetCounter++;
		}
	}
}

void FileReader::CreateMetadataPacket(int maxPacketNum, char dataType) {

	MetaDataPacket mdp;

	strncpy(mdp.fileName, FileName.c_str(), METADATA_BUFFER);
	strncpy(mdp.md5hash, MD5hash.c_str(), 16);
	mdp.fileSize = fileSize;
	mdp.dataType = dataType;
	mdp.maxPacketNumber = maxPacketNum;
	mdp.packetType = 'M';

	metadataPacket = mdp;
}
