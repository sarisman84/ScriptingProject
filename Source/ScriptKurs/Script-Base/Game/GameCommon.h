#pragma once


constexpr int LEVEL_WIDTH = 16;
constexpr int LEVEL_HEIGHT = 9;

enum class SpriteId
{
	Grass,
	Wall,
	WallLeft,
	WallRight,
	WallTop,
	WallTopLeft,
	WallTopRight,
	WallSpecial,
	WallSpecialLeft,
	WallSpecialRight,
	WallSpecialTop,
	WallSpecialTopLeft,
	WallSpecialTopRight,
	PlateDown,
	PlateUp,
	YellowDoor,
	YellowKey,
	Coin,
	Flag,
	Ball,
	CharacterRed,
	CharacterBlue,
	Count
};

enum class LevelTileType
{
	Grass,
	Wall,
	WallSpecial
};
