#include <iostream>
#include <vector>
#include "CLoadBalancer.h"

int main(int argc, char **argv){

	if(argc <3) {
		printf("Usage <PortNo> <IpAdrres>\n");
		exit(1);
	}

	CLoadBalancer::getInstance(atoi(argv[1]),argv[2]);
	CLoadBalancer::addPackets(20);
	CLoadBalancer::running();
	CLoadBalancer::removeInstance();

}