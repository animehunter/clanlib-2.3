
#include "precomp.h"
#include "splash_screen.h"

SplashScreen::SplashScreen(CL_GUIManager *gui)
: UIScreen(gui)
{
	label = new CL_Label(this);
	label->set_geometry(CL_Rect(10, 10, CL_Size(300,40)));
	label->set_text("Loading..");

	button = new CL_PushButton(this);
	button->set_geometry(CL_Rect(10, 50, CL_Size(100, 30)));
	button->set_text("Continue");
	button->func_clicked().set(this, &SplashScreen::on_button_click);

	func_render().set(this, &SplashScreen::on_render);
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::on_render(CL_GraphicContext &gc, const CL_Rect &rect)
{
	gc.clear(CL_Colorf::orangered);
}

void SplashScreen::on_button_click()
{

}