#pragma once

#include <vector>

struct EnemyData {
	int frame;
	int type;
	int dir;
};

class StageLoader
{
private:
	int _stageNo;

	std::vector<EnemyData> _enemyData;

public:
	StageLoader();
	~StageLoader();

	void LoadStage(int&);

	const int GetStageNo() const;
	const std::vector<EnemyData> GetEnemyData() const;
};

