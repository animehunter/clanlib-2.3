
#include "precomp.h"
#include "ui_screen.h"

UIScreen::UIScreen(CL_GUIManager *gui)
: CL_GUIComponent(gui, get_toplevel_description())
{
}

CL_GUITopLevelDescription UIScreen::get_toplevel_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_visible(false);
	return desc;
}
