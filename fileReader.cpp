#include <iostream>
#include <fstream>
#include <string>
#include "fileReader.h"
#include "md5.h"

using namespace std;


// constructor
// file type is either binary or ASCII
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
		int totalPacketsRequired = AllBinaryData.size() / DATA_BUFFER;
		int counter = 0; // used to iterate over the binary data

		vector<char[DATA_BUFFER]> binaryDataChunks;

		for (int i = 0; i < totalPacketsRequired; i++)
		{
			char tmp[DATA_BUFFER];

			for (int j = 0; j < DATA_BUFFER; j++)
			{
				tmp[j] = AllBinaryData[counter];
				counter++;
			}

			binaryDataChunks.push_back(tmp);
		}

		vector<char[DATA_BUFFER]>::iterator iter = binaryDataChunks.begin();

		for (iter; iter != binaryDataChunks.end(); iter++)
		{
			Packet newPacket = CreatePacket(PACKET_TYPE_DATA, packetCounter, *iter, totalPacketsRequired);
			packetList.push_back(newPacket);

			packetCounter++;
		}
	}
}
