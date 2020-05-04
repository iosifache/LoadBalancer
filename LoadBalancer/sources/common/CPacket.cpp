#include "CPacket.h"
#include <string.h>

CPacket::CPacket(){
}

CPacket::CPacket(const char *srcIP, unsigned srcProt) : _sourceIP(srcIP), _sourcePort(srcProt){
	_method;
}

CPacket::CPacket(char *meth){

	_method = atoi(meth);
}

CPacket::~CPacket(){
}

char *CPacket::getSourceIp(){
	return (char *)_sourceIP.c_str();
}

unsigned CPacket::getSourcePort(){
	return _sourcePort;
}

bool CPacket::decapsulation(){
	return false;
}

unsigned CPacket::getMethod(){
	return _method;
}
