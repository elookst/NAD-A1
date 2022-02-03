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
Packet FileReader::CreatePacket(char type, char data[])
{

	Packet newPacket;

	newPacket.packetType = type;
	newPacket.fileNamePacket = this->FileName;

	return newPacket;
}

void FileReader::Read()
{
	if (FileType == "-t")
	{
		// read file contents char by char and store it in AllFileData
		std::ifstream file(FileName);
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

		int totalPacketsRequired = AllTextFileData.length() / DATA_BUFFER;



	}
	else if (FileType == "-b")
	{

		int totalPacketsRequired = AllBinaryData.size() / DATA_BUFFER;


	}
}
