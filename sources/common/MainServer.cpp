
#include <iostream>
#include <vector>
#include "CServer.h"
#include "ColoredLog.h"

int main(int argc, char **argv){

	COLORED_LOG(COLOR_BLUE, "Init server on port %d", SERVER_PORT_NUMBER);

	CServer s1( SERVER_PORT_NUMBER ,2);
	while (1){
		s1.listenForConnection();
	}

	return 0;

}
