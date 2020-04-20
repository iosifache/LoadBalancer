#pragma region IncludedHeaders

#include <string>
#include <locale>
#include <codecvt>
#include "CWindow.h"
#include "aux-cvt.h"

#pragma endregion

#pragma region Methods

sciter::string CWindow::GetLoadBalancerInfos(){

    LoadBalancerInfos infos;
    std::basic_string<char16_t> result;
    std::string intermediate;
    char *serialized;
    int value;
    std::wstring_convert<std::codecvt_utf8<char16_t>,char16_t> convert;
    
    // Get informations
    infos = this->load_balancer.GetLoadBalancerInfos();
    
    // Serialize informations
    serialized = CLoadBalancer::SerializeLoadBalancerInfos(&infos);
    printf("[C++] Serialized datas about load balancer: %s\n", serialized);
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
    infos = this->load_balancer.GetServerInfo(argument);
    
    // Serialize informations
    serialized = CLoadBalancer::SerializeServerInfos(&infos);
    printf("[C++] Serialized datas about server #%d: %s\n", argument, serialized);
    intermediate = serialized;
    result = convert.from_bytes(intermediate);

    return result;

}

#pragma endregion