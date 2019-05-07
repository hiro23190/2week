#include "Floor.h"

#include <DxLib.h>

#include <math.h>
#include <string>

#include "Game.h"

constexpr int StageNum = 5;

Floor::Floor()
{
	for (int i = 0; i < 5; i++) {
		auto str = "img/Block/Block" + std::to_string(i + 1) + ".png";
		img_block[i] = LoadGraph(str.c_str());
		str = "img/Block/Block" + std::to_string(i + 1) + "_Push.png";
		img_block_push[i] = LoadGraph(str.c_str());
	}
	img_ground		= LoadGraph("img/Ground.png");
	img_pipe1		= LoadGraph("img/Pipe.png");
	img_pipe2		= LoadGraph("img/Pipe2.png");
	img_pow			= LoadGraph("img/POW.png");

	_blockNo = 0;

	auto size = Game::GetInstance().GetWindowSize();

	// 14
	_blocks.emplace_back(
		Vector2(12 * 8, size.y / 3 - 8), 16);
	_blocks.emplace_back(
		Vector2(size.x - 12 * 8, size.y / 3 - 8), 16);
	// 16
	_blocks.emplace_back(
		Vector2(size.x / 2, size.y / 2 + 8), 16);
	// 4
	_blocks.emplace_back(
		Vector2(-8 * 8, size.y / 2 + 24), 16);
	_blocks.emplace_back(
		Vector2(size.x + 8 * 8, size.y / 2 + 24), 16);
	// 12
	_blocks.emplace_back(
		Vector2(8 * 8, size.y / 4 * 3 - 16), 16);
	_blocks.emplace_back(
		Vector2(size.x - 8 * 8, size.y / 4 * 3 - 16), 16);

	_pow.pos = Vector2(size.x / 2, size.y / 4 * 3 - 8);

	_ground.pos = Vector2(size.x / 2, size.y - 33);
}


Floor::~Floor()
{
}

void Floor::SetBlock(const int & in)
{
	_blockNo = min(in, StageNum - 1);

	_pow.pow_state = 0;
	_pushb.clear();
}

void Floor::Update()
{
	for (auto& l : _pushb) {
		l.count++;
	}
	_pushb.remove_if([](PushBlock l) { return l.count >= 12; }); // 値1の要素を全て削除

#if _DEBUG
	if (CheckHitKey(KEY_INPUT_1)) {
		_pow.pow_state = 0;
	}
#endif
}

void Floor::Draw()
{
	auto size = Game::GetInstance().GetWindowSize();

	// ブロック
	for (auto& b : _blocks) {
		DrawRectRotaGraph(
			b.pos.x, b.pos.y,
			0, 0,
			b.width / 2, b.height / 2,
			2, 0, img_block[_blockNo], true);
#if _DEBUG
		DrawBox(b.pos.x - b.width / 2, b.pos.y - b.height / 2,
			b.pos.x + b.width / 2, b.pos.y + b.height / 2,
			0xff0000, false);
		DrawCircle(b.pos.x, b.pos.y, 3, 0x00ff00);
#endif
	}
	// 突き上げブロック
	for (auto& l : _pushb) {
		DrawRectRotaGraph(
			l.pos.x, l.pos.y,
			l.width / l.expansion * fabs((l.count + 6) / 2 % 6 - 3) + (l.point == LEFT ? l.width / l.expansion / 3 : 0), 0,
			l.point == MIDDLE ? l.width / 2 : l.width / 3, l.height / 2,
			2, 0, img_block_push[_blockNo], true);
#if _DEBUG
		DrawBox(l.pos.x - (l.point == MIDDLE ? l.width / 2 : l.width / 3), l.pos.y - l.height / 2,
			l.pos.x + (l.point == MIDDLE ? l.width / 2 : l.width / 3), l.pos.y + l.height / 2,
			l.point == MIDDLE ? 0xff0000 : l.point == RIGHT ? 0x0000ff : 0x00ff00, false);
		DrawCircle(l.pos.x, l.pos.y, 3, 0x00ff00);
#endif
	}

	// 地面
	DrawRotaGraph(_ground.pos.x, _ground.pos.y, _ground.expansion, 0, img_ground, true);
#if _DEBUG
	DrawBox(_ground.pos.x - _ground.width / 2, _ground.pos.y - _ground.height / 2,
		_ground.pos.x + _ground.width / 2, _ground.pos.y + _ground.height / 2,
		0xff0000, false);
	DrawCircle(_ground.pos.x, _ground.pos.y, 3, 0x00ff00);
#endif

	// POW
	DrawRectRotaGraph(
		_pow.pos.x, _pow.pos.y,
		_pow.pow_state * _pow.width / 2, 0,
		_pow.width / 2, _pow.height / 2,
		_pow.expansion, 0, img_pow, true);
#if _DEBUG
	DrawBox(_pow.pos.x - _pow.width / 2, _pow.pos.y - _pow.height / 2,
		_pow.pos.x + _pow.width / 2, _pow.pos.y + _pow.height / 2,
		0xff0000, false);
	DrawCircle(_pow.pos.x, _pow.pos.y, 3, 0x00ff00);
#endif

	// 土管
	DrawRotaGraph(32, size.y / 6 * 5 - 2,
		2, 0, img_pipe1, true);
	DrawRotaGraph(size.x - 32, size.y / 6 * 5 - 2,
		2, 0, img_pipe1, true, true);
	DrawRotaGraph(48, size.y / 6 + 18,
		2, 0, img_pipe2, true);
	DrawRotaGraph(size.x - 48, size.y / 6 + 18,
		2, 0, img_pipe2, true, true);
}

void Floor::PushUP(const Vector2 & p, const PushPoint& pp)
{
	_pushb.emplace_back(p, pp);
}

void Floor::PushPOW()
{
	_pow.pow_state++;
	if (_pow.pow_state > 3) _pow.pow_state = 3;
}

const std::vector<Block> Floor::GetBlocks() const
{
	return _blocks;
}

const std::list<PushBlock> Floor::GetPushBlock() const
{
	return _pushb;
}

const POW Floor::GetPOW() const
{
	return _pow;
}

const Ground Floor::GetGround() const
{
	return _ground;
}
