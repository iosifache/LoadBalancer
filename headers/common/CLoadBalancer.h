#ifndef _CLOAD_BALANCER_H_

#define _CLOAD_BALANCER_H_

#include "CServer.h"
#include "CPacket.h"
#include "ILoadBalancer.h"
#include <iostream>
#include <queue>
#include <vector>
#include<chrono>

//#define noStations 2
//#define QUEUE_SIZE 4
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define STOP_PROCESSING_PCK 6
#define ACK_PROCESSING_PCK 66

typedef struct {
	int sockfd, portno;
	char buffer[256];
	struct hostent *server;
	struct sockaddr_in serv_addr;
}_networkData;

class CLoadBalancer {
	
	protected:
		static unsigned noPacket;
		static unsigned noStations;
		static unsigned QUEUE_SIZE;

		static std::vector<iStation *>_servers;
		static unsigned *_serversStatus;
		static _networkData *serverData;

		static std::vector<iPacket*> _packetQueue;

		static std::vector<iPacket*> _responseQueue;
		
		/*Last worker*/
		unsigned _lastServer; 
		static  LoadBalancerInfos _Infos;
		static ServerInfos *_serverDetails;

	public:
		static LoadBalancerInfos GetLoadBalancerInfos();
		static ServerInfos GetServerInfo(int server_id);
		//static void setServerInfos(const LoadBalancerInfos&obj);
		static void running();
		static void addPackets(int val);
		static CLoadBalancer *getInstance(int el);
		static int resetConnection();
		static void sendRespons( iPacket & pck); 
		static void sendPacket( iPacket * pck); 
		static bool queueIsFull();
		static void getWorkCapacity();
		static void removeInstance();
		~CLoadBalancer();
		static void * thread_processFn(void *pck);
		static void * thread_responsFn(void *pck);
		static void * thread_timeFn(void *pck);
	 
	private:
		static void initNetworkData(int i, int portno, const char *serverIP);
		static void ConnectToServer(int i, char* packType);
		static void sendPacketNetwork(iPacket *pck);
		static void sendPacketNetworkA( iPacket* pck);
		static void processPacket();
		static int findAvailableStation();
		static bool initStations(); 
		static int  clearStations();
		static int waitForPacket();
		static CLoadBalancer *_pInstance;
		static pthread_t thProcess;
		static pthread_t thRespons;
		static pthread_t thTime;
		CLoadBalancer(int el);
		static void setCapcity(int serverNo, char *data);
		static std::chrono::steady_clock::time_point begin;
		static int _request_in_a_minute;

};

#endif