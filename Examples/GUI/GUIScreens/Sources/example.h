
#pragma once

class Example
{
public:
	int exec();

private:
	void on_window_close();

	CL_SlotContainer slots;
	CL_GUIManager gui;
};
