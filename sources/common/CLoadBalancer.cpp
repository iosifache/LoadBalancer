#include "CLoadBalancer.h"

CLoadBalancer *CLoadBalancer::_pInstance = NULL;


unsigned *CLoadBalancer::_serversStatus = NULL;
std::vector<iStation *> CLoadBalancer::_servers;
std::vector<iPacket *> CLoadBalancer::_packetQueue;
std::vector<iPacket *> CLoadBalancer::_responseQueue;

LoadBalancerInfos CLoadBalancer::_Infos;
ServerInfos *CLoadBalancer::_serverDetails = NULL;

unsigned CLoadBalancer::noStations = 2;
unsigned CLoadBalancer::QUEUE_SIZE = 4;
unsigned CLoadBalancer::noPacket = 0;

int CLoadBalancer::_request_in_a_minute = 0;
std::chrono::steady_clock::time_point CLoadBalancer::begin;
_networkData *CLoadBalancer::serverData = NULL;

pthread_t CLoadBalancer::thProcess;
pthread_t CLoadBalancer::thRespons;
pthread_t CLoadBalancer::thTime;

static std::string IpAddress;
static int PortNOO = 0;


CLoadBalancer *CLoadBalancer::getInstance(int el, const char *ipAddr){
	if (_pInstance == NULL)
		_pInstance = new CLoadBalancer(el, ipAddr);

	return _pInstance;
}

void CLoadBalancer::removeInstance(){
	delete _pInstance;
}

int CLoadBalancer::resetConnection(){

	if (_pInstance != NULL)
	{
		delete _pInstance;
		_pInstance == NULL;
	}
	//_pInstance = new CLoadBalancer(9);
	if (_pInstance == NULL)
		return -1;
	return 1;
}



ServerInfos CLoadBalancer::GetServerInfo(int server_id){

	return _serverDetails[server_id];
}

LoadBalancerInfos CLoadBalancer::GetLoadBalancerInfos(){

	return _Infos;
}



void CLoadBalancer::running(){
	CPacket pck;

	if (noPacket > 0)
	{
		for (int i = 0; i < noPacket; i++)
		{
			int err = pthread_create(&thProcess, NULL, thread_processFn, &pck);
			//err = pthread_create(&thRespons, NULL, thread_responsFn, NULL);
			pthread_join(thProcess, NULL);
			err = pthread_create(&thTime, NULL, thread_timeFn, NULL);
			pthread_join(thTime, NULL);
			//pthread_join(thRespons, NULL);
			printf("[C++]LoadBalancer-->Processing... \n");
			sleep(2);
		}
	}
}

void CLoadBalancer::addPackets(int val){
	noPacket = val;
}



int CLoadBalancer::findAvailableStation(){

	for (int i = 0; i < noStations; i++)
	{

		if (_servers[i]->isAvailable())
		{
			return i;
		}
	}
	return -1;
}

void CLoadBalancer::setCapcity(int no, char *data){
	
	int j = 0;
	std::string nr = "";
	int load = 0;
	for (int i = 0; i < strlen(data); i++)
	{
		if (data[i] == ';')
		{

			int se = atoi(nr.c_str());
			load += se;

			_servers[no]->setCapacity(j, se, 0);
			nr = "";
			j++;
		}
		else
		{
			nr += data[i];
		}
	}
	_servers[no]->setLoaded(load);
	
}


CLoadBalancer::CLoadBalancer(int el, const char* IpAddr){

	_lastServer = 0;
	IpAddress=IpAddr;
	if (_serverDetails == NULL)
		_serverDetails = new ServerInfos[2];
	_serverDetails[0].number_of_cores = 2;
	_serverDetails[1].number_of_cores = 2;
	_Infos.number_of_managed_servers = noStations;
	_Infos.requests_per_minute.reserve(10);

	if (!initStations())
	{
		std::cout << "Error initianing stations \n";
		exit(1);
	}
	PortNOO = el;
	serverData = new _networkData[noStations];
	for (int i = 0; i < noStations; i++)
	{
		printf("[C++]LoadBalancer-->Initialization server connections %d \n", i);
	}
}

bool CLoadBalancer::initStations(){
	for (int i = 0; i < noStations; i++)
	{
		new CServer(noStations);
		_servers.push_back(new CServer(noStations));
	}
	if (_serversStatus == NULL)
		_serversStatus = new unsigned[noStations];
	if (_serversStatus != NULL)
		return true;
	return false;
}

CLoadBalancer::~CLoadBalancer(){

	//while(!waitForPacket());

	clearStations();
	if (_serverDetails != NULL)
	{
		delete[] _serverDetails;
		_serverDetails = NULL;
	}
}

int CLoadBalancer::clearStations(){

	if (_serversStatus != NULL)
	{
		delete[] _serversStatus;
		_serversStatus = NULL;
	}
	return 1;
}



void *CLoadBalancer::thread_timeFn(void *request){
	static int poz = 0;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto timePast = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
	if (timePast >= 10)
	{	
		if(_Infos.requests_per_minute.size()==10) {
			
			//_Infos.requests_per_minute[poz]=_request_in_a_minute;
			fit_10_request(_request_in_a_minute);
			//poz=poz+1%10;
		}else _Infos.requests_per_minute.push_back(_request_in_a_minute);
		
		begin = end;
		_request_in_a_minute = 0;
	}

	return NULL;

}

void *CLoadBalancer::thread_processFn(void *pck){

	if (_packetQueue.size() == QUEUE_SIZE - 1)
	{
		for (int i = 0; i < QUEUE_SIZE - 1; i++)
		{
			_request_in_a_minute++;
			_packetQueue.pop_back();
		}
	}
	else
	{
		_request_in_a_minute++;
		sendPacketNetwork((iPacket *)pck);
	}

	return NULL;
}

void *CLoadBalancer::thread_responsFn(void *pck){
	if (_responseQueue.size() > 0)
	{
		while (_responseQueue.size() > 0)
		{
			_responseQueue.pop_back();
			printf("[C++]LoadBalancer-->Packet process\n");
		}
	}

	return NULL;
}



void CLoadBalancer::initNetworkData(int i, int portno, const char *serverIP){

	{
		serverData[i].portno = portno;
		serverData[i].sockfd = socket(AF_INET, SOCK_STREAM, 0);

		if (serverData[i].sockfd < 0)
			printf("[C++]LoadBalancer-->ERROR opening socket");

		serverData[i].server = gethostbyname(serverIP);
		if (serverData[i].server == NULL)
		{
			printf("[C++]LoadBalancer-->ERROR, no such host\n");
			exit(0);
		}
		bzero((char *)&serverData[i].serv_addr, sizeof(serverData[i].serv_addr));
		serverData[i].serv_addr.sin_family = AF_INET;
		bcopy((char *)serverData[i].server->h_addr,
			  (char *)&serverData[i].serv_addr.sin_addr.s_addr,
			  serverData[i].server->h_length);
		serverData[i].serv_addr.sin_port = htons(portno);

		printf("[C++] Connecting to address %s, port %d\n", serverIP, portno);

	}
}

void CLoadBalancer::ConnectToServer(int i, const char *packType){

	int stat = connect(serverData[i].sockfd, (struct sockaddr *)&serverData[i].serv_addr, sizeof(serverData[i].serv_addr));
	if (stat < 0)
		printf("[C++]LoadBalancer-->ERROR connecting\n");

	bzero(serverData[i].buffer, 256);
	
	strcpy(serverData[i].buffer, packType);
	int n = write(serverData[i].sockfd, serverData[i].buffer, strlen(serverData[i].buffer));
	printf("------------------------------------------\n\n\n ");
	if (n < 0)
		printf("[C++]LoadBalancer-->ERROR writing to socket");
	bzero(serverData[i].buffer, 256);
	n = read(serverData[i].sockfd, serverData[i].buffer, 255);
	if (n < 0)
		printf("[C++]LoadBalancer-->ERROR reading from socket");

	setCapcity(i, serverData[i].buffer);
	int sum = 0;
	for (int i = 0; i < noStations; i++)
	{

		auto f = _servers[i]->getSolicitation();
		sum += f;
		_serverDetails[i].number_of_occupied_cores = f / 100 * 2;
		_serverDetails[i].cores_load = f;
	}
	_Infos.total_load = sum / noStations;
	
	close(serverData[i].sockfd);
}

void CLoadBalancer::sendPacketNetwork(iPacket *pck){


	int current = findAvailableStation();

	if (current != -1)
	{
		initNetworkData(current, PortNOO + current, IpAddress.c_str());
		ConnectToServer(current, "3");
	}
	else
	{
		printf("[C++]LoadBalancer-->Servers full packet added in queue: \n");

		for (int i = 0; i < noStations; i++)
		{
			initNetworkData(i % noStations, PortNOO + i % noStations, "127.0.0.1");
			ConnectToServer(i % noStations, "1010");
		}

		_packetQueue.push_back(pck);
	}
}

int CLoadBalancer::waitForPacket(){

	for (int i = 0; i < noStations; i++)
	{
		auto aa = _servers[i]->getSolicitation();
		if (aa != 0.0)
		{
			printf("[C++]LoadBalancer-->Need more time to process packet before exit\n");
			processPacket();
			return 0;
		}
	}
	return 1;
}

void CLoadBalancer::processPacket(){

	 for (int i = 0; i < noStations; i++) {
		_servers[i]->do_work();
		 if(_servers[i]->doneProcessing() == true) {
			std::cout << "Packet server"<<i<<" "<<"process\n";
			 _responseQueue.push_back(new CPacket);
		 }
	 }
}


void CLoadBalancer::sendPacket(iPacket *pck){
	processPacket();
	int current = findAvailableStation();
	if (current != -1)
	{
		_servers[current]->processPacket(*pck);
	}
	else
	{
		printf("[C++]LoadBalancer-->Servers full packet added in queue \n");
		_packetQueue.push_back(pck);
	}
}

void CLoadBalancer::fit_10_request(int el) {

	for(int i=1;i<_Infos.requests_per_minute.size();i++) {
		_Infos.requests_per_minute[i-1]= _Infos.requests_per_minute[i];
	}
	_Infos.requests_per_minute[9]=el;
}