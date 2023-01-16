#pragma once

#include <Game/GameCommon.h>
#include <Game/EntityContainer.h>
#include <Script/ScriptRuntimeInstance.h>
#include <tge/math/Vector.h>
#include <memory>

#include <Script/ScriptCommon.h>
struct GameLevel
{

	Tga::ScriptStringId levelNameID;
	LevelTileType tiles[LEVEL_WIDTH][LEVEL_HEIGHT];
	EntityContainer entities;
	std::unique_ptr<Tga::ScriptRuntimeInstance> levelScript;

	/// <summary>
	/// Reads from tiles, but returns LevelTileType::Wall if asking for coordinates outside the level
	/// </summary>
	LevelTileType GetTileType(Tga::Vector2i pos)
	{
		if (pos.x < 0 || pos.x >= LEVEL_WIDTH || pos.y < 0 || pos.y >= LEVEL_HEIGHT)
			return LevelTileType::Wall;

		return tiles[pos.x][pos.y];
	}
};

