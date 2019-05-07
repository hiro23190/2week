#include "EnemyFactory.h"

#include "Game.h"

#include "Turtle.h"

EnemyFactory::EnemyFactory()
{
}


EnemyFactory::~EnemyFactory()
{
}

void EnemyFactory::Create(const int& type, const int& dir)
{
	Vector2 pos = dir ? _entreePointL : _entreePointR;

	switch ((EnemyType)type)
	{
	case EnemyType::Turtle:
		_Enemys.push_back(new Turtle(pos));
			break;
	default:
		break;
	}
}

void EnemyFactory::Reset()
{
	_Enemys.clear();
}

const std::list<Enemy*> EnemyFactory::GetEnemys() const
{
	return _Enemys;
}
