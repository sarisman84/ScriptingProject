#pragma once


constexpr int LEVEL_WIDTH = 16;
constexpr int LEVEL_HEIGHT = 9;

enum class SpriteId
{
	Grass,					//0
	Wall,					//1
	WallLeft,				//2
	WallRight,				//3
	WallTop,				//4
	WallTopLeft,			//5
	WallTopRight,			//6
	WallSpecial,			//7
	WallSpecialLeft,		//8
	WallSpecialRight,		//9
	WallSpecialTop,			//10
	WallSpecialTopLeft,		//11
	WallSpecialTopRight,	//12
	PlateDown,				//13
	PlateUp,				//14
	YellowDoor,				//15
	YellowKey,				//16
	Coin,					//17
	Flag,					//18
	Ball,					//19
	CharacterRed,			//20
	CharacterBlue,			//21
	Count
};

enum class LevelTileType
{
	Grass,
	Wall,
	WallSpecial
};
