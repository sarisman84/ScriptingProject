#pragma once

#include <Game/GameLevel.h>
#include <tge/input/InputManager.h>
#include <vector>

namespace Tga
{
	class Texture;
}

class Game
{
public:
	Game();
	~Game();

	void Init();
	void Update(const Tga::InputManager& inputmanager, float aTimeDelta);
	void Render();

	void LoadLevel(const char* name, bool runScripts);

private:
	std::vector<Tga::Texture*> mySpriteTextures;
	GameLevel myCurrentLevel;

};