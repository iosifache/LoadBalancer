
#include <iostream>
#include <vector>
#include "CServer.h"

int main(int argc, char **argv){

	printf("[CServer C++]-->Init server on port: %s \n", argv[1]);
	CServer s1(atoi(argv[1]), 2);
	while (1){
		s1.listenForConnection();
	}

	return 0;

}