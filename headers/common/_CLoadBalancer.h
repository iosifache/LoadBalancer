#ifndef _CLOADBALANCER_H

#define _CLOADBALANCER_H

#pragma region IncludedHeaders

#include "ILoadBalancer.h"

#pragma endregion

#pragma region Configuration

#define MAX_BUFFER_LENGTH                128
#define NUMBER_OF_SERVERS                4
#define MAX_TOTAL_LOAD                   100
#define REQUESTS_COUNT                   10
#define MAX_NUMER_OF_REQUESTS            100
#define MAX_NUMBER_OF_CORES              8

#pragma endregion

#pragma region ClassDeclaration

class CLoadBalancer{
    public:
        CLoadBalancer();
        LoadBalancerInfos GetLoadBalancerInfos();
        ServerInfos GetServerInfo(int server_id);
        static char *SerializeLoadBalancerInfos(LoadBalancerInfos *infos);
        static char *SerializeServerInfos(ServerInfos *infos);
    private:
        LoadBalancerInfos own_infos;
        void ComputeRandomInfos();
};

#pragma endregion

#endif