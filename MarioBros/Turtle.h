#pragma once

#include "Enemy.h"
#include "Geometry.h"

class EnemyFactory;

class Turtle :
	public Enemy
{
private:
	int _img;

	int _action;
	int _count;

	// メンバ関数ポインタ
	void (Turtle::*_updater)();

	// 状態（ステート）
	// 待機
	void WaitUpdate();
	// 出現
	void EntreeUpdate();
	// 退場
	void ExitUpdate();
	// 歩く
	void WalkUpdate();
	// 方向転換
	void TurnUpdate();
	// 転倒
	void ReverseUpdate();
	// 死亡
	void DieUpdate();

	void Walk();
	void Turn();
	void Reverse();
	void Die();

public:
	Turtle(const Vector2&);
	~Turtle();

	void Update(Floor& f);
	void Draw();

	bool HitCheck();
	bool OnDamage(const Vector2&);
	void TurnOver(const Vector2&);

	const Rect GetRect() const { return _rc; }
	const bool isAlive() const { return _alive; }
};

