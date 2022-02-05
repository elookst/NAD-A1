/*
	Reliability and Flow Control Example
	From "Networking for Game Programmers" - http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/


/*
	TO DO:
	 - cmd args
	 - get file name from cmd args
	 - transfer binary data
	 - construct and send metadata packet
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Net.h"
#include "fileReader.h"

//#define SHOW_ACKS

// prototypes
int checkArgs(int, char* []);

using namespace std;
using namespace net;

const int ServerPort = 30000;
const int ClientPort = 30001;
const int ProtocolId = 0x11223344;
const float DeltaTime = 1.0f / 30.0f;
const float SendRate = 1.0f / 30.0f;
const float TimeOut = 10.0f;
const int PacketSize = 256;

const char packetNumIndicator[5] = "<PN>";
const char maxPacketNumIndicator[5] = "<MP>";
const char packetTypeIndicator[5] = "<PT>";
const char dataIndicator[5] = "<DT>";

class FlowControl
{
public:

	FlowControl()
	{
		printf("flow control initialized\n");
		Reset();
	}

	void Reset()
	{
		mode = Bad;
		penalty_time = 4.0f;
		good_conditions_time = 0.0f;
		penalty_reduction_accumulator = 0.0f;
	}

	void Update(float deltaTime, float rtt)
	{
		const float RTT_Threshold = 250.0f;

		if (mode == Good)
		{
			if (rtt > RTT_Threshold)
			{
				printf("*** dropping to bad mode ***\n");
				mode = Bad;
				if (good_conditions_time < 10.0f && penalty_time < 60.0f)
				{
					penalty_time *= 2.0f;
					if (penalty_time > 60.0f)
						penalty_time = 60.0f;
					printf("penalty time increased to %.1f\n", penalty_time);
				}
				good_conditions_time = 0.0f;
				penalty_reduction_accumulator = 0.0f;
				return;
			}

			good_conditions_time += deltaTime;
			penalty_reduction_accumulator += deltaTime;

			if (penalty_reduction_accumulator > 10.0f && penalty_time > 1.0f)
			{
				penalty_time /= 2.0f;
				if (penalty_time < 1.0f)
					penalty_time = 1.0f;
				printf("penalty time reduced to %.1f\n", penalty_time);
				penalty_reduction_accumulator = 0.0f;
			}
		}

		if (mode == Bad)
		{
			if (rtt <= RTT_Threshold)
				good_conditions_time += deltaTime;
			else
				good_conditions_time = 0.0f;

			if (good_conditions_time > penalty_time)
			{
				printf("*** upgrading to good mode ***\n");
				good_conditions_time = 0.0f;
				penalty_reduction_accumulator = 0.0f;
				mode = Good;
				return;
			}
		}
	}

	float GetSendRate()
	{
		return mode == Good ? 30.0f : 10.0f;
	}

private:

	enum Mode
	{
		Good,
		Bad
	};

	Mode mode;
	float penalty_time;
	float good_conditions_time;
	float penalty_reduction_accumulator;
};

// ----------------------------------------------


// call to command line argument parsing function
// should parse Client/Server, ip address to send to, and the filename
// NOTE maybe switch for text or binary file?? (-t or -b)
// 0 if success, other values will be errors
int checkArgs(int numArgs, char* args[])
{

	int result = 0;
	// check that server was specified
	// default if starting program is to be a server if no other args
	if (numArgs == 1)
	{

	}
	// check that client with ip address and filename was specified
	// maybe default file type is binary?
	else if (numArgs == 4)
	{

	}
	// check that client, ip, filename, and filetype is in arguments
	else if (numArgs == 5)
	{

	}
	// invalid arguments given
	// display error and exit
	else
	{

		// display usage and error

		result = -1;
	}


	return result;

}

int main(int argc, char* argv[])
{
	FileReader fr = FileReader("C:/tmp/sample.txt", "-t");
	list<Packet> packetsToSend = fr.packetList;
	list<Packet>::iterator packetIter = packetsToSend.begin();


	int result = checkArgs(argc, argv);

	// make sure to validate fileSize so that the int values for packet numbers aren't more than 7 digits!

	// result value 0 is OK
	// any other value may mean invalid args and will exit


	enum Mode
	{
		Client,
		Server
	};


	// assign mode and address based on result from checkArgs only

	Mode mode = Server;
	Address address;


	// remove this, arguments will be parsed by checkArgs function
	if (argc >= 2)
	{
		int a, b, c, d;
#pragma warning(suppress : 4996)

		// this is checking for a valid ip address given
		if (sscanf(argv[1], "%d.%d.%d.%d", &a, &b, &c, &d))
		{
			mode = Client;
			address = Address(a, b, c, d, ServerPort);
		}
	}

	// initialize

	if (!InitializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}

	ReliableConnection connection(ProtocolId, TimeOut);

	const int port = mode == Server ? ServerPort : ClientPort;

	if (!connection.Start(port))
	{
		printf("could not start connection on port %d\n", port);
		return 1;
	}

	if (mode == Client)
		connection.Connect(address);
	else
		connection.Listen();

	bool connected = false;
	float sendAccumulator = 0.0f;
	float statsAccumulator = 0.0f;

	FlowControl flowControl;

	while (true)
	{
		// update flow control

		if (connection.IsConnected())
			flowControl.Update(DeltaTime, connection.GetReliabilitySystem().GetRoundTripTime() * 1000.0f);

		const float sendRate = flowControl.GetSendRate();

		// detect changes in connection state

		if (mode == Server && connected && !connection.IsConnected())
		{
			flowControl.Reset();
			printf("reset flow control\n");
			connected = false;
		}

		if (!connected && connection.IsConnected())
		{
			printf("client connected to server\n");
			connected = true;
		}

		if (!connected && connection.ConnectFailed())
		{
			printf("connection failed\n");
			break;
		}

		// at this point, client should be connected to server
		// this is when to retrieve the file data from disk
		// attempt to open and close file for reading
		// error check at each stage of open/read
		// utilize fread or fgets as necessary until all contents are read
		// client may also need to reference fileReader.cpp (may convert to .h file)


		// send and receive packets
		int counter = 0;
		sendAccumulator += DeltaTime;

		// client send loop
		while (sendAccumulator > 1.0f / sendRate)
		{

			unsigned char packet[PacketSize];
			memset(packet, 0, sizeof(packet));

			// packet construction (using an incremented pointer to fill it):
			unsigned char* ptr = packet;

			// check if the packer iterator is at the end. If so, we want to stop.
			if (packetIter == packetsToSend.end())
			{
				break;
			}

			// add packet number
			memcpy(ptr, packetNumIndicator, 4);
			ptr += 4;

			itoa(packetIter->packetNumber, (char*)ptr, 10);
			ptr += 8;

			// add packet type
			memcpy(ptr, packetTypeIndicator, 4);
			ptr += 4;

			*ptr = packetIter->packetType;
			ptr++;

			// add max packet number
			memcpy(ptr, maxPacketNumIndicator, 4);
			ptr += 4;

			itoa(packetIter->maxPacketNumber, (char*)ptr, 10);
			ptr += 8;

			// add the rest of the data
			memcpy(ptr, dataIndicator, 4);
			ptr += 4;

			int dataSize = strlen(packetIter->data);
			for (int i = 0; i < dataSize; i++)
			{
				*ptr = packetIter->data[i];
				ptr++;
			}


			// we;re done with this packet, so we increment the packet iterator to the next
			// packet in the packetList
			packetIter++;

			connection.SendPacket(packet, sizeof(packet));
			// iterate through the group of packets after ack

			sendAccumulator -= 1.0f / sendRate;
		}


		// server receiving info here
		while (true)
		{
			unsigned char packet[256];
			int bytes_read = connection.ReceivePacket(packet, sizeof(packet));

			// include function for parsing the packet
			// parsing should discern if the packet included file contents (D) or metadata (M)
			// metadata broken up into pieces by their header tags (<fs> for file size etc.)
			// string functions as necessary

			// generate hash at the end of file indicator


			if (bytes_read != 0)
			{

			}
			if (bytes_read == 0)
				break;
		}

		// at end of packets sent, call on fileValidation and fileReader to verify the hashes and write the file data to the disk

		// show packets that were acked this frame

#ifdef SHOW_ACKS
		unsigned int* acks = NULL;
		int ack_count = 0;
		connection.GetReliabilitySystem().GetAcks(&acks, ack_count);
		if (ack_count > 0)
		{
			printf("acks: %d", acks[0]);
			for (int i = 1; i < ack_count; ++i)
				printf(",%d", acks[i]);
			printf("\n");
		}
#endif

		// update connection

		connection.Update(DeltaTime);

		// show connection stats

		statsAccumulator += DeltaTime;

		while (statsAccumulator >= 0.25f && connection.IsConnected())
		{
			float rtt = connection.GetReliabilitySystem().GetRoundTripTime();

			unsigned int sent_packets = connection.GetReliabilitySystem().GetSentPackets();
			unsigned int acked_packets = connection.GetReliabilitySystem().GetAckedPackets();
			unsigned int lost_packets = connection.GetReliabilitySystem().GetLostPackets();

			float sent_bandwidth = connection.GetReliabilitySystem().GetSentBandwidth();
			float acked_bandwidth = connection.GetReliabilitySystem().GetAckedBandwidth();

			printf("rtt %.1fms, sent %d, acked %d, lost %d (%.1f%%), sent bandwidth = %.1fkbps, acked bandwidth = %.1fkbps\n",
				rtt * 1000.0f, sent_packets, acked_packets, lost_packets,
				sent_packets > 0.0f ? (float)lost_packets / (float)sent_packets * 100.0f : 0.0f,
				sent_bandwidth, acked_bandwidth);

			statsAccumulator -= 0.25f;
		}

		net::wait(DeltaTime);
	}

	ShutdownSockets();

	return 0;
}
