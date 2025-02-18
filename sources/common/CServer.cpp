#include <pthread.h>
#include "CServer.h"
#include "CPacket.h"
#include "ColoredLog.h"

#define BASE_OPERATION 1000
#define AUTHENTIFICATION (BASE_OPERATION + 1)
#define GET (BASE_OPERATION + 2)
#define POST (BASE_OPERATION + 3)
#define TIME_OP 2
#define READY_STATE 50
#define BUSY_STATE 51

CServer::CServer(unsigned noProcess)
	: _noProcess(noProcess), _lastProcess(-1), _loaded(0){

	_process = new int[_noProcess];
	_myProcess.reserve(noProcess);
	if (_process == NULL)
		COLORED_LOG(COLOR_RED, "Error on creating process");
	for (int i = 0; i < _noProcess; i++)
	{
		_process[i] = READY_STATE;
		process tmp;
		tmp.state = READY_STATE;
		tmp.remainingTime = 0;
		_myProcess.push_back(tmp);
	}
}

CServer::CServer(int portno, unsigned noProcess)
	: _noProcess(noProcess), _lastProcess(-1), _loaded(0){

	_process = new int[_noProcess];
	_myProcess.reserve(noProcess);
	if (_process == NULL)
		COLORED_LOG(COLOR_RED, "Error on creating process");
	for (int i = 0; i < _noProcess; i++)
	{
		_process[i] = READY_STATE;
		process tmp;
		tmp.state = READY_STATE;
		tmp.remainingTime = 0;
		_myProcess.push_back(tmp);
	}
	initConnection(portno);
}

// CServer::CServer() {
//	_process = new int[2]; // CONST VALUE CHANGE
//}

CServer::~CServer(){

	// while(allProcessReady() == true)
	// 	do_work();

	close(_serverCon.newsockfd);
	close(_serverCon.sockfd);
}

iPacket *CServer::processPacket(iPacket &obj){

	srand(time(NULL));
	int time = rand() % 3 + 1;
	while (time-- > 0)
	{
		do_work();
	}
	int timp = selectTask(obj.getMethod());

	if (timp < 0)
	{

		return NULL;
	}
	// if (_loaded == _noProcess)
	// 	return NULL;

	int nextServer = findAvailableProcess();

	if (nextServer != -1)
	{
		COLORED_LOG(COLOR_BLUE, "Core #%d received a task to process", nextServer);
		setProcessBusy(nextServer, timp);
	}
	return NULL;
}

bool CServer::doneProcessing(){

	for (int i = 0; i < _noProcess; i++)
	{
		if (_myProcess[i].remainingTime == 0 && _myProcess[i].state == BUSY_STATE)
		{
			_myProcess[i].state = READY_STATE;
			_loaded -= 1;
			return true;
		}
	}
	return false;
}

bool CServer::isAvailable(){
	for (int i = 0; i < _noProcess; i++)
	{
		if (_myProcess[i].state == READY_STATE)
			return true;
	}
	return false;
}

float CServer::getSolicitation(){

	float var = (float)_loaded / _noProcess;
	return var * 100;
}

int CServer::findAvailableProcess(){
	bool findServer = false;
	int no = (_lastProcess + 1) % _noProcess;

	while (no != _lastProcess)
	{
		if (_myProcess[no].state == READY_STATE)
		{
			//_lastProcess = no;
			return no;
		}
		no = (no + 1) % _noProcess;
	}

	return -1;
}

int CServer::selectTask(int task){

	int timeProcessing = 1;

	int factor = _loaded;
	if (_loaded < 1)
		factor = 2;
	factor = rand() % 10 + 3;
	switch (task)
	{
	case 3:
		timeProcessing = TIME_OP * (factor + 1);
		break;
	case 1010:
		timeProcessing = -1;
		break;
	case AUTHENTIFICATION:
		timeProcessing = TIME_OP * (factor + 1);
		break;
	case GET:
		timeProcessing = (TIME_OP + 1) * (factor + 1);
		break;
	case POST:
		timeProcessing = (TIME_OP + 2) * (factor + 1);
		break;
	default:
		timeProcessing = (TIME_OP) * (factor + 1);
		break;
	}

	return timeProcessing;
}

bool CServer::allProcessReady(){
	for (int i = 0; i < _noProcess; i++)
	{
		if (_myProcess[i].state == BUSY_STATE)
		{
			return false;
		}
	}
	return true;
}

void CServer::do_work(){
	char me = 0;
	for (int i = 0; i < _noProcess; i++)
	{
		if (_myProcess[i].state == BUSY_STATE)
		{
			_myProcess[i].remainingTime -= 1;
			if (_myProcess[i].remainingTime == 0)
			{
				COLORED_LOG(COLOR_GREEN, "Core #%d finished the processing of a request", i);
				_myProcess[i].state = READY_STATE;
				_loaded -= 1;
			}
		}
	}
}

void CServer::setProcessBusy(int no, int time){
	_lastProcess = no;
	_myProcess[_lastProcess].state = BUSY_STATE;
	_myProcess[_lastProcess].remainingTime = time;
	// //_loaded =_loaded+ 1;
}

void CServer::initConnection(int portno){
	_serverCon.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverCon.sockfd < 0)
		COLORED_LOG(COLOR_RED, "Error on creating a socket", NULL);
	bzero((char *)&_serverCon.serv_addr, sizeof(_serverCon.serv_addr));
	portno = portno;
	_serverCon.serv_addr.sin_family = AF_INET;
	_serverCon.serv_addr.sin_addr.s_addr = INADDR_ANY;
	_serverCon.serv_addr.sin_port = htons(portno);
	int st = bind(_serverCon.sockfd, (struct sockaddr *)&_serverCon.serv_addr,
				  sizeof(_serverCon.serv_addr));
	if (st < 0)
		COLORED_LOG(COLOR_RED, "Error on socket binding", NULL);
}

void *CServer::thread_processFn(void *pck) { return NULL; }

void CServer::listenForConnection(){
	listen(_serverCon.sockfd, 5);
	_serverCon.clilen = sizeof(_serverCon.cli_addr);
	_serverCon.newsockfd =
		accept(_serverCon.sockfd, (struct sockaddr *)&_serverCon.cli_addr,
			   &_serverCon.clilen);
	if (_serverCon.newsockfd < 0)
	COLORED_LOG(COLOR_RED, "Error on socket accepting", NULL);
	bzero(_serverCon.buffer, 256);
	std::string mess = "";
	int n = read(_serverCon.newsockfd, _serverCon.buffer, 255);
	if (n < 0)
		COLORED_LOG(COLOR_RED, "Error on reading from socket", NULL);
	COLORED_LOG(COLOR_BLUE, "A new request was received", NULL);
	{
		CPacket tmp(_serverCon.buffer);

		// th = std::thread(thread_processFn,(void *)&tmp);
		// th.join();
		// pthread_join(thProcess, NULL);

		processPacket(tmp);

		// n = write(_serverCon.newsockfd,mess.c_str(),mess.size());

		for (int i = 0; i < _noProcess; i++)
		{
			if (_myProcess[i].state == BUSY_STATE)
				mess += "1;";
			else
				mess += "0;";
		}
	}
	n = write(_serverCon.newsockfd, mess.c_str(), mess.size());
	if (n < 0)
		COLORED_LOG(COLOR_RED, "Error on writing to socket", NULL);
}

void CServer::setCapacity(int nrProcess, int val, int load){
	//_loaded= load;
	if (val == 0)
	{
		if (_loaded > 0)
			// if(_myProcess[nrProcess].state == BUSY_STATE)
			// 	_loaded--;
			_myProcess[nrProcess].state = READY_STATE;
	}
	else
	{
		// if(_myProcess[nrProcess].state == READY_STATE)
		//_loaded++;
		_myProcess[nrProcess].state = BUSY_STATE;
	}
}