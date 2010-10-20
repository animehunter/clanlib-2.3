
#pragma once

#include "postprocess_effect.h"

class Example
{
public:
	int exec();

private:
	void on_window_close();
	void on_timer();

	CL_SlotContainer slots;
	CL_GUIManager gui;

	float alpha;
	PostProcessEffectTransparency effect;
};
