#include <iostream>
#include <cstring>
#include <cstdio>
#include "Transport.h"
#include "TransConst.h"

#define DEFAULT_SEQNO 2

namespace Transport
{
	/// <summary>
	/// Initializes a new instance of the <see cref="Transport"/> class.
	/// </summary>
	Transport::Transport (short BUFSIZE)
	{
		link = new Link(BUFSIZE+ACKSIZE);
		checksum = new Checksum();
		buffer = new char[BUFSIZE+ACKSIZE];
		seqNo = 0;
		old_seqNo = DEFAULT_SEQNO;
		errorCount = 0;
	}

	/// <summary>
	/// Delete dynamics attribute before delete this object.
	/// </summary>
	Transport::~Transport()
	{
		if(link) delete link;
		if(checksum) delete checksum;
		if(buffer) delete [] buffer;
	}
	
	/// <summary>
	/// Receives the ack.
	/// </summary>
	/// <returns>
	/// The ack.
	/// </returns>
	bool Transport::receiveAck()
	{
		char buf[ACKSIZE];
		short size = link->receive(buf, ACKSIZE);
		if (size != ACKSIZE) return false;
		if(!checksum->checkChecksum(buf, ACKSIZE) ||
				buf[SEQNO] != seqNo ||
				buf[TYPE] != ACK)
			return false;
			
		seqNo = (buffer[SEQNO] + 1) % 2;
			
		return true;
	}

	/// <summary>
	/// Sends the ack.
	/// </summary>
	/// <param name='ackType'>
	/// Ack type.
	/// </param>
	void Transport::sendAck (bool ackType)
	{
		char ackBuf[ACKSIZE];
		ackBuf [SEQNO] = (ackType ? buffer[SEQNO] : (buffer[SEQNO] + 1) % 2);
		ackBuf [TYPE] = ACK;
		checksum->calcChecksum (ackBuf, ACKSIZE);

		link->send(ackBuf, ACKSIZE);

	}

	/// <summary>
	/// Send the specified buffer and size.
	/// </summary>
	/// <param name='buffer'>
	/// Buffer.
	/// </param>
	/// <param name='size'>
	/// Size.
	/// </param>
	void Transport::send(char buf[], short size)
	{
		memcpy(this->buffer+ACKSIZE, buf, size);
		checksum->calcChecksum(buffer,size);
		buffer[SEQNO]=seqNo;
		buffer[TYPE]=DATA;//type = data
		    
		link->send(this->buffer,size+ACKSIZE);//header + data

		if(!receiveAck())
			std::cout<<"Error did not receive ACK in transportlayer\r\n";
			
		return;
	}

	/// <summary>
	/// Receive the specified buffer.
	/// </summary>
	/// <param name='buffer'>
	/// Buffer.
	/// </param>
	short Transport::receive(char buf[], short size)
	{
		short bytesRead;
		bytesRead = link->receive(this->buffer,size+ACKSIZE);

		if(!checksum->checkChecksum(this->buffer,size))
		{
			std::cout<<"Error in checksum\r\n";
			sendAck(false);
			return -1;
		}
		else
		{
			sendAck(true);
			memcpy(buf,buffer+ACKSIZE,bytesRead);
			return bytesRead;
		}
	}
}

