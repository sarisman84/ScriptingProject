#pragma once

#include <Game/GameLevel.h>
#include <tge/input/InputManager.h>
#include <Script/ScriptCommon.h>
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

	void LoadLevel(const char* name, bool runScripts, bool runSameScript = false, bool resetPreviousLevel = false);
	inline Tga::ScriptStringId GetPreviousLevel() const noexcept { return myPreviousLevel; }
private:
	Tga::ScriptStringId myPreviousLevel;
	std::vector<Tga::Texture*> mySpriteTextures;
	GameLevel myCurrentLevel;

};