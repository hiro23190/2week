#pragma once
#include "Scene.h"

#include <memory>
#include <random>

class StageLoader;
class Mario;
class EnemyFactory;
class Floor;

class GamePlayingScene :
	public Scene
{
private:
	int _img_PHASE;
	int _img_P;
	int _img_Number;
	int _img_GameOver;

	std::shared_ptr<StageLoader>	_loader;
	std::shared_ptr<Mario>			_mario;
	std::shared_ptr<EnemyFactory>	_factory;
	std::shared_ptr<Floor>			_floor;

	int _frame;
	int _phase;
	int _dethEnemyCount;

	void (GamePlayingScene::*_updater)();

	void StageLoadUpdate();
	void GameStartUpdate();
	void NormalUpdate();
	void ClearUpdate();
	void GameOverUpdate();
	void FadeoutUpdate();

public:
	GamePlayingScene();
	~GamePlayingScene();

	void Update();
	void Draw();

	void PhaseDraw();
};

