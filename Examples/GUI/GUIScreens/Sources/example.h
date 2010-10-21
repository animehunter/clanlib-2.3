
#pragma once

class PostProcessScene;
class PostProcessEffectTransparency;

class Example
{
public:
	int exec();

private:
	void on_window_close();
	void on_timer();

	CL_SlotContainer slots;
	CL_GUIManager gui;

	PostProcessEffectTransparency *effect_transparency;
	CL_Timer timer;
};
