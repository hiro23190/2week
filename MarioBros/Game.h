#pragma once

#include <memory>

#include "Geometry.h"

class Scene;

class Game
{
private:
	Game();
	~Game();

	int _screen;
	Vector2 _pos;
	int _shakePower;

	///�V�[���Ǘ�
	std::shared_ptr<Scene> _scene;

public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	static Game& GetInstance() {
		static Game inst;
		return inst;
	}

	void Init();
	void Run();
	void End();

	///�V�[���̕ύX
	void ChangeScene(Scene* scene);

	const Vector2 GetWindowSize() const;

	void SetScreenShake(const int&);
};

