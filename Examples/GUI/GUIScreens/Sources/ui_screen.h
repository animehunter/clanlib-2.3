
#pragma once

class UIScreen : public CL_GUIComponent
{
public:
	UIScreen(CL_GUIManager *gui);

private:
	static CL_GUITopLevelDescription get_toplevel_description();
};
