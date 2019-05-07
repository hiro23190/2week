#pragma once

#include "Geometry.h"

class Floor;
class Enemy;

class Mario
{
private:
	int _img;
	int _img_Respawn;

	int _width = 16 * 2;
	int _height = 24 * 2;

	Vector2 _pos;
	Vector2 _vel;
	Vector2 _closest_point;
	Vector2 _push_point;

	bool _isTurn; // false : 左, true : 右
	bool _isAerial;
	bool _pow;

	int _action;
	int _count;

	int _life;

	//メンバ関数ポインタ
	void (Mario::*_updater)();

	// 状態（ステート）
	// ニュートラル
	void NeutralUpdate();
	// 歩く
	void WalkUpdate();
	// 滑る
	void SlideUpdate();
	// ジャンプ
	void JumpUpdate();
	// 死ぬ
	void DieUpdate();
	// リスポーン
	void RespawnUpdate();

	void Walk_L();
	void Walk_R();
	void Jump();

public:
	Mario();
	~Mario();

	void Update(Floor& f);
	void Draw();

	void OnDamage();
	const bool POW() const;
	const int GetLife() const;

	const Vector2 GetPos() const { return _pos; }
	const Vector2 GetSize() const { return Vector2(_width, _height); }
	void Reset();
};

