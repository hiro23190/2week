#pragma once

#include <vector>
#include <list>

#include "Geometry.h"

struct Block {
	Vector2 pos;	// íÜêSç¿ïW
	int num;		// ÉuÉçÉbÉNêî
	int width;		// ïù
	int height;		// çÇÇ≥
	int expansion;	// ägëÂó¶

	Block() : pos(0, 0), num(1), expansion(2) { width = num * 8 * expansion; height = 8 * expansion; }
	Block(Vector2 p, int n, int e = 2) :pos(p), num(n), expansion(e) { width = num * 8 * expansion; height = 8 * expansion; }
};

enum PushPoint {
	MIDDLE,
	RIGHT,
	LEFT
};

struct PushBlock {
	Vector2 pos;
	PushPoint point;
	int count;
	int width;
	int height;
	int expansion;

	PushBlock() : pos(0, 0), point(MIDDLE), count(0), expansion(2) { width = 24 * expansion; height = 16 * expansion; }
	PushBlock(Vector2 p, PushPoint pp = MIDDLE, int e = 2) : pos(p), point(pp), count(0), expansion(e) { width = 24 * expansion; height = 16 * expansion; }
};

struct POW {
	Vector2 pos;
	int width;
	int height;
	int expansion;	// ägëÂó¶

	int pow_state;

	POW() : pos(0, 0), expansion(2), pow_state(0) { width = 16 * expansion; height = 16 * expansion; }
	POW(Vector2 p, int e = 2) : pos(p), expansion(e), pow_state(0) { width = 16 * expansion; height = 16 * expansion; }
};

struct Ground {
	Vector2 pos;
	int width;
	int height;
	int expansion;	// ägëÂó¶

	Ground() : pos(0, 0), expansion(2) { width = 256 * expansion; height = 16 * expansion; }
	Ground(Vector2 p, int e = 2) : pos(p), expansion(e) { width = 256 * expansion; height = 16 * expansion; }
};

class Floor
{
private:
	int img_block[5];
	int img_block_push[5];
	int img_ground;
	int img_pipe1;
	int img_pipe2;
	int img_pow;

	int _blockNo;

	std::vector<Block> _blocks;
	std::list<PushBlock> _pushb;
	POW _pow;
	Ground _ground;

public:
	Floor();
	~Floor();

	void SetBlock(const int&);
	void Update();
	void Draw();

	void PushUP(const Vector2& p, const PushPoint&);
	void PushPOW();

	const std::vector<Block> GetBlocks() const;
	const std::list<PushBlock> GetPushBlock() const;
	const POW GetPOW() const;
	const Ground GetGround() const;
};

