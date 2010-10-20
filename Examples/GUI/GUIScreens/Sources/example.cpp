
#include "precomp.h"
#include "example.h"
#include "window_manager.h"
#include "splash_screen.h"

int Example::exec()
{
	CL_DisplayWindow display_window("ClanLib GUIScreens Example", 1024, 768);
	slots.connect(display_window.sig_window_close(), this, &Example::on_window_close);

	CL_GUIWindowManager wm = WindowManager::create(display_window);
	gui = CL_GUIManager(wm, "../../../Resources/GUIThemeAeroPacked");

	alpha = 0.0f;
	effect.set_transparency(alpha);

	SplashScreen splash_screen(&gui);
	splash_screen.set_geometry(CL_Rect(0,0,1024,768));
	splash_screen.set_visible(true);
	splash_screen.set_postprocess_effect(&effect);

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
	alpha += 0.01f;
	effect.set_transparency(alpha);
}