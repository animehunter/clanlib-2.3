
#pragma once

class PostProcessEffect
{
public:
	virtual ~PostProcessEffect() {}
	virtual void render(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect geometry) = 0;
};

class PostProcessEffectDarken : public PostProcessEffect
{
public:
	PostProcessEffectDarken() {};

	void render(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect geometry);
};

class PostProcessEffectTransparency : public PostProcessEffect
{
public:
	PostProcessEffectTransparency();

	void render(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect geometry);
	void set_transparency(float alpha);

private:
	float alpha;
};
