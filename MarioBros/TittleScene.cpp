#include "TittleScene.h"

#include <DxLib.h>

#include "Game.h"
#include "Geometry.h"
#include "GamePlayingScene.h"

constexpr int WAIT_TIME = 30;

void TittleScene::NormalUpdate()
{
	auto& size = Game::GetInstance().GetWindowSize();

	DrawExtendGraph(0, 0, size.x, size.y, _img_Tittle, true);
	DrawGraph(size.x / 6, size.y / 2, _img_Point, true);

	if (CheckHitKey(KEY_INPUT_SPACE)) {
		_updater = &TittleScene::FadeoutUpdate;
	}
}

void TittleScene::FadeoutUpdate()
{
	auto& size = Game::GetInstance().GetWindowSize();

	_wait++;
	DrawBox(0, 0, size.x, size.y, 0x000000, true);

	if (_wait > WAIT_TIME)
		Game::GetInstance().ChangeScene(new GamePlayingScene());
}

TittleScene::TittleScene()
{
	_img_Tittle = LoadGraph("img/Tittle.png");
	_img_Point = LoadGraph("img/Point_Tittle.png");

	_wait = 0;
	_updater = &TittleScene::NormalUpdate;
}


TittleScene::~TittleScene()
{
}

void TittleScene::Update()
{
	(this->*_updater)();
}
