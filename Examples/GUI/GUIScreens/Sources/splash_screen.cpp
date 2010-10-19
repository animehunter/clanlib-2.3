
#include "precomp.h"
#include "splash_screen.h"

SplashScreen::SplashScreen(CL_GUIManager *gui)
: UIScreen(gui)
{
	label = new CL_Label(this);
	label->set_geometry(CL_Rect(10, 10, CL_Size(300,40)));
	label->set_text("Loading..");
}

SplashScreen::~SplashScreen()
{
}
