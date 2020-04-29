#ifndef _CWINDOW_H

#define _CWINDOW_H

#pragma region IncludedHeaders

#include "sciter-x-window.hpp"
#include "CLoadBalancer.h"

#pragma endregion

#pragma region ClassDeclaration

class CWindow: public sciter::window{

	public:

        // Constructor
		CWindow();

        // Method that informations about load balancer and servers
        sciter::string GetLoadBalancerInfos();
        sciter::string GetServerInfo(sciter::value server_id);

        // Map of native functions exposed to TIScript
		BEGIN_FUNCTION_MAP
            FUNCTION_0("GetLoadBalancerInfos", GetLoadBalancerInfos);
            FUNCTION_1("GetServerInfo", GetServerInfo);
		END_FUNCTION_MAP

    private:

        // Members
        pthread_t load_balancer_thread;

        // Methods that serialize infos that will be sended to UI
        static char *SerializeLoadBalancerInfos(LoadBalancerInfos *infos);
        static char *SerializeServerInfos(ServerInfos *infos);

};

#pragma endregion

#endif