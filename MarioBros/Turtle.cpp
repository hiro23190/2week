#include "Turtle.h"

#include <DxLib.h>

#include "Floor.h"
#include "Game.h"

constexpr float WALK_SPEED = 0.5f;

constexpr int ANIM_FRAME			= 10;
constexpr int WALK_ACTION			= 0;
constexpr int WALK_ACTION_COUNT		= 3;
constexpr int REVERSE_ACTION		= 3;
constexpr int REVERSE_ACTION_COUNT	= 2;
constexpr int TURN_ACTION			= 5;
constexpr int TURN_ACTION_COUNT		= 2;

void Turtle::WaitUpdate()
{
	_vel = Vector2();

	if (_count > 240) {
		_count	= 0;
		_action = WALK_ACTION;

		_updater = &Turtle::EntreeUpdate;
	}
}

void Turtle::EntreeUpdate()
{
	_action = (_count / ANIM_FRAME) % WALK_ACTION_COUNT;

	_vel = Vector2();
	_vel.x = _isTurn ? WALK_SPEED : -WALK_SPEED;

	auto d = fabs(_rc.center.x - (_isTurn ? _entreePointL : _entreePointR).x);
	if (d > 32.0f) Walk();
}

void Turtle::ExitUpdate()
{
	_action = (_count / ANIM_FRAME) % WALK_ACTION_COUNT;

	_vel = Vector2();
	if (_rc.center.y < (_isTurn ? _exitPointR : _exitPointL).y) 
		_vel.x = _isTurn ? WALK_SPEED : -WALK_SPEED;
	else 
		_vel.y = -WALK_SPEED;

	if (1.0f > GetDistance(_rc.center, (_isTurn ? _exitPointR : _exitPointL))) {
		_vel	= Vector2();
		_rc.center = _isTurn ? _entreePointR : _entreePointL;
		_isTurn = !_isTurn;

		_updater = &Turtle::WaitUpdate;
	}
}

void Turtle::WalkUpdate()
{
	_action = (_count / ANIM_FRAME) % WALK_ACTION_COUNT;

	_vel.x = _isTurn ? WALK_SPEED : -WALK_SPEED;

	if (35.0f > GetDistance(_rc.center, (_isTurn ? _exitPointR : _exitPointL))) _updater = &Turtle::ExitUpdate;
}

void Turtle::TurnUpdate()
{
	_action = TURN_ACTION + (_count / ANIM_FRAME);

	if (_action > 6) {
		_isTurn = !_isTurn;
		Walk();
	}
}

void Turtle::ReverseUpdate()
{
	_action = REVERSE_ACTION + (_count / ANIM_FRAME) % REVERSE_ACTION_COUNT;

	if (_count > 600) Walk();
}

void Turtle::DieUpdate()
{
	if (_rc.center.y > Game::GetInstance().GetWindowSize().y + _rc.Width() / 2) {
		_vel = Vector2();

		_alive = false;
	}
}

void Turtle::Walk()
{
	_count	= 0;
	_action = WALK_ACTION;
	_updater = &Turtle::WalkUpdate;
}

void Turtle::Turn()
{
	_count	= 0;
	_action = TURN_ACTION;
	_updater = &Turtle::TurnUpdate;
}

void Turtle::Reverse()
{
	_count	= 0;
	_action = REVERSE_ACTION;
	_updater = &Turtle::ReverseUpdate;
}

void Turtle::Die()
{
	_count	= 0;
	_action = REVERSE_ACTION;
	_updater = &Turtle::DieUpdate;
}

Turtle::Turtle(const Vector2& p) : Enemy(p)
{
	_img = LoadGraph("img/Turtle1.png");

	_rc.center	= p;
	_rc.size	= Size(32, 32);

	_vel = Vector2(0, 0);

	_isTurn = _rc.center.x < Game::GetInstance().GetWindowSize().x / 2.0f;
	_alive	= true;

	_count	= 0;
	_action = WALK_ACTION;

	_updater = &Turtle::EntreeUpdate;
}


Turtle::~Turtle()
{
}

void Turtle::Update(Floor& f)
{
	_count++;

	(this->*_updater)();

	// 移動
	_rc.center += _vel;

	auto size = Game::GetInstance().GetWindowSize();
	if (_rc.center.x < 0 - _rc.Width() / 2.0f) _rc.center.x = size.x + _rc.Width() / 2.0f;
	if (_rc.center.x > size.x + _rc.Width() / 2.0f) _rc.center.x = 0 - _rc.Width() / 2.0f;

	if (_updater != &Turtle::DieUpdate) {
		// ブロックとの当たり判定
		for (auto& b : f.GetBlocks()) {
			if (_rc.Bottom()		> b.pos.y - b.height / 2 &&
				_rc.Top()	< b.pos.y + b.height / 2 &&
				_rc.Left()		< b.pos.x + b.width / 2 &&
				_rc.Right()		> b.pos.x - b.width / 2) {

				if (_vel.y > 0 && _rc.center.y < b.pos.y - b.height / 2) {
					_rc.center.y = b.pos.y - b.height / 2 - _rc.Height() / 2.0f;
					_vel = Vector2();
				}
			}
		}

		// 突き上げの当たり判定
		for (auto& pb : f.GetPushBlock()) {
			if (_rc.Bottom()	> pb.pos.y - pb.height / 2 &&
				_rc.Top()		< pb.pos.y + pb.height / 2 &&
				_rc.Left()		< pb.pos.x + pb.width / 2 &&
				_rc.Right()		> pb.pos.x - pb.width / 2) {

				TurnOver(pb.pos);
			}
		}

		// 地面との当たり判定
		auto& g = f.GetGround();
		if (_rc.Bottom() > g.pos.y - g.height / 2) {
			_rc.center.y = g.pos.y - g.height / 2 - _rc.Height() / 2;
			_vel = Vector2();
		}
	}

	// 重力
	_vel.y += 0.4f;
	_vel.y = min(_vel.y, 8.0f);
}

void Turtle::Draw()
{
	DrawRectRotaGraph(
		_rc.center.x, _rc.center.y,
		(_action % 5) * _rc.Height() / 2, (_action / 5) * _rc.Height() / 2,
		_rc.Width() / 2, _rc.Height() / 2,
		2, 0,
		_img, true, _isTurn, false);

#if _DEBUG
	DrawBox(_rc.Left(), _rc.Top(), _rc.Right(), _rc.Bottom(), 0xff0000, false);
	// 中心
	DrawCircle(_rc.center.x, _rc.center.y, 3, 0xff0000);
#endif
}

bool Turtle::HitCheck()
{
	if (_updater == &Turtle::ReverseUpdate) return false;
	if (_updater == &Turtle::DieUpdate)		return false;
	if (_updater == &Turtle::WaitUpdate)	return false;
	if (_updater == &Turtle::EntreeUpdate)	return false;
	if (_updater == &Turtle::ExitUpdate)	return false;

	return true;
}

bool Turtle::OnDamage(const Vector2 & p)
{
	if (_updater == &Turtle::DieUpdate)		return false;
	if (_updater == &Turtle::WaitUpdate)	return false;
	if (_updater == &Turtle::EntreeUpdate)	return false;
	if (_updater == &Turtle::ExitUpdate)	return false;

	_vel = Vector2((_rc.center.x < p.x) ? -2.0f : 2.0f, -5.0f);

	_updater = &Turtle::DieUpdate;

	return true;	
}

void Turtle::TurnOver(const Vector2& p)
{
	if (_count > 20) {
		_vel.y = -5.0f;
		_vel.x = (_rc.center.x - p.x) * 0.1f;
		if (_updater == &Turtle::WalkUpdate) { Reverse(); return; }
		if (_updater == &Turtle::ReverseUpdate) { Walk(); return; }
	}
}
