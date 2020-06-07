#ifndef _CLOAD_BALANCER_H_

#define _CLOAD_BALANCER_H_

#include"ConnectionDetails.h"

#include "CServer.h"
#include "CPacket.h"

#include <iostream>
#include <queue>
#include <vector>
#include<chrono>


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

/*
 * Structure need for socket connections
 */
typedef struct {
	int sockfd, portno;
	char buffer[256];
	struct hostent *server;
	struct sockaddr_in serv_addr;
}_networkData;


typedef struct {
    int number_of_managed_servers;
    int total_load;
    std::vector<int> requests_per_minute;
} LoadBalancerInfos;

typedef struct {
    int number_of_cores;
    int number_of_occupied_cores;
    int cores_load;
} ServerInfos;

/*
 * Main Class references to LoadBalancer~LoadB
 * This is a Singleton Class
 */

class CLoadBalancer {
	
	protected:
		//Packets received by LoadB
		static unsigned noPacket;
		//Number of server 
		static unsigned noStations;
		//Maximum packets saved to be processed in queue
		static unsigned QUEUE_SIZE;
		//Pointers saving server informations
		static std::vector<iStation *>_servers;
		//Pointer describing state of servers
		static unsigned *_serversStatus;
		//Pointer contains structure for socket connection
		static _networkData *serverData;
		//Queue for saving packets that can not be processed 
		static std::vector<iPacket*> _packetQueue;
		//Queue for respons after processing packet
		static std::vector<iPacket*> _responseQueue;
		
		//Last availeble server
		unsigned _lastServer; 
		//Pointers saving informations for SCITER
		static  LoadBalancerInfos _Infos;
		static ServerInfos *_serverDetails;

	public:


		//Functions SCITER's interface
		static LoadBalancerInfos GetLoadBalancerInfos(); //GET "_Infos"
		static ServerInfos GetServerInfo(int server_id);//GET "_serverDetails"

		//Creating Singleton member
		static CLoadBalancer *getInstance(int el, const char*IpAddr);
		//Reset Singleton member
		static void removeInstance();

		//Start enviroment for process packets
		static void running();
		//Adding a number of packet to be processed
		static void addPackets(int val);
		
			
	 
	private:
/*
 * SINGLETON SECTIONS
 */	
		//Singleton instanse 
		static CLoadBalancer *_pInstance;
	
		CLoadBalancer()=delete;
		CLoadBalancer(CLoadBalancer &obj)=delete;
		//C-tor of class
		CLoadBalancer(int PortNo,const char*IpAddr);
		static bool initStations(); 
		//D-tor of class 
		~CLoadBalancer();
		static int  clearStations();
/*
 * UTILITY SECTIONS
 */	
		//Get station handler to process packer
		static int findAvailableStation();

		//If queue full -> Received packets will be processed 
		static int waitForPacket();
		
		//Updating server status after connections
		static void setCapcity(int serverNo, char *data);

		//Sort request per minutes
		static void fit_10_request(int el);
/*
 * THREAD SECTIONS
 */		
		//Process packets thread
		static pthread_t thProcess;
		static void * thread_processFn(void *pck);
		//Send respons thread
		static pthread_t thRespons;
		static void * thread_responsFn(void *pck);
		//Counter time thread
		static pthread_t thTime;
		static void * thread_timeFn(void *pck);
		//Initial clock
		static std::chrono::steady_clock::time_point begin;
		//Counter of requests/ minutes
		static int _request_in_a_minute;
/*
 * SOCKET SECTIONS
 */			
		static int resetConnection();
		
		//Set up connection information to servers
		static void initNetworkData(int i, int portno, const char *serverIP);
		//Connecting to server[i]
		static void ConnectToServer(int i, const char* packType);
		//Sending packet to servers
		static void sendPacketNetwork(iPacket *pck);
/*
 * STATIC SECTIONS
 * 					this methods are developed for testing LoadB 
 * 					funtionality on the same machine
 */			
		//Process packet static on LoadB
		static void processPacket();
		static void sendPacket( iPacket * pck); 


		
		

};

#endif
