#include <iostream>
#include <vector>
#include "CLoadBalancer.h"

int main(int argc, char **argv){

	CLoadBalancer::getInstance(atoi(argv[1]));
	CLoadBalancer::addPackets(20);
	CLoadBalancer::running();
	CLoadBalancer::removeInstance();

}