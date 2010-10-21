
#include "precomp.h"
#include "example.h"
#include "window_manager.h"
#include "splash_screen.h"
#include "error_screen.h"

int Example::exec()
{
	CL_DisplayWindow display_window("ClanLib GUIScreens Example", 1024, 768);
	slots.connect(display_window.sig_window_close(), this, &Example::on_window_close);

	CL_GUIWindowManager wm = WindowManager::create(display_window);
	gui = CL_GUIManager(wm, "../../../Resources/GUIThemeAeroPacked");

	SplashScreen splash_screen(&gui);
	splash_screen.set_geometry(CL_Rect(0,0,1024,768));
	splash_screen.set_visible(true);
	splash_screen.set_postprocess_effect(&darken_effect);

	ErrorScreen error_screen(&gui);
	error_screen.set_geometry(CL_Rect(200,200,824,568));
	error_screen.set_visible(true);
	error_screen.set_postprocess_effect(&transparency_effect);

	transparency_effect.set_transparency(0.0f);

	CL_Timer t;
	t.func_expired().set(this, &Example::on_timer);
	t.start(50);

	return gui.exec();
}

void Example::on_window_close()
{
	gui.exit_with_code(0);
}

void Example::on_timer()
{
	transparency_effect.set_transparency(transparency_effect.get_transparency() + 0.01f);
}