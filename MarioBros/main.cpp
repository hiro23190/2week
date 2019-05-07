#include "Game.h"

#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
		auto& game = Game::GetInstance();

	game.Init();
	game.Run();
	game.End();

	return 0;
}