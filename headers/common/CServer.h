#ifndef _CSERVER_

#define _CSERVER_

#include"ConnectionDetails.h"

#include <iostream>
#include <vector>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "IStation.h"

typedef struct{
	int state;
	int remainingTime;
} process;

typedef struct{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
} _serverData;

class CServer : virtual public iStation{

	protected:
		int *_process;
		std::vector<process> _myProcess;
		unsigned _noProcess;
		unsigned _lastProcess;
		int _loaded;
		_serverData _serverCon;
		pthread_t thProcess;
		std::thread th;

	public:
		CServer(unsigned noProcess = 1);
		CServer(int portno, unsigned noProcess = 1);
		~CServer();
		iPacket *processPacket(iPacket &obj) override;
		bool doneProcessing() override;
		bool isAvailable() override;
		float getSolicitation() override;
		void do_work() override;
		void listenForConnection() override;
		void setCapacity(int nrProcess, int val, int load);
		void setLoaded(int val) { _loaded = val; }

	private:
		void initConnection(int portno);
		void setProcessBusy(int no, int time);
		int findAvailableProcess();
		int selectTask(int task);
		bool allProcessReady();
		static void *thread_processFn(void *);

};

#endif
