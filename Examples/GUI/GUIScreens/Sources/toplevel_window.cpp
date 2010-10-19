
#include "precomp.h"
#include "toplevel_window.h"

TopLevelWindow::TopLevelWindow(WindowManager *wm, CL_GUITopLevelWindow *owner, CL_GUITopLevelDescription description)
: wm(wm), owner(owner), description(description)
{
}
