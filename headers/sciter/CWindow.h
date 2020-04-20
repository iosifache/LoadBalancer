#ifndef _CWINDOW_H

#define _CWINDOW_H

#pragma region IncludedHeaders

#include "sciter-x-window.hpp"
#include "CLoadBalancer.h"

#pragma endregion

#pragma region ClassDeclaration

class CWindow: public sciter::window{

	public:

		CWindow(): window(SW_TITLEBAR | SW_CONTROLS | SW_MAIN | SW_ENABLE_DEBUG){};

        sciter::string GetLoadBalancerInfos();
        sciter::string GetServerInfo(sciter::value server_id);

        // Map of native functions exposed to TIScript
		BEGIN_FUNCTION_MAP
            FUNCTION_0("GetLoadBalancerInfos", GetLoadBalancerInfos);
            FUNCTION_1("GetServerInfo", GetServerInfo);
		END_FUNCTION_MAP

    private:
        CLoadBalancer load_balancer;

};

#pragma endregion

#endif