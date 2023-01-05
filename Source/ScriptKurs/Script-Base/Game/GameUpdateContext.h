#pragma once

#include <Script/ScriptUpdateContext.h>

namespace Tga
{
	class InputManager;
}

struct GameLevel;

/// <summary>
/// By inheriting from ScriptUpdateContext, GameUpdateContext can be used to provide additional data to nodes in this game
/// See usage in PlayerNode for example CreatePlayerNode for example
/// </summary>
struct GameUpdateContext : Tga::ScriptUpdateContext
{
	GameLevel& gameLevel;
	const Tga::InputManager& inputManager;
};
