#include "Mario.h"

#include <DxLib.h>

#include <iostream>

#include "Game.h"
#include "Floor.h"
#include "Enemy.h"
#include "Turtle.h"

constexpr float START_POS_X	= 120;
constexpr float START_POS_Y	= 420;

constexpr float WALK_SPEED	= 0.3f;
constexpr float MAX_SPEED	= 3.0f;
constexpr float JUMP_SPEED	= -9.0f;

constexpr int MAX_LIFE		= 3;

constexpr int ANIM_FRAME	= 2;
constexpr int NEUTRAL		= 3;
constexpr int WALK			= 0;
constexpr int WALK_COUNT	= 2;
constexpr int JUMP			= 4;
constexpr int SLIDE			= 2;
constexpr int BRAKE			= 6;
constexpr int BRAKE_COUNT	= 2;
constexpr int DAMAGE		= 7;
constexpr int FALL			= 8;

void Mario::NeutralUpdate()
{
	_action = NEUTRAL;

	if (CheckHitKey(KEY_INPUT_LEFT)) {
		_count	= 0;
		_action = WALK;
		_isTurn = false;

		Walk_L();

		_updater = &Mario::WalkUpdate;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		_count	= 0;
		_action = WALK;
		_isTurn = true;

		Walk_R();

		_updater = &Mario::WalkUpdate;
	}
	if (CheckHitKey(KEY_INPUT_SPACE) && !_isAerial) Jump();
}

void Mario::WalkUpdate()
{
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		Walk_L();

		if (_vel.x < 0) {
			_count++;
			_action = (_count / ANIM_FRAME) % WALK_COUNT;
			_isTurn = false;
		}
		else _action = BRAKE;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT)) {
		Walk_R();

		if (_vel.x > 0) {
			_count++;
			_action = (_count / ANIM_FRAME) % WALK_COUNT;
			_isTurn = true;
		}
		else _action = BRAKE;
	}
	else _updater = &Mario::SlideUpdate;

	if (CheckHitKey(KEY_INPUT_SPACE) && !_isAerial) Jump();
}

void Mario::SlideUpdate()
{
	_action = SLIDE;

	if (_vel.x > 0) _vel.x = max(_vel.x - 0.2f, 0.0f);
	if (_vel.x < 0) _vel.x = min(_vel.x + 0.2f, 0.0f);

	if (CheckHitKey(KEY_INPUT_LEFT)) {
		Walk_L();

		if (_vel.x < 0) {
			_action		= WALK;
			_count		= 0;
			_isTurn		= false;
			_updater	= &Mario::WalkUpdate;
		}
		else _action = BRAKE;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		Walk_R();

		if (_vel.x > 0) {
			_action		= 0;
			_count		= 0;
			_isTurn		= true;
			_updater	= &Mario::WalkUpdate;
		}
		else _action = BRAKE;
	}
	if (CheckHitKey(KEY_INPUT_SPACE) && !_isAerial) Jump();

	if (_vel.x == 0) _updater = &Mario::NeutralUpdate;
}

void Mario::JumpUpdate()
{
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		Walk_L();
		_isTurn = false;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		Walk_R();
		_isTurn = true;
	}

	if (!_isAerial) {
		_action		= NEUTRAL;
		_updater	= &Mario::SlideUpdate;
	}
}

void Mario::DieUpdate()
{
	_action = _vel.y > 0 ? FALL : DAMAGE;

	auto& size = Game::GetInstance().GetWindowSize();

	if (_pos.y > size.y + _width / 2.0f) {
		_life--;

		if (_life >= 0) {
			_action = NEUTRAL;
			_count = 0;

			_pos = Vector2(size.x / 2.0f, 0.0f);
			_vel = Vector2();

			_updater = &Mario::RespawnUpdate;
		}
	}
}

void Mario::RespawnUpdate()
{
	_isAerial = false;

	if (_pos.y < 50.0f) _vel.y = 0.4f;
	else {
		_vel.y = 0.0f;
		_count++;
	}

	if (CheckHitKey(KEY_INPUT_LEFT)) {
		_count	= 0;
		_action = WALK;
		_isTurn = false;

		Walk_L();

		_updater = &Mario::WalkUpdate;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		_count	= 0;
		_action = WALK;
		_isTurn = true;

		Walk_R();

		_updater = &Mario::WalkUpdate;
	}
	if (CheckHitKey(KEY_INPUT_SPACE) && !_isAerial) Jump();

	if (_count > 180.0f) _updater = &Mario::NeutralUpdate;
}

void Mario::Walk_L()
{
	_vel.x -= WALK_SPEED;
	_vel.x = _vel.x < (-MAX_SPEED) ? (-MAX_SPEED) : _vel.x;
}

void Mario::Walk_R()
{
	_vel.x += WALK_SPEED;
	_vel.x = _vel.x > MAX_SPEED ? MAX_SPEED : _vel.x;
}

void Mario::Jump()
{
	_vel.y		= JUMP_SPEED;
	_action		= JUMP;
	_isAerial	= true;

	_updater	= &Mario::JumpUpdate;
}

Mario::Mario() : _pos(START_POS_X, START_POS_Y), _vel(0, 0), _action(NEUTRAL), _isTurn(true), _life(MAX_LIFE)
{
	_img			= LoadGraph("img/Mario.png");
	_img_Respawn	= LoadGraph("img/RespawnBlock.png");

	_updater = &Mario::NeutralUpdate;
}


Mario::~Mario()
{
}

void Mario::Update(Floor& f)
{
	(this->*_updater)();

	// 移動
	_pos += _vel;

	auto size = Game::GetInstance().GetWindowSize();
	if (_pos.x < 0 - _width / 4.0f) _pos.x = size.x + _width / 4.0f;
	if (_pos.x > size.x + _width / 4.0f) _pos.x = 0 - _width / 4.0f;

	_pow		= false;
	_isAerial	= true;

	if (_updater != &Mario::DieUpdate) {
		// ブロックとの当たり判定
		for (auto& b : f.GetBlocks()) {
			if (_pos.y + _height / 2 > b.pos.y - b.height / 2 &&
				_pos.y - _height / 2 < b.pos.y + b.height / 2 &&
				_pos.x - _width / 4 < b.pos.x + b.width / 2 &&
				_pos.x + _width / 4 > b.pos.x - b.width / 2) {

				// 最近接点
				_closest_point = _pos;
				if (_pos.x > b.pos.x + b.width / 2) _closest_point.x = b.pos.x + b.width / 2;
				if (_pos.x < b.pos.x - b.width / 2) _closest_point.x = b.pos.x - b.width / 2;
				if (_pos.y > b.pos.y + b.height / 2) _closest_point.y = b.pos.y + b.height / 2;
				if (_pos.y < b.pos.y - b.height / 2) _closest_point.y = b.pos.y - b.height / 2;

				if (_vel.y > 0 && _pos.y < b.pos.y - b.height / 2) {
					_pos.y = b.pos.y - b.height / 2 - _height / 2;
					_vel.y = 0;
					_isAerial = false;
				}
				if (_vel.y < 0 && _pos.y > b.pos.y + b.height / 2) {
					_pos.y = b.pos.y + _height / 2;
					_vel.y = 0;

					PushPoint pp = MIDDLE;
					_push_point = _closest_point;

					if (_closest_point.x > b.pos.x) {
						for (int i = b.num - 1; i >= 0; i--) {
							if (_closest_point.x >= (b.pos.x + b.width / 2) - (b.num - i) * (b.width / b.num)) {
								pp = i == b.num - 1 ? RIGHT : MIDDLE;
								_push_point.y -= b.height;
								_push_point.x = (b.pos.x + b.width / 2) - (b.num - i) * (b.width / b.num) + (pp == MIDDLE ? (float)(b.width / b.num) / 2.0f : 0);
								break;
							}
						}
					}
					else {
						for (int i = 1; i <= b.num; i++) {
							if (_closest_point.x <= (b.pos.x - b.width / 2) + i * (b.width / b.num)) {
								pp = i == 1 ? LEFT : MIDDLE;
								_push_point.y -= b.height;
								_push_point.x = (b.pos.x - b.width / 2.0f) + i * (b.width / b.num) - (pp == MIDDLE ? (float)(b.width / b.num) / 2.0f : 0);
								break;
							}
						}
					}
					f.PushUP(_push_point, pp);
				}
			}
		}

		// POWとの当たり判定
		auto& pow = f.GetPOW();
		if (pow.pow_state < 3) {
			if (_pos.y + _height / 2 > pow.pos.y - pow.height / 2 &&
				_pos.y - _height / 2 < pow.pos.y + pow.height / 2 &&
				_pos.x - _width / 4 < pow.pos.x + pow.width / 2 &&
				_pos.x + _width / 4 > pow.pos.x - pow.width / 2) {

				if (_vel.y > 0 && _pos.y < pow.pos.y - pow.height / 2) {
					_pos.y = pow.pos.y - pow.height / 2 - _height / 2;
					_vel.y = 0;
					_isAerial = false;
				}
				if (_vel.y < 0 && _pos.y > pow.pos.y + pow.height / 2) {
					_pos.y = pow.pos.y + _height / 2;
					_vel.y = 0;
					_pow = true;
					f.PushPOW();
				}
			}
		}

		// 地面との当たり判定
		auto& g = f.GetGround();
		if (_pos.y + _height / 2 > g.pos.y - g.height / 2) {
			_pos.y = g.pos.y - g.height / 2 - _height / 2;
			_vel.y = 0;
			_isAerial = false;
		}
	}

	// 重力
	_vel.y = min(_vel.y + 0.4f, 8.0f);

	//std::cout << _life << std::endl;
}

void Mario::Draw()
{
	DrawRectRotaGraph(
		_pos.x, _pos.y,
		(_action % 5) * _width / 2, (_action / 5) * _height / 2,
		_width / 2, _height / 2, 2, 0, _img, true, _isTurn);
	if (_updater == &Mario::RespawnUpdate)
		DrawRectRotaGraph(
			_pos.x, _pos.y + _height / 2.0f + 8, 
			(_count / 60) * 16, 0, 16, 8, 2, 0, _img_Respawn, true);

#if _DEBUG
	// マリオの矩形
	DrawBox(_pos.x - _width / 2 / 2, _pos.y - _height / 2,
		_pos.x + _width / 2 / 2, _pos.y + _height / 2,
		0xff0000, false);
	// マリオの中心
	DrawCircle(_pos.x, _pos.y, 3, 0xff0000);
	// 最接近点
	DrawCircle(_closest_point.x, _closest_point.y, 3, 0x0000ff);
	// 突き上げブロックの中心
	DrawCircle(_push_point.x, _push_point.y, 3, 0x0000ff);
#endif
}

void Mario::OnDamage()
{
	if (_updater == &Mario::DieUpdate) return;

	_count	= 0;
	_action = DAMAGE;

	_vel = Vector2(0, -5.0f);

	_updater = &Mario::DieUpdate;
}

const bool Mario::POW() const
{
	return _pow;
}

const int Mario::GetLife() const
{
	return _life;
}

void Mario::Reset()
{
	_pos = Vector2(START_POS_X, START_POS_Y);
	_vel = Vector2();

	_action = NEUTRAL;
	_isTurn = true;
}
