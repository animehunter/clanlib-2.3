
#pragma once

class PostProcessScene;

class Example
{
public:
	int exec();

private:
	void on_window_close();
	void on_timer(PostProcessScene &scene);

	CL_SlotContainer slots;
	CL_GUIManager gui;
};
