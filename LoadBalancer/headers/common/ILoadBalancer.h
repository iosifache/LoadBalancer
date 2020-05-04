#ifndef _ILOADBALANCER_H 

#define _ILOADBALANCER_H

#pragma region IncludedHeaders

#include <vector>

#pragma endregion

#pragma region Structures

typedef struct {
    int number_of_managed_servers;
    int total_load;
    std::vector<int> requests_per_minute;
} LoadBalancerInfos;

typedef struct {
    int number_of_cores;
    int number_of_occupied_cores;
    int cores_load;
} ServerInfos;

#pragma endregion

#pragma region InterfaceDeclaration

class ILoadBalancer{
    public:
        virtual LoadBalancerInfos GetLoadBalancerInfos() const = 0;
        virtual ServerInfos GetServerInfo(int server_id) const = 0;
};

#pragma endregion

#endif