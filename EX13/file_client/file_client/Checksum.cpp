#include <cstring>
#include "Checksum.h"
#include "TransConst.h"

namespace Transport
{
	long Checksum::checksum(char buf[], short size)
	{
		int i = 0, length = size;
		long sum = 0;
		while (length > 0) 
		{
	   		sum	+= (buf[i++]&0xff) << 8;
   			if ((--length)==0) break;
   			sum += (buf[i++]&0xff);
	   		--length;
		}

		return (~((sum & 0xFFFF)+(sum >> 16)))&0xFFFF;
	}

	bool Checksum::checkChecksum(char buf[], short size)
	{
		char buffer[size];

		memcpy(buffer, buf, size); 
		calcChecksum(buffer,size);

		if(buffer[0]==buf[0] && buffer[1]==buf[1])
			return true;
		else
			return false;

	}

	void Checksum::calcChecksum (char buf[], short size)
	{
		char buffer[size];
		long sum = 0;

		memcpy(buffer, buf+ACKSIZE, size); 
		sum = checksum(buffer, size);
		buf[CHKSUMHIGH] = ((sum >> 8) & 255);
		buf[CHKSUMLOW] = (sum & 255);
	}
}

