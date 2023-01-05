#include "Player.h"

#include <Game/GameLevel.h>
#include <Game/GameUpdateContext.h>
#include <tge/input/InputManager.h>

void Player::Update(const GameUpdateContext& context)
{
	Tga::Vector2i delta = { 0,0 };

	if (context.inputManager.IsKeyPressed(VK_UP))
		delta += {0, 1};

	if (context.inputManager.IsKeyPressed(VK_DOWN))
		delta += {0, -1};

	if (context.inputManager.IsKeyPressed(VK_LEFT))
		delta += {-1, 0};

	if (context.inputManager.IsKeyPressed(VK_RIGHT))
		delta += {1, 0};

	if (delta == Tga::Vector2i{ 0, 0 })
		return;

	Tga::Vector2i newPosition = position + delta;

	if (context.gameLevel.GetTileType(newPosition) != LevelTileType::Grass)
		return;

	Entity* pushable = nullptr;

	{
		bool failed = false;
		context.gameLevel.entities.ForEachEntityAtPosition(newPosition, [&](Entity& e)
			{
				InteractionResult r = e.Interact(context, *this);
				if (r != InteractionResult::NoInteraction)
				{
					if (r == InteractionResult::Stay)
					{
						failed = true;
					}
					return;
				}

				if (e.canStandOn)
					return;

				if (e.canPush && pushable == nullptr)
				{
					pushable = &e;
					return;
				}

				failed = true;
			});

		if (failed)
			return;
	}

	if (pushable)
	{
		Tga::Vector2i newPushablePosition = newPosition + delta;

		if (context.gameLevel.GetTileType(newPushablePosition) != LevelTileType::Grass)
			return;

		bool failed = false;
		context.gameLevel.entities.ForEachEntityAtPosition(newPushablePosition, [&](Entity& e)
			{
				if (e.canStandOn)
					return;

				failed = true;
			});

		if (failed)
			return;

		pushable->position = newPushablePosition;
	}

	position = newPosition;

}