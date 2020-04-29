#include "CLoadBalancer.h"
CLoadBalancer *CLoadBalancer::_pInstance = NULL;
std::vector<iStation *> CLoadBalancer::_servers;
unsigned *CLoadBalancer::_serversStatus = NULL;

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

static int PortNOO = 0;
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

void CLoadBalancer::processPacket(){

	//  for (int i = 0; i < noStations; i++) {
	// 	// _servers[i]->do_work();
	// 	 if(_servers[i]->doneProcessing() == true) {
	// 		// std::cout << "Packet server"<<i<<" "<<"process\n";
	// 		 _responseQueue.push_back(new CPacket);
	// 	 }
	//  }
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

int CLoadBalancer::clearStations(){

	if (_serversStatus != NULL)
	{
		delete[] _serversStatus;
		_serversStatus = NULL;
	}
	return 1;
}

CLoadBalancer::CLoadBalancer(int el){

	_lastServer = 0;
	if (_serverDetails == NULL)
		_serverDetails = new ServerInfos[2];
	_serverDetails[0].number_of_cores = 2;
	_serverDetails[1].number_of_cores = 2;
	_Infos.number_of_managed_servers = noStations;

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
CLoadBalancer::~CLoadBalancer(){

	//while(!waitForPacket());

	clearStations();
	if (_serverDetails != NULL)
	{
		delete[] _serverDetails;
		_serverDetails = NULL;
	}
}

void *CLoadBalancer::thread_timeFn(void *request){
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto timePast = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	if (timePast > 60)
	{
		_Infos.requests_per_minute.push_back((int)_request_in_a_minute);
		begin = end;
		_request_in_a_minute = 0;
	}
}

void *CLoadBalancer::thread_processFn(void *pck){

	if (_packetQueue.size() == QUEUE_SIZE - 1)
	{
		//printf("Packet Queue full\n Wait for processing\n");

		for (int i = 0; i < QUEUE_SIZE - 1; i++)
		{

			_request_in_a_minute++;
			_packetQueue.pop_back();
		}
		//printf("[C++]LoadBalancer-->Queue empty\n");
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

CLoadBalancer *CLoadBalancer::getInstance(int el){
	if (_pInstance == NULL)
		_pInstance = new CLoadBalancer(el);

	return _pInstance;
}

int CLoadBalancer::resetConnection(){

	if (_pInstance != NULL)
	{
		delete _pInstance;
	}
	_pInstance = new CLoadBalancer(9);
	if (_pInstance == NULL)
		return -1;
	return 1;
}

void CLoadBalancer::sendRespons(iPacket &pck){

	{

		//auto i =_responseQueue.pop();
		std::cout << "Respond sent to ip \n"; //<< pck.getSourceIp << " on port " << pck.getSourcePort() << std::endl;
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

void CLoadBalancer::sendPacketNetwork(iPacket *pck){

	//processPacket();
	int current = findAvailableStation();
	//printf("\n FIND   %d \n",current);

	if (current != -1)
	{
		initNetworkData(current, PortNOO + current, "127.0.0.1");
		ConnectToServer(current, "3");
		//_servers[current]->processPacket(*pck);
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

void CLoadBalancer::sendPacketNetworkA(iPacket *pck){

	// for(int i=0;i< noStations;i++) {
	// 	initNetworkData(i,PortNOO+i,"127.0.0.1");
	// 	ConnectToServer(i,"1010");
	// }
}

bool CLoadBalancer::queueIsFull(){
	/*if(_packetQueue.size() == QUEUE_SIZE)
		return true;*/
	return false;
}

void CLoadBalancer::getWorkCapacity(){
	// std::cout << "Nr of server: " << noStations<<"\n";

	// for (int i = 0; i < noStations; i++) {
	// 	auto aa = _servers[i]->getSolicitation();
	// 	std::cout<<"Server "<<i<<" : "<<_servers[i]->getSolicitation()<<"% \n";

	// }
	for (int i = 0; i < noStations; i++)
	{
		//initNetworkData(i,22133,"127.0.0.1");
		ConnectToServer(i, "getCapacity");
	}
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
	}
}
void CLoadBalancer::ConnectToServer(int i, char *packType){

	int stat = connect(serverData[i].sockfd, (struct sockaddr *)&serverData[i].serv_addr, sizeof(serverData[i].serv_addr));
	if (stat < 0)
		printf("[C++]LoadBalancer-->ERROR connecting\n");

	bzero(serverData[i].buffer, 256);
	//fgets(serverData[i].buffer,255,stdin);
	strcpy(serverData[i].buffer, packType);
	int n = write(serverData[i].sockfd, serverData[i].buffer, strlen(serverData[i].buffer));
	printf("------------------------------------------\n\n\n ");
	if (n < 0)
		printf("[C++]LoadBalancer-->ERROR writing to socket");
	bzero(serverData[i].buffer, 256);
	n = read(serverData[i].sockfd, serverData[i].buffer, 255);
	if (n < 0)
		printf("[C++]LoadBalancer-->ERROR reading from socket");

	//sprintf("%s\n",serverData[i].buffer);
	//printf("Buffer %s\n",serverData[i].buffer);

	//printf("Server nr:%d  values:%s\n",i,serverData[i].buffer);
	setCapcity(i, serverData[i].buffer);
	int count = 0;
	for (int i = 0; i < noStations; i++)
	{

		auto f = _servers[i]->getSolicitation();
		if (f != 0)
			count++;
		printf("[C++]LoadBalancer_Server %d = %f\n", i, f);
		_serverDetails[i].number_of_cores = 2 - f / 100 * 2;
		_serverDetails[i].cores_load = f;
	}
	_Infos.total_load = noStations * 2 / count * 100;
	//printf("Con end\n");
	close(serverData[i].sockfd);
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

void CLoadBalancer::removeInstance(){
	delete _pInstance;
}

void CLoadBalancer::setCapcity(int no, char *data){
	//printf( "Server %d ",no);
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
	//printf("END \n");
}

LoadBalancerInfos CLoadBalancer::GetLoadBalancerInfos(){

	return _Infos;
}

ServerInfos CLoadBalancer::GetServerInfo(int server_id){

	return _serverDetails[server_id];
}