#pragma once

#include <list>

#include "Geometry.h"

#include "Enemy.h"

enum class EnemyType {
	NON,
	Turtle,
};

class EnemyFactory
{
private:
	std::list<Enemy*> _Enemys;

public:
	EnemyFactory();
	~EnemyFactory();

	void Create(const int&, const int&);
	void Reset();

	const std::list<Enemy*> GetEnemys() const;
};

