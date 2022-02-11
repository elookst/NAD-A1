// NAME				:	FileCreator.cpp
// PROJECT			:	Network Application Development A1
// PROGRAMMER		:	Erica Luksts
// FIRST-VERSION	:	February 1, 2022
// DESCRIPTION		:	This file contains all of the method definitions for the FileCreator class.


#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "fileReader.h"
#include "fileCreator.h"
#include "md5.h"

using namespace std;


#pragma warning(disable:4996)

// METHOD		:	FileCreator - ctor
// PURPOSE		:	Instantiates a new FileCreator instance.
// PARAMETERS	:	NONE
// RETURNS		:	FileCreator object
FileCreator::FileCreator()
{
	this->fileName = "";
	this->fileType = "-t";
	this->recievedHash = "";
	this->fileSize = 0;
	this->currentPacketNumber = 0;
	this->maxPacketNumber = 1;
	this->textData = "";
}

// METHOD		:	SetFileSize
// PURPOSE		:	Sets file size
// PARAMETERS	:	int size
// RETURNS		:	NONE
void FileCreator::SetFileSize(int size)
{
	this->fileSize = size;
}

// METHOD		:	GetFileSize
// PURPOSE		:	Gets file size
// PARAMETERS	:	NONE
// RETURNS		:	int - file size
int FileCreator::GetFileSize(void)
{
	return this->fileSize;
}

// METHOD		:	SetCurrentPacketNumber
// PURPOSE		:	Sets current packet number
// PARAMETERS	:	int packetNumber
// RETURNS		:	NONE
void FileCreator::SetCurrentPacketNumber(int packetNumber)
{
	this->currentPacketNumber = packetNumber;
}

// METHOD		:	GetCurrentPacketNumber
// PURPOSE		:	Gets current packet number
// PARAMETERS	:	NONE
// RETURNS		:	int - current packet number received
int FileCreator::GetCurrentPacketNumber(void)
{
	return this->currentPacketNumber;
}

// METHOD		:	SetMaxPacketNumber
// PURPOSE		:	Sets max packet number
// PARAMETERS	:	int packetNumber
// RETURNS		:	NONE
void FileCreator::SetMaxPacketNumber(int packetNumber)
{
	this->maxPacketNumber = packetNumber;
}

// METHOD		:	GetMaxPacketNumber
// PURPOSE		:	Gets max packet number
// PARAMETERS	:	NONE
// RETURNS		:	int - max packet number received
int FileCreator::GetMaxPacketNumber(void)
{
	return this->maxPacketNumber;
}


// METHOD		:	GetFileName
// PURPOSE		:	Gets file name
// PARAMETERS	:	NONE
// RETURNS		:	string - filename
string FileCreator::GetFileName(void)
{
	return this->fileName;
}


// METHOD		:	SetFileName
// PURPOSE		:	Sets file name
// PARAMETERS	:	string - filename
// RETURNS		:	NONE
void FileCreator::SetFileName(string filename)
{
	this->fileName = filename;
}


// METHOD		:	SetFileType
// PURPOSE		:	Sets file type
// PARAMETERS	:	const char* - filetype
// RETURNS		:	NONE
void FileCreator::SetFileType(const char* fileType)
{
	this->fileType = string(fileType);
}


// METHOD		:	SetBinaryData
// PURPOSE		:	sets binary data
// PARAMETERS	:	char* binaryData
// RETURNS		:	NONE
void FileCreator::SetBinaryData(char* binaryData)
{
	this->binaryData = binaryData;
}

// METHOD		:	GetBinaryData
// PURPOSE		:	Gets binary data
// PARAMETERS	:	NONE
// RETURNS		:	char* binaryData
string FileCreator::GetBinaryData(void)
{
	return this->binaryData;
}


// METHOD		:	GetTextData
// PURPOSE		:	Gets text data
// PARAMETERS	:	string - text data
// RETURNS		:	NONE
string FileCreator::GetTextData(void)
{
	return this->textData;
}


// METHOD		:	SetReceivedHash
// PURPOSE		:	Sets received hash
// PARAMETERS	:	string - hash
// RETURNS		:	NONE
void FileCreator::SetReceivedHash(string hash)
{
	this->recievedHash = hash;
}


// METHOD		:	ParseMetadataPacket
// PURPOSE		:	Gets the data from the metadata packet received
// PARAMETERS	:	unsigned char* - packetData received
// RETURNS		:	NONE
void FileCreator::ParseMetadataPacket(unsigned char* packetData)
{
	
	// convert to string format for ease
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
	// 16 characters
	SetReceivedHash(packetStr.substr(HASH_INDEX, HASH_LENGTH));

	// set max packet number
	SetMaxPacketNumber(stoi(packetStr.substr(MAX_PACKET_NUM_INDEX, MAX_PACKET_BYTE_MAX)));

	// set filename
	// removes the filler characters after filename received
	string fileNameWithTrail = packetStr.substr(FILENAME_INDEX, METADATA_BUFFER);
	fileNameWithTrail.erase(remove(fileNameWithTrail.begin(), fileNameWithTrail.end(), '-'), fileNameWithTrail.end());
	SetFileName(fileNameWithTrail);
	
	
}



// METHOD		:	AppendToFile
// PURPOSE		:	Gets the content from data packet and writes to the file
// PARAMETERS	:	unsigned char* - packetData received
// RETURNS		:	1 if it was the last packet, 0 if more packets arriving
int FileCreator::AppendToFile(unsigned char* packetData)
{
	
	const int MAX_PACKET_SIZE = 256;
	// convert to a C++ string for convenience
	size_t packetLength = (size_t) MAX_PACKET_SIZE;
	string packetStr = string(reinterpret_cast<char const*>(packetData), packetLength);

	// set the current packet number
	string packetNumber = packetStr.substr(0, MAX_PACKET_BYTE_MAX);
	packetNumber.erase(remove(packetNumber.begin(), packetNumber.end(), '-'), packetNumber.end());
	SetCurrentPacketNumber(stoi(packetNumber));
	

	// has not reached the last packet
	if (this->currentPacketNumber != this->maxPacketNumber)
	{
		// remove protocol header characters
		string dataToWrite = packetStr.erase(0, 17);
		size_t dataSizeToWrite = DATA_BUFFER;

		// text file
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
				cout << "Error writing to transfered text file.\n";
			}

		}
		// binary file
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
				cout << "Error writing to transfered binary file.\n";
			}
			
		}
			
		return 0;

	}
	// this is the last packet
	// return 1 to indicate to outer loop to exit
	else
	{
		string dataToWrite = packetStr.erase(0, 17);
		size_t dataSizeToWrite = MAX_PACKET_SIZE - 17;

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

// METHOD		:	ReadCreatedFileContents
// PURPOSE		:	Gets content from the copied file for use in generating hash
// PARAMETERS	:	NONE
// RETURNS		:	NONE
void FileCreator::ReadCreatedFileContents(void)
{
	// text file
	if (this->fileType == "-t")
	{
		streampos size;
		char* textRead;
		ifstream file(GetFileName().c_str(), ios::ate);
		
		if (file.is_open())
		{
			// get file size before reading
			size = file.tellg();
			textRead = new char[size];

			// reset position to beginning
			file.seekg(0, ios::beg);
			file.read(textRead, size);

			// set text data member
			this->textData = string(textRead);
			file.close();

			delete[] textRead;
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

			cout << "File size expected: " << GetFileSize() << "\n";
			cout << "Bytes received into new file" << size << "\n";
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


	
}


// METHOD		:	SetCreatedFileHash
// PURPOSE		:	Generates 16-bit hash from copied file contents
// PARAMETERS	:	NONE
// RETURNS		:	NONE
void FileCreator::SetCreatedFileHash(void)
{
	if (this->fileType == "-t")
	{
		this->createdFileHash = md5(GetTextData()).substr(0, 16);
		
	}
	else
	{
		this->createdFileHash = md5(GetBinaryData()).substr(0,16);
	}
}

// METHOD		:	VerifyHash
// PURPOSE		:	Compares received hash to generated hash
// PARAMETERS	:	NONE
// RETURNS		:	NONE
int FileCreator::VerifyHash(void)
{

	if (this->recievedHash == this->createdFileHash)
	{
		cout << "File transfered successfully.\n";
		return 0;
	}
	else
	{
		cout << "File did not transfer successfully - unverified hashes\n";
		cout << "Received Hash:" << this->recievedHash << "\n";
		cout << "Created Hash:" << this->createdFileHash << "\n";
		return -1;
	}

}

// METHOD		:	DisplayTransferTime
// PURPOSE		:	Computes the transfer speed of the file send in MBps
// PARAMETERS	:	std::chrono::milliseconds - duration of transfer in ms
// RETURNS		:	NONE
void FileCreator::DisplayTransferTime(std::chrono::milliseconds duration)
{
	
	float transferRate = (float)GetFileSize() / duration.count();
	
	cout << "Calculated Transfer Time: " << (transferRate / 1000) << " MBps\n";


}



// METHOD		:	TestErrorDetection
// PURPOSE		:	Checks that file comparison by hashing works, generates hashes for 2 identical strings and compares them.
// PARAMETERS	:	NONE
// RETURNS		:	NONE
void FileCreator::TestErrorDetection(void)
{
	string txt1Data = "This is a test string";
	string txt2Data = "This is a test string";

	string txt1Hash;
	string txt2Hash;

	
	// create their hashes in 16 bit section only
	string txt1HashPart = md5(txt1Data);
	txt1Hash = txt1HashPart.substr(0, 16);
	string txt2HashPart = md5(txt2Data);
	txt2Hash = txt2HashPart.substr(0, 16);


	// check if hashes match
	if (txt1Hash == txt2Hash)
	{
		cout << "Test of Verification System Passed\n";
	}
	else
	{
		
		cout << "Verification Test Failed\n";
		cout << "Test Hash 1:" << txt1Hash << "\n";
		cout << "Test Hash 2:" << txt2Hash << "\n";

	}


}


