
#pragma once

#include "ui_screen.h"

class SplashScreen : public UIScreen
{
public:
	SplashScreen(CL_GUIManager *gui);
	~SplashScreen();

private:
	CL_Label *label;
};
