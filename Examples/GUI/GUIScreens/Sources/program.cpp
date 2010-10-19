
#include "precomp.h"
#include "program.h"
#include "window_manager.h"
#include "splash_screen.h"
#include <ClanLib/application.h>

CL_ClanApplication clanapp(&Program::main);

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	CL_SetupGUI setup_gui;

	try
	{
		CL_DisplayWindow dispwindow("ClanLib GUIScreens Example", 1024, 768);
		CL_GUIWindowManager wm = WindowManager::create(dispwindow);
		CL_GUIManager gui(wm, "../../../Resources/GUIThemeAeroPacked");

		SplashScreen splash_screen(&gui);

		return gui.exec();
	}
	catch(CL_Exception &exception)
	{
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Error: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
}
