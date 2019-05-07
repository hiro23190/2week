#include "StageLoader.h"

#include <DxLib.h>

#include <string>
#include <cassert>

StageLoader::StageLoader()
{
}


StageLoader::~StageLoader()
{
}

void StageLoader::LoadStage(int & no)
{
	auto path = "Stage/Stage" + std::to_string(no) + ".stg";

	int h = DxLib::FileRead_open(path.c_str(), false);

	_stageNo = 0;
	DxLib::FileRead_read(&_stageNo, sizeof(_stageNo), h);
	assert(_stageNo < 5);

	int enemy_count = 0;
	DxLib::FileRead_read(&enemy_count, sizeof(enemy_count), h);
	assert(enemy_count >= 0);

	_enemyData.resize(enemy_count);
	for (int i = 0; i < enemy_count; i++) {
		DxLib::FileRead_read(
			&_enemyData[i].frame, sizeof(_enemyData[i].frame), h);
		DxLib::FileRead_read(
			&_enemyData[i].type, sizeof(_enemyData[i].type), h);
		DxLib::FileRead_read(
			&_enemyData[i].dir, sizeof(_enemyData[i].dir), h);
	}

	DxLib::FileRead_close(h);
}

const int StageLoader::GetStageNo() const
{
	return _stageNo;
}

const std::vector<EnemyData> StageLoader::GetEnemyData() const
{
	return _enemyData;
}
