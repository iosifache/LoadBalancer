#ifndef _INTERFACE_PACKET_
#define INTERFACE_PACKET_

class iPacket{

	public:

		// Get client ip addr
		virtual char *getSourceIp() = 0;

		// Get client port sourse
		virtual unsigned getSourcePort() = 0;

		// Get client port sourse
		virtual bool decapsulation() = 0;

		virtual unsigned getMethod() = 0;

};

#endif