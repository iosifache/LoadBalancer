#pragma region IncludedHeaders

#include <string>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "CLoadBalancer.h"

#pragma endregion

#pragma region Methods

CLoadBalancer::CLoadBalancer(){

    // Initialize random seed
    srand(time(0));

    // Initialize infos
    this->own_infos.number_of_managed_servers = NUMBER_OF_SERVERS;

}

LoadBalancerInfos CLoadBalancer::GetLoadBalancerInfos(){

    this->ComputeRandomInfos();

    // Return
    return this->own_infos;

}

ServerInfos CLoadBalancer::GetServerInfo(int server_id){

    ServerInfos infos;

    // Initialize members
    infos.number_of_cores = rand() % MAX_NUMBER_OF_CORES;
    if (infos.number_of_cores != 0){
        infos.number_of_occupied_cores = rand() % infos.number_of_cores;
        infos.cores_load = (float)((float)infos.number_of_occupied_cores / (float)infos.number_of_cores) * 100;
    }
    else{
        infos.number_of_occupied_cores = 0;
        infos.cores_load = 0;
    }
        
    

    // Return
    return infos;

}

void CLoadBalancer::ComputeRandomInfos(){
    
    int i;

    // Initialize members
    this->own_infos.total_load = rand() % MAX_TOTAL_LOAD;
    this->own_infos.requests_per_minute.clear();
    for (i = 0; i < REQUESTS_COUNT; i++)
        this->own_infos.requests_per_minute.push_back(rand() % MAX_NUMER_OF_REQUESTS);

}

char *CLoadBalancer::SerializeLoadBalancerInfos(LoadBalancerInfos *infos){

    char intermediate[32];
    char *result;

    result = (char *)malloc(MAX_BUFFER_LENGTH * sizeof(char));
    sprintf(result, "%d / %d", infos->number_of_managed_servers, infos->total_load);
    for (auto const& value: infos->requests_per_minute){
        sprintf(intermediate, " / %d", value);
        strcat(result, intermediate);
    }

    // Return
    return result;

}

char *CLoadBalancer::SerializeServerInfos(ServerInfos *infos){

    char *result;

    result = (char *)malloc(MAX_BUFFER_LENGTH * sizeof(char));
    sprintf(result, "%d / %d / %d", infos->number_of_cores, infos->number_of_occupied_cores, infos->cores_load);

    // Return
    return result;

}

#pragma endregion