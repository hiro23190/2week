#pragma once
#include "Scene.h"
class TittleScene :
	public Scene
{
private:
	// ‰æ‘œ
	int _img_Tittle;
	int _img_Point;

	int _wait;

	void (TittleScene::*_updater)();

	void NormalUpdate();
	void FadeoutUpdate();

public:
	TittleScene();
	~TittleScene();

	void Update();
};

