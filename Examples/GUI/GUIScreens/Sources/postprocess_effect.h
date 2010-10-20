
#pragma once

class PostProcessEffect
{
public:
	PostProcessEffect() {};
	virtual ~PostProcessEffect() {};
};

class PostProcessEffectDarken : public PostProcessEffect
{
public:
	PostProcessEffectDarken();
	virtual ~PostProcessEffectDarken() {};
};
