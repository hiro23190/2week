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

	// �����o�֐��|�C���^
	void (Turtle::*_updater)();

	// ��ԁi�X�e�[�g�j
	// �ҋ@
	void WaitUpdate();
	// �o��
	void EntreeUpdate();
	// �ޏ�
	void ExitUpdate();
	// ����
	void WalkUpdate();
	// �����]��
	void TurnUpdate();
	// �]�|
	void ReverseUpdate();
	// ���S
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

