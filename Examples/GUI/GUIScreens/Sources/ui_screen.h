
#pragma once

class PostProcessEffect;


class UIScreen : public CL_GUIComponent
{
public:
	UIScreen(CL_GUIManager *gui);

public:
	void set_postprocess_effect(const PostProcessEffect &effect);
	void remove_postprocess_effect();

private:
	static CL_GUITopLevelDescription get_toplevel_description();
};
