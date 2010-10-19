
#include "precomp.h"
#include "program.h"
#include "window_manager.h"
#include <ClanLib/application.h>

CL_ClanApplication clanapp(&Program::main);

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	CL_SetupGUI setup_gui;
	CL_DisplayWindow dispwindow("ClanLib GUIScreens Example", 1024, 768);
	CL_GUIWindowManager wm = WindowManager::create(dispwindow);
	CL_GUIManager gui(wm, "../../../Resources/GUIThemeAeroPacked/theme.css");

	return gui.exec();
}
