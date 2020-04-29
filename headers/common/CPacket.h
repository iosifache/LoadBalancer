#ifndef _CPACKET_H_

#define _CPACKET_H_

#include <iostream>
#include "IPacket.h"

class CPacket : public iPacket{

	protected:
		std::string _sourceIP;
		unsigned int _sourcePort;
		unsigned _method;
		std::string _payload;

	public:
		CPacket();
		CPacket(char *message);
		CPacket(const char *srcIP, unsigned srcProt);
		~CPacket();

		char *getSourceIp();
		unsigned getSourcePort();
		bool decapsulation();
		unsigned getMethod();

};

#endif