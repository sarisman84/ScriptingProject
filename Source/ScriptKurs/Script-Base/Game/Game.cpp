#include "Game.h"

#include <Game/Entity.h>
#include <Game/EntityContainer.h>
#include <Game/GameCommon.h>
#include <Game/GameLevel.h>
#include <Game/GameUpdateContext.h>

#include <Script/ScriptNodeTypeRegistry.h>
#include <Script/ScriptManager.h>
#include <Script/ScriptRuntimeInstance.h>
#include <Script/ScriptStringRegistry.h>

#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/sprite/sprite.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/text/text.h>
#include <tge/engine.h>
#include <tge/settings/settings.h>

#include <fstream>

Game::Game()
{}

Game::~Game()
{}

void Game::Init()
{
	auto& engine = *Tga::Engine::GetInstance();
	Tga::TextureManager& textureManager = engine.GetTextureManager();

	mySpriteTextures.resize((int)SpriteId::Count);
	mySpriteTextures[(int)SpriteId::Grass] = textureManager.GetTexture(L"sprites/grass.png");
	mySpriteTextures[(int)SpriteId::Wall] = textureManager.GetTexture(L"sprites/wall.png");
	mySpriteTextures[(int)SpriteId::WallLeft] = textureManager.GetTexture(L"sprites/wall_left.png");
	mySpriteTextures[(int)SpriteId::WallRight] = textureManager.GetTexture(L"sprites/wall_right.png");
	mySpriteTextures[(int)SpriteId::WallTop] = textureManager.GetTexture(L"sprites/wall_top.png");
	mySpriteTextures[(int)SpriteId::WallTopLeft] = textureManager.GetTexture(L"sprites/wall_top_left.png");
	mySpriteTextures[(int)SpriteId::WallTopRight] = textureManager.GetTexture(L"sprites/wall_top_right.png");
	mySpriteTextures[(int)SpriteId::WallSpecial] = textureManager.GetTexture(L"sprites/wall_special.png");
	mySpriteTextures[(int)SpriteId::WallSpecialLeft] = textureManager.GetTexture(L"sprites/wall_special_left.png");
	mySpriteTextures[(int)SpriteId::WallSpecialRight] = textureManager.GetTexture(L"sprites/wall_special_right.png");
	mySpriteTextures[(int)SpriteId::WallSpecialTop] = textureManager.GetTexture(L"sprites/wall_special_top.png");
	mySpriteTextures[(int)SpriteId::WallSpecialTopLeft] = textureManager.GetTexture(L"sprites/wall_special_top_left.png");
	mySpriteTextures[(int)SpriteId::WallSpecialTopRight] = textureManager.GetTexture(L"sprites/wall_special_top_right.png");
	mySpriteTextures[(int)SpriteId::PlateDown] = textureManager.GetTexture(L"sprites/plate_down.png");
	mySpriteTextures[(int)SpriteId::PlateUp] = textureManager.GetTexture(L"sprites/plate_up.png");
	mySpriteTextures[(int)SpriteId::YellowDoor] = textureManager.GetTexture(L"sprites/yellow_door.png");
	mySpriteTextures[(int)SpriteId::YellowKey] = textureManager.GetTexture(L"sprites/yellow_key.png");
	mySpriteTextures[(int)SpriteId::Coin] = textureManager.GetTexture(L"sprites/coin.png");
	mySpriteTextures[(int)SpriteId::Flag] = textureManager.GetTexture(L"sprites/flag.png");
	mySpriteTextures[(int)SpriteId::Ball] = textureManager.GetTexture(L"sprites/ball.png");
	mySpriteTextures[(int)SpriteId::CharacterRed] = textureManager.GetTexture(L"sprites/character_red.png");
	mySpriteTextures[(int)SpriteId::CharacterBlue] = textureManager.GetTexture(L"sprites/character_blue.png");

	LoadLevel("world01", false);
}

void Game::Update(const Tga::InputManager& inputManager, float aTimeDelta)
{


	GameUpdateContext context{ aTimeDelta,*this, myCurrentLevel, inputManager };

	if (myCurrentLevel.levelScript)
	{
		myCurrentLevel.levelScript->Update(context);
	}

	myCurrentLevel.entities.ForEachEntity([&](Entity& entity)
		{
			entity.Update(context);
		});

	if (inputManager.IsKeyPressed('R'))
	{
		LoadLevel("world01", true, false, true);
	}

}

void Game::Render()
{
	int pixelSize = 4;
	int spriteSize = pixelSize * 12;

	auto& engine = *Tga::Engine::GetInstance();

	Tga::Vector2ui intResolution = engine.GetRenderSize();

	int xOffset = (intResolution.x - spriteSize * LEVEL_WIDTH) / 2;
	int yOffset = (intResolution.y - spriteSize * LEVEL_HEIGHT) / 2;


	Tga::SpriteDrawer& drawer = engine.GetGraphicsEngine().GetSpriteDrawer();

	auto drawTile = [&](SpriteId type, auto condition)
	{
		Tga::SpriteSharedData sharedData;
		sharedData.myTexture = mySpriteTextures[(int)type];
		sharedData.mySamplerFilter = Tga::SamplerFilter::Point;

		Tga::SpriteBatchScope batch = drawer.BeginBatch(sharedData);
		for (int x = 0; x < LEVEL_WIDTH; x++)
		{
			for (int y = 0; y < LEVEL_HEIGHT; y++)
			{
				if (condition(Tga::Vector2i{ x,y }))
				{
					Tga::Sprite2DInstanceData instance = {};
					instance.myPosition = { (float)(xOffset + spriteSize * x),  (float)(yOffset + spriteSize * y) };
					instance.mySize = (float)spriteSize;
					batch.Draw(instance);
				}
			}
		}

	};

	drawTile(SpriteId::WallTop, [&](Tga::Vector2i pos)
		{
			return myCurrentLevel.tiles[pos.x][pos.y] == LevelTileType::Wall && myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 0, 1 }) == LevelTileType::Grass;
		});

	drawTile(SpriteId::WallLeft, [&](Tga::Vector2i pos)
		{
			if (myCurrentLevel.tiles[pos.x][pos.y] != LevelTileType::Wall)
				return false;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ -1, 0 }) == LevelTileType::Grass)
				return true;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ -1, -1 }) == LevelTileType::Grass)
				return true;

			return false;
		});

	drawTile(SpriteId::WallRight, [&](Tga::Vector2i pos)
		{
			if (myCurrentLevel.tiles[pos.x][pos.y] != LevelTileType::Wall)
				return false;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 1, 0 }) == LevelTileType::Grass)
				return true;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 1, -1 }) == LevelTileType::Grass)
				return true;

			return false;
		});

	drawTile(SpriteId::WallTopLeft, [&](Tga::Vector2i pos)
		{
			if (myCurrentLevel.tiles[pos.x][pos.y] != LevelTileType::Wall)
				return false;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ -1, 1 }) == LevelTileType::Grass)
				return true;

			return false;
		});

	drawTile(SpriteId::WallTopRight, [&](Tga::Vector2i pos)
		{
			if (myCurrentLevel.tiles[pos.x][pos.y] != LevelTileType::Wall)
				return false;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 1, 1 }) == LevelTileType::Grass)
				return true;

			return false;
		});

	drawTile(SpriteId::Wall, [&](Tga::Vector2i pos)
		{
			return myCurrentLevel.tiles[pos.x][pos.y] == LevelTileType::Wall && myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 0, -1 }) == LevelTileType::Grass;
		});

	drawTile(SpriteId::WallSpecialTop, [&](Tga::Vector2i pos)
		{
			return myCurrentLevel.tiles[pos.x][pos.y] == LevelTileType::WallSpecial && myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 0, 1 }) == LevelTileType::Grass;
		});

	drawTile(SpriteId::WallSpecialLeft, [&](Tga::Vector2i pos)
		{
			if (myCurrentLevel.tiles[pos.x][pos.y] != LevelTileType::WallSpecial)
				return false;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ -1, 0 }) == LevelTileType::Grass)
				return true;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ -1, -1 }) == LevelTileType::Grass)
				return true;

			return false;
		});

	drawTile(SpriteId::WallSpecialRight, [&](Tga::Vector2i pos)
		{
			if (myCurrentLevel.tiles[pos.x][pos.y] != LevelTileType::WallSpecial)
				return false;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 1, 0 }) == LevelTileType::Grass)
				return true;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 1, -1 }) == LevelTileType::Grass)
				return true;

			return false;
		});

	drawTile(SpriteId::WallSpecialTopLeft, [&](Tga::Vector2i pos)
		{
			if (myCurrentLevel.tiles[pos.x][pos.y] != LevelTileType::WallSpecial)
				return false;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ -1, 1 }) == LevelTileType::Grass)
				return true;

			return false;
		});

	drawTile(SpriteId::WallSpecialTopRight, [&](Tga::Vector2i pos)
		{
			if (myCurrentLevel.tiles[pos.x][pos.y] != LevelTileType::WallSpecial)
				return false;

			if (myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 1, 1 }) == LevelTileType::Grass)
				return true;

			return false;
		});

	drawTile(SpriteId::WallSpecial, [&](Tga::Vector2i pos)
		{
			return myCurrentLevel.tiles[pos.x][pos.y] == LevelTileType::WallSpecial && myCurrentLevel.GetTileType(pos + Tga::Vector2i{ 0, -1 }) == LevelTileType::Grass;
		});

	drawTile(SpriteId::Grass, [&](Tga::Vector2i pos)
		{
			return myCurrentLevel.tiles[pos.x][pos.y] == LevelTileType::Grass;
		});

	for (int i = 0; i < (int)SpriteId::Count; i++)
	{
		Tga::SpriteSharedData sharedData;
		sharedData.myTexture = mySpriteTextures[i];
		sharedData.mySamplerFilter = Tga::SamplerFilter::Point;

		SpriteId spriteId = (SpriteId)i;

		Tga::SpriteBatchScope batch = drawer.BeginBatch(sharedData);

		myCurrentLevel.entities.ForEachEntity([&](Entity& e)
			{
				if (e.sprite == spriteId)
				{
					Tga::Sprite2DInstanceData instance = {};
					instance.myPosition = { (float)(xOffset + spriteSize * e.position.x + pixelSize * e.renderOffset.x),  (float)(yOffset + spriteSize * e.position.y + pixelSize * e.renderOffset.y) };
					instance.mySize = (float)spriteSize;
					batch.Draw(instance);
				}
			});
	}

	{
		Tga::Text text(L"Text/arial.ttf", Tga::FontSize_18);
		text.SetText("Move using arrow keys. Start/Restart level by pressing 'r'. Have fun!");
		text.SetPosition(Tga::Vector2f{ 0.5f * intResolution.x - 0.5f * text.GetWidth(), 0.1f * intResolution.y });
		text.Render();
	}
}

void Game::LoadLevel(const char* name, bool runScripts, bool runSameScript, bool resetPreviousLevel)
{
	if (!resetPreviousLevel)
		myPreviousLevel = myCurrentLevel.levelNameID;
	std::string path = Tga::Settings::ResolveAssetPath("levels/" + std::string(name) + ".txt");
	static std::unique_ptr< Tga::ScriptRuntimeInstance> oldScript;
	oldScript = std::move(myCurrentLevel.levelScript);
	//myCurrentLevel.entities.ForEachEntity([this](Entity& anEntity)
	//	{
	//		myCurrentLevel.entities.DestroyEntity(anEntity.id);
	//	});
	myCurrentLevel = {};
	myCurrentLevel.levelNameID = Tga::ScriptStringRegistry::RegisterOrGetString(name);
	if (resetPreviousLevel)
		myPreviousLevel = myCurrentLevel.levelNameID;
	{
		std::ifstream file(path);
		std::string line;

		int ballCount = 0;
		int coinCount = 0;
		int flagCount = 0;

		char nameBuffer[128];

		int lineIndex = 0;
		while (std::getline(file, line) && lineIndex < LEVEL_HEIGHT)
		{
			int row = LEVEL_HEIGHT - 1 - lineIndex;
			for (int column = 0; column < LEVEL_WIDTH; column++)
			{
				LevelTileType t = LevelTileType::Grass;
				if (line[column] == '#')
				{
					t = LevelTileType::Wall;
				}
				if (line[column] == 'S')
				{
					t = LevelTileType::WallSpecial;
				}
				else if (line[column] == 'b')
				{
					sprintf_s(nameBuffer, "ball %03d", ballCount++);

					Entity* b = myCurrentLevel.entities.CreateEntity<Entity>(Tga::ScriptStringRegistry::RegisterOrGetString(nameBuffer));
					b->position = { column, row };
					b->renderOffset = { 0, 2 };
					b->sprite = SpriteId::Ball;
					b->canPush = true;
				}
				else if (line[column] == 'c')
				{
					sprintf_s(nameBuffer, "coin %03d", coinCount++);

					Entity* b = myCurrentLevel.entities.CreateEntity<Entity>(Tga::ScriptStringRegistry::RegisterOrGetString(nameBuffer));
					b->position = { column, row };
					b->renderOffset = { 0, 2 };
					b->sprite = SpriteId::Coin;
					b->canStandOn = true;
				}
				else if (line[column] == 'f')
				{
					sprintf_s(nameBuffer, "flag %03d", flagCount++);

					Entity* b = myCurrentLevel.entities.CreateEntity<Entity>(Tga::ScriptStringRegistry::RegisterOrGetString(nameBuffer));
					b->position = { column, row };
					b->renderOffset = { 0, 2 };
					b->sprite = SpriteId::Flag;
					b->canStandOn = true;
				}

				myCurrentLevel.tiles[column][row] = t;
			}
			lineIndex++;
		}
	}

	if (runScripts)
	{
		if (runSameScript && oldScript)
		{
			myCurrentLevel.levelScript = std::move(oldScript);
		}
		else
		{

			std::shared_ptr<const Tga::Script> script = Tga::ScriptManager::GetScript(name);
			if (script)
			{
				//if (oldScript)
				//	oldScript.reset(nullptr);
				//myCurrentLevel.levelScript.reset(nullptr);
				myCurrentLevel.levelScript = std::make_unique<Tga::ScriptRuntimeInstance>(script);
				myCurrentLevel.levelScript->Init();
			}
		}

		//if (!runSameScript && oldScript)
		//	oldScript.reset(nullptr);

	}

}