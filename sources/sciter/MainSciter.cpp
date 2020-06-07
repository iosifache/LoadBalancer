#pragma region IncludedHeaders

#include "sciter-x.h"
#include "CWindow.h"
#include "Resources.cpp"

#pragma endregion

#pragma region SciterMainFunction

int uimain(std::function<int()> run){

	aux::asset_ptr<CWindow> pwin = new CWindow();

	// Enable features you may need in your scripts:
	SciterSetOption(NULL, SCITER_SET_SCRIPT_RUNTIME_FEATURES, ALLOW_FILE_IO | ALLOW_SOCKET_IO | ALLOW_EVAL | ALLOW_SYSINFO);

	// Bind resources[] with the archive
	sciter::archive::instance().open(aux::elements_of(resources));

	// Dedicated sciter::archive referencing
	pwin->load(WSTR("this://app/main.htm"));
	
	// Expand
	pwin->expand();

	// Return
	return run();

}

#pragma endregion