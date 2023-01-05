#pragma once

#include <Game/GameCommon.h>
#include <Game/GameUpdateContext.h>
#include <Script/ScriptCommon.h>
#include <tge/Math/Vector.h>

class Entity
{
public:
	enum class InteractionResult
	{
		NoInteraction,
		MoveOntoTile,
		Stay,
	};

	Tga::ScriptStringId id;
	Tga::Vector2i position; // position in tile size
	Tga::Vector2i renderOffset; // offset relative to tile, in pixels
	SpriteId sprite;
	bool canPush;
	bool canStandOn;

	virtual void Update(const GameUpdateContext& context) { context; }
	virtual InteractionResult Interact(const GameUpdateContext& context, Entity& interactingEntity) { context; interactingEntity;  return InteractionResult::NoInteraction; }
};
