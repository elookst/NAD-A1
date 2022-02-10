// NAME				:	FileCreator.cpp
// PROJECT			:	Network Application Development A1
// PROGRAMMER		:
// FIRST-VERSION	:

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "fileReader.h"
#include "fileCreator.h"
#include "md5.h"

using namespace std;

#pragma warning(disable:4996)

// sets blank filename etc.
FileCreator::FileCreator()
{
	this->fileName = "C:\\tmp\\error2.jpg";
	//this->fileName = "C:\\tmp\\test2.txt";
	this->fileType = "-t";
	this->recievedHash = "";
	this->fileSize = 0;
	this->currentPacketNumber = 0;
	this->maxPacketNumber = 1;
	this->textData = "";
	this->binaryData = "";
}


void FileCreator::SetFileSize(int size)
{
	this->fileSize = size;
}

int FileCreator::GetFileSize(void)
{
	return this->fileSize;
}

void FileCreator::SetCurrentPacketNumber(int packetNumber)
{
	this->currentPacketNumber = packetNumber;
}

int FileCreator::GetCurrentPacketNumber(void)
{
	return this->currentPacketNumber;
}

void FileCreator::SetMaxPacketNumber(int packetNumber)
{
	this->maxPacketNumber = packetNumber;
}

int FileCreator::GetMaxPacketNumber(void)
{
	return this->maxPacketNumber;
}


string FileCreator::GetFileName(void)
{
	return this->fileName;
}

// *****************************************
// need to test this
// temporarily renames the filename so that it can be stored on tested system
void FileCreator::SetFileName(string filename)
{
	
	string copy = "C:\\tmp\\error2.jpg";
	//string copy = "C:\\tmp\\test2.txt";
	this->fileName = copy;
}

// need to test
void FileCreator::SetFileType(const char* fileType)
{
	this->fileType = string(fileType);
}

void FileCreator::SetBinaryData(char* binaryData)
{
	size_t size = sizeof(binaryData);
	string binaryStr = string(reinterpret_cast<char const*>(binaryData), size);
	this->binaryData = binaryStr;
}

string FileCreator::GetBinaryData(void)
{
	return this->binaryData;
}

void FileCreator::SetTextData(char* textData)
{
	size_t size = sizeof(textData);
	string textStr = string(reinterpret_cast<char const*>(textData), size);
	this->textData = textStr;
}

string FileCreator::GetTextData(void)
{
	return this->textData;
}



void FileCreator::SetReceivedHash(string hash)
{
	this->recievedHash = hash;
}


// break down packet into elements
// 
int FileCreator::ParseMetadataPacket(unsigned char* packetData)
{
	size_t packetLength = METADATA_BUFFER;
	string packetStr = string(reinterpret_cast<char const*>(packetData), packetLength);
	

	// set which type of file to write
	if (packetData[FILE_TYPE_INDEX] == 't')
	{
		SetFileType("-t"); 
	}
	else
	{
		SetFileType("-b");
	}

	// set file size
	// get substring from the packet string
	string fileSize = packetStr.substr(FILE_SIZE_INDEX, FILE_SIZE_BYTE_MAX);
	double fFileSize = atof(fileSize.c_str());

	SetFileSize((int)(fFileSize*1000));

	
	// set hash
	SetReceivedHash(packetStr.substr(HASH_INDEX, HASH_LENGTH));

	// set max packet number
	SetMaxPacketNumber(stoi(packetStr.substr(MAX_PACKET_NUM_INDEX, MAX_PACKET_BYTE_MAX)));

	// set filename
	// need to remove filler characters
	string fileNameWithTrail = packetStr.substr(FILENAME_INDEX, METADATA_BUFFER);
	SetFileName(fileNameWithTrail);
	
	return 0;
}



// write contents to file that is opened
// returns 1 if there are no more packets to receive
// otherwise returns 0
int FileCreator::AppendToFile(unsigned char* packetData)
{
	
	// convert to a C++ string for convenience
	
	size_t packetLength = 256;

	string packetStr = string(reinterpret_cast<char const*>(packetData), packetLength);

	string packetNumber = packetStr.substr(0, MAX_PACKET_BYTE_MAX);

	packetNumber.erase(remove(packetNumber.begin(), packetNumber.end(), '-'), packetNumber.end());

	// update current packet number
	SetCurrentPacketNumber(stoi(packetNumber));
	

	// has not reached the last packet
	if (this->currentPacketNumber != this->maxPacketNumber)
	{
		string dataToWrite = packetStr.erase(0, 17);

		size_t dataSizeToWrite = 256 - 17;

		if (this->fileType == "-t")
		{

			fstream fp(GetFileName().c_str(), ios::out | ios::app);

			if (fp.is_open())
			{
				fp << (dataToWrite.c_str());
				fp.close();
			}
			else
			{
				cout << "Error writing to file.\n";
			}

		}
		else
		{

			fstream fp(GetFileName().c_str(), ios::out | ios::app | ios::binary);

			if (fp.is_open())
			{

				fp.write(GetFileName().c_str(), dataSizeToWrite);
				fp.close();
			}
			else
			{
				cout << "Error writing to file.\n";
			}
			
		}
			

		return 0;

	}
	// this is the last packet
	// return 1 to indicate to outer loop to exit
	else
	{
		string dataToWrite = packetStr.erase(0, 17);

		size_t dataSizeToWrite = 256 - 17;

		if (this->fileType == "-t")
		{
			fstream fp(GetFileName().c_str(), ios::out | ios::app);

			if (fp.is_open())
			{
				fp << (dataToWrite.c_str());
				fp.close();
			}
			else
			{
				cout << "Error writing to file.\n";
			}
		}
		else
		{
			// convert dataToWrite into 
			fstream fp(GetFileName().c_str(), ios::out | ios::app | ios::binary);

			if (fp.is_open())
			{

				fp.write(dataToWrite.c_str(), dataSizeToWrite);
				fp.close();
			}
			else
			{
				cout << "Error writing to file.\n";
			}


			
		}

		return 1;
	}



	
}

// gets all the text or binary file data
// stores it in string member

int FileCreator::ReadCreatedFileContents()
{
	// text file
	if (this->fileType == "-t")
	{
		streampos size;
		char* textData;
		ifstream file(GetFileName().c_str(), ios::ate);
		
		if (file.is_open())
		{
			// get file size before reading
			size = file.tellg();
			textData = new char[size];

			// reset position to beginning
			file.seekg(0, ios::beg);
			file.read(textData, size);

			// set text data member
			SetTextData(textData);
			file.close();

			delete[] textData;
		}
		else
		{
			cout << "Error: could not read file created.";
		}


	}
	else if (this->fileType == "-b")
	{
		
		streampos size;
		char* binaryData;
		
		// open file, move position to end to get size requirement
		ifstream file(GetFileName().c_str(), ios::binary | ios::ate);
		if (file.is_open())
		{
			size = file.tellg();
			binaryData = new char[size];

			// reset position to beginning
			file.seekg(0, ios::beg);
			file.read(binaryData, size);
			SetBinaryData(binaryData);
			file.close();

			delete[] binaryData;
		}
		else
		{
			cout << "Error: could not read file created.";
		}

	}


	
	return 0;
}


// using text or binary file contents
// generates the hash
void FileCreator::SetCreatedFileHash(void)
{
	if (this->fileType == "-t")
	{
		this->createdFileHash = md5(GetTextData());
	}
	else
	{
		this->createdFileHash = md5(GetBinaryData());
	}
}

// compares received hash to created file hash
int FileCreator::VerifyHash(void)
{

	if (this->recievedHash == this->createdFileHash)
	{
		cout << "File transfered successfully.";
		return 0;
	}
	else
	{
		cout << "File did not transfer successfully - unverified hashes";
		return -1;
	}

}


void FileCreator::DisplayTransferTime(std::chrono::seconds duration)
{
	
	double transferRate = (double)this->fileSize / duration.count();
	
	cout << "Calculated Transfer Time: " << transferRate / 1000000 << "MBps";


}


