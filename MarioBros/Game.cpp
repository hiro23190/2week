#include "Game.h"

#include <DxLib.h>

#include "Scene.h"
#include "TittleScene.h"
#include "GamePlayingScene.h"

constexpr int screen_x_size = 512;
constexpr int screen_y_size = 480;

Game::Game()
{
}


Game::~Game()
{
}

void Game::Init()
{
	DxLib::SetGraphMode(screen_x_size, screen_y_size, 32);
	DxLib::SetMainWindowText("1701327_“¡Š|—T–ç_MarioBros");
	DxLib::ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return;
	}

	_screen = MakeScreen(screen_x_size, screen_y_size);
	_pos = Vector2();
	_shakePower = 0;

	ChangeScene(new TittleScene());
}

void Game::Run()
{
	int flame = 0;

	while (ProcessMessage() == 0) {
		if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}
		flame++;


		DxLib::SetDrawScreen(_screen);
		DxLib::ClsDrawScreen();

		_scene->Update();

		DxLib::SetDrawScreen(DX_SCREEN_BACK);
		DxLib::ClsDrawScreen();

		DrawGraph(_pos.x, _pos.y - _shakePower, _screen, true);

		DxLib::ScreenFlip();

		_shakePower = max(--_shakePower, 0);
	}
}

void Game::End()
{
	DxLib_End();
}

void Game::ChangeScene(Scene * scene)
{
	_scene.reset(scene);
}

const Vector2 Game::GetWindowSize() const
{
	return Vector2(screen_x_size, screen_y_size);
}

void Game::SetScreenShake(const int& in)
{
	_shakePower = in;
}
