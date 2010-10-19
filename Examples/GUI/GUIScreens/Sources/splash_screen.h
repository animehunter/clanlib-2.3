#pragma once

class SplashScreen : public CL_GUIComponent
{
public:
	SplashScreen(CL_GUIManager *gui);
	~SplashScreen();

private:
	CL_Label *label;
};
