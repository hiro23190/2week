#include "GamePlayingScene.h"

#include <DxLib.h>
#include <iostream>
#include <random>

#include "Game.h"
#include "TittleScene.h"
#include "StageLoader.h"
#include "Mario.h"
#include "EnemyFactory.h"
#include "Floor.h"

constexpr int WAIT_TIME = 60;
constexpr int MAX_PHASE = 2;

//static	std::mt19937 rand_src(12345);

void GamePlayingScene::StageLoadUpdate()
{
	auto& size = Game::GetInstance().GetWindowSize();
	DrawBox(0, 0, size.x, size.y, 0x000000, true);

	_factory->Reset();
	_mario->Reset();

	_loader->LoadStage(_phase);

	_floor->SetBlock(_loader->GetStageNo());
	_dethEnemyCount = 0;

	_frame = 0;
	_updater = &GamePlayingScene::GameStartUpdate;
}

void GamePlayingScene::GameStartUpdate()
{
	PhaseDraw();
	_floor->Draw();

	if (_frame > WAIT_TIME) {
		_frame = 0;
		_updater = &GamePlayingScene::NormalUpdate;
		return;
	}

	_frame++;
}

void GamePlayingScene::NormalUpdate()
{
	auto& data = _loader->GetEnemyData();
	for (auto& d : data) {
		if (d.frame == _frame) {
			_factory->Create(d.type, d.dir);
		}
	}

	_floor->Update();
	for (auto& e : _factory->GetEnemys()) {
		e->Update(*_floor);
	}
	_mario->Update(*_floor);

	// “G‚Æ‚Ì“–‚½‚è”»’è
	for (auto& e : _factory->GetEnemys()) {
		auto rc = e->GetRect();

		if (_mario->GetPos().y + _mario->GetSize().y / 2 > rc.Top()		&&
			_mario->GetPos().y - _mario->GetSize().y / 2 < rc.Bottom()	&&
			_mario->GetPos().x - _mario->GetSize().x / 4 < rc.Right()	&&
			_mario->GetPos().x + _mario->GetSize().x / 4 > rc.Left()	) {
			if (e->HitCheck()) {
				_mario->OnDamage();
			}
			else {
				if (e->OnDamage(_mario->GetPos())) _dethEnemyCount++;
			}
		}

		for (auto& en : _factory->GetEnemys()) {
				
		}
	}

	if (_mario->POW()) {
		for (auto& e : _factory->GetEnemys()) {
			e->TurnOver(e->GetRect().center);
			Game::GetInstance().SetScreenShake(15);
		}
	}

	PhaseDraw();
	Draw();

	if (_dethEnemyCount == _loader->GetEnemyData().size()) {
		_frame = 0;
		_updater = &GamePlayingScene::ClearUpdate;
		return;
	}
	if (_mario->GetLife() < 0) {
		_frame = 0;
		_updater = &GamePlayingScene::GameOverUpdate;
		return;
	}

	_frame++;
}

void GamePlayingScene::ClearUpdate()
{
	for (auto& e : _factory->GetEnemys()) {
		e->Update(*_floor);
	}

	Draw();

	if (_frame > WAIT_TIME) {
		_frame = 0;

		if (++_phase > MAX_PHASE) {
			_updater = &GamePlayingScene::FadeoutUpdate;
			return;
		}

		_updater = &GamePlayingScene::StageLoadUpdate;
		return;
	}

	_frame++;
}

void GamePlayingScene::GameOverUpdate()
{
	auto& size = Game::GetInstance().GetWindowSize();

	_floor->Draw();
	DrawRotaGraph(size.x / 2, size.y / 2 - 45, 0.8, 0, _img_GameOver, false);

	if (_frame > 450) {
		_frame = 0;

		_updater = &GamePlayingScene::FadeoutUpdate;
		return;
	}

	_frame++;
}

void GamePlayingScene::FadeoutUpdate()
{
	_floor->Draw();

	auto& size = Game::GetInstance().GetWindowSize();

	DrawBox(0, 0, size.x, _frame * (size.y / 30.0f), 0x000000, true);

	if (_frame > 60) {
		Game::GetInstance().ChangeScene(new TittleScene());
		return;
	}

	_frame++;
}

GamePlayingScene::GamePlayingScene()
{
	_loader.reset(new StageLoader());
	_mario.reset(new Mario());
	_factory.reset(new EnemyFactory());
	_floor.reset(new Floor());

	_img_PHASE		= LoadGraph("img/PHASE.png");
	_img_P			= LoadGraph("img/P=.png");
	_img_Number		= LoadGraph("img/Number.png");
	_img_GameOver	= LoadGraph("img/GameOver.png");

	_frame	= 0;
	_phase	= 1;
	_dethEnemyCount = 0;

	_updater = &GamePlayingScene::StageLoadUpdate;
}


GamePlayingScene::~GamePlayingScene()
{
}

void GamePlayingScene::Update()
{
	(this->*_updater)();
}

void GamePlayingScene::Draw()
{
	auto& size = Game::GetInstance().GetWindowSize();

	DrawRotaGraph(32, size.y - 55,
		1.8, 0, _img_P, true);
	DrawRectRotaGraph(
		56, size.y - 55,
		(_phase % 10) * 8, 0,
		8, 8, 1.8, 0, _img_Number, false);

	for (auto& e : _factory->GetEnemys()) {
		e->Draw();
	}
	_floor->Draw();
	_mario->Draw();
}

void GamePlayingScene::PhaseDraw()
{
	auto& size = Game::GetInstance().GetWindowSize();

	// PHASE•\Ž¦
	if (_frame < WAIT_TIME) {
		DrawRotaGraph(size.x / 2 - 15, size.y / 2 + 50, 0.5, 0, _img_PHASE, false);
		DrawRectRotaGraph(
			size.x / 2 + 30, size.y / 2 + 52,
			(_phase / 10) * 8, 0,
			8, 8, 1.8, 0, _img_Number, false);
		DrawRectRotaGraph(
			size.x / 2 + 45, size.y / 2 + 52,
			(_phase % 10) * 8, 0,
			8, 8, 1.8, 0, _img_Number, false);
	}
}
