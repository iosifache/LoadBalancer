#ifndef _INTERFACE_STATION_

#define _INTERFACE_STATION_

#include <iostream>

class iPacket;

class iStation{

	public:
		virtual iPacket *processPacket(iPacket &obj) = 0;
		virtual bool doneProcessing() = 0;
		virtual bool isAvailable() = 0;
		virtual float getSolicitation() = 0;
		virtual void do_work() = 0;
		virtual void listenForConnection() = 0;
		virtual ~iStation() { ; };
		virtual void setCapacity(int nrProcess, int val, int load) = 0;
		virtual void setLoaded(int val) = 0;

};

#endif