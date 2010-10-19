
#pragma once

class WindowManager;

class TopLevelWindow
{
public:
	TopLevelWindow(WindowManager *wm, CL_GUITopLevelWindow *owner, CL_GUITopLevelDescription description);

	bool is_visible() const { return visible; }
	bool is_enabled() const { return enabled; }
	bool is_minimized() const { return minimized; }
	bool is_maximized() const { return maximized; }
	const CL_Rect &get_geometry() const { return geometry; }

	void set_visible(bool show, bool activate);
	void set_geometry(const CL_Rect &box);
	void set_enabled(bool enable) { enabled = enable; }

private:
	void create_texture();
	void destroy_texture();

	WindowManager *wm;
	CL_GUITopLevelWindow *owner;
	CL_GUITopLevelDescription description;
	bool visible;
	bool enabled;
	bool minimized;
	bool maximized;
	CL_Rect geometry;
	CL_Texture texture;
};
