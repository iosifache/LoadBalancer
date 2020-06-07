#pragma region IncludedHeaders

#include <pthread.h>
#include <string>
#include <locale>
#include <codecvt>
#include "CWindow.h"
#include "ConnectionDetails.h"
#include "ColoredLog.h"
#include "aux-cvt.h"

#pragma endregion

#pragma region Configuration

#define SENDED_PACKETS 2000
#define MAX_BUFFER_LENGTH 128

#pragma endregion

#pragma region Methods

void *RunLoadBalancer(void *nothing_here){

    CLoadBalancer::getInstance(LOAD_BALANCER_PORT_NUMBER, "");
    CLoadBalancer::addPackets(SENDED_PACKETS);
    CLoadBalancer::running();

    // Return
    return NULL;

}

CWindow::CWindow(): window(SW_TITLEBAR | SW_CONTROLS | SW_MAIN | SW_ENABLE_DEBUG){

    pthread_create(&(this->load_balancer_thread), NULL, &RunLoadBalancer, NULL);

};

sciter::string CWindow::GetLoadBalancerInfos(){

    LoadBalancerInfos infos;
    std::basic_string<char16_t> result;
    std::string intermediate;
    char *serialized;
    int value;
    std::wstring_convert<std::codecvt_utf8<char16_t>,char16_t> convert;
    
    // Get informations
    infos = CLoadBalancer::GetLoadBalancerInfos();
    
    // Serialize informations
    serialized = CWindow::SerializeLoadBalancerInfos(&infos);
    COLORED_LOG(COLOR_GREEN, "Serialized datas about load balancer: %s", serialized);
    intermediate = serialized;
    result = convert.from_bytes(intermediate);

    return result;

}

sciter::string CWindow::GetServerInfo(sciter::value server_id){

    ServerInfos infos;
    std::basic_string<char16_t> result;
    std::string intermediate;
    char *serialized;
    int argument;
    std::wstring_convert<std::codecvt_utf8<char16_t>,char16_t> convert;
    
    // Get informations
    argument = server_id.get<int>();
    infos = CLoadBalancer::GetServerInfo(argument);
    
    // Serialize informations
    serialized = CWindow::SerializeServerInfos(&infos);
    COLORED_LOG(COLOR_GREEN, "Serialized datas about server #%d: %s", argument, serialized);
    intermediate = serialized;
    result = convert.from_bytes(intermediate);

    return result;

}

char *CWindow::SerializeLoadBalancerInfos(LoadBalancerInfos *infos){

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

char *CWindow::SerializeServerInfos(ServerInfos *infos){

    char *result;

    result = (char *)malloc(MAX_BUFFER_LENGTH * sizeof(char));
    sprintf(result, "%d / %d / %d", infos->number_of_cores, infos->number_of_occupied_cores, infos->cores_load);

    // Return
    return result;

}

#pragma endregion