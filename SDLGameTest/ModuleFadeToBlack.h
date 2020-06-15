#pragma once

#include "Module.h"

class Scene;

class ModuleFadeToBlack : public Module
{
public:
	ModuleFadeToBlack();
	~ModuleFadeToBlack();

	bool Start() override;
	UpdateStatus Update() override;
	void FadeToBlack(Scene* sceneOn, Scene* sceneOff = nullptr, float time = 1.0f);
	bool isFading() const;

private:

	int m_StartTime = 0;
	int m_TotalTime = 0;
	bool m_FadingIn = true;
	Scene* m_SceneOn = nullptr;
	Scene* m_SceneOff = nullptr;
};