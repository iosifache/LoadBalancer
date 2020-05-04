#pragma region IncludedHeaders

#include <stdio.h>
#include <string>
#include <string.h>
#include "CLoadBalancer.h"

#pragma endregion

#pragma region MainFunction

int main(){

    CLoadBalancer load_balancer;
    LoadBalancerInfos infos;
    ServerInfos server_infos;
    char *serialized;

    // Log formatted datas about load balancer
    infos = load_balancer.GetLoadBalancerInfos();
    printf("[+] Datas about load balancer\n");
    printf("- number of managed servers: %d\n", infos.number_of_managed_servers);
    printf("- total load: %d\n", infos.total_load);
    printf("- requests array: ");
    for (auto const& value: infos.requests_per_minute){
        printf("%d ", value);
    };
    printf("\n");

    // Log serialized datas about load balancer
    serialized = CLoadBalancer::SerializeLoadBalancerInfos(&infos);
    printf("- serialized data: %s\n\n", serialized);
    free(serialized);

    // Log formatted datas about first server
    server_infos = load_balancer.GetServerInfo(0);
    printf("[+] Datas about first server\n");
    printf("- number of cores: %d\n", server_infos.number_of_cores);
    printf("- number of occupied cores %d\n", server_infos.number_of_occupied_cores);

    // Log serialized datas about first server
    serialized = CLoadBalancer::SerializeServerInfos(&server_infos);
    printf("- serialized data: %s\n", serialized);
    free(serialized);

    // Return
    return 0;

}

#pragma endregion