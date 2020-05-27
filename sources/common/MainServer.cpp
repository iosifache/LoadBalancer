
#include <iostream>
#include <vector>
#include "CServer.h"

int main(int argc, char **argv){

	printf("[CServer C++]-->Init server on port: %s \n", PORT_NO );
	CServer s1( PORT_NO ,2);
	while (1){
		s1.listenForConnection();
	}

	return 0;

}
