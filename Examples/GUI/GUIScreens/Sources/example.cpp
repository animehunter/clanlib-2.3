
#include "precomp.h"
#include "example.h"
#include "window_manager.h"
#include "splash_screen.h"
#include "postprocess_effect.h"

int Example::exec()
{
	CL_DisplayWindow display_window("ClanLib GUIScreens Example", 1024, 768);
	slots.connect(display_window.sig_window_close(), this, &Example::on_window_close);

	CL_GUIWindowManager wm = WindowManager::create(display_window);
	gui = CL_GUIManager(wm, "../../../Resources/GUIThemeAeroPacked");

	PostProcessEffectTransparency effect;
	effect.set_transparency(0.5f);

	SplashScreen splash_screen(&gui);
	splash_screen.set_geometry(CL_Rect(0,0,1024,768));
	splash_screen.set_visible(true);
	splash_screen.set_postprocess_effect(&effect);

	return gui.exec();
}

void Example::on_window_close()
{
	gui.exit_with_code(0);
}
