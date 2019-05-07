#pragma once

#include "Geometry.h"
#include "Game.h"

class Floor;

const Vector2 _entreePointL = Vector2(80, 88);
const Vector2 _entreePointR = Vector2(Game::GetInstance().GetWindowSize().x - 80, 88);
const Vector2 _exitPointL	= Vector2(48, Game::GetInstance().GetWindowSize().y - 80);
const Vector2 _exitPointR	= Vector2(Game::GetInstance().GetWindowSize().x - 48, Game::GetInstance().GetWindowSize().y - 80);

class Enemy
{
protected:
	Rect _rc;

	Vector2 _vel;

	bool _isTurn;
	bool _alive;

public:
	Enemy(const Vector2&);
	~Enemy();

	virtual void Update(Floor& f)	= 0;
	virtual void Draw()				= 0;

	virtual bool HitCheck()					= 0;
	virtual bool OnDamage(const Vector2&)	= 0;
	virtual void TurnOver(const Vector2&)	= 0;

	virtual const Rect GetRect() const		= 0;
	virtual const bool isAlive() const		= 0;
};

