#include "GameNodes.h"

#include <Game/GameCommon.h>
#include <Game/GameLevel.h>
#include <Game/GameUpdateContext.h>
#include <Game/Player.h>

#include <Script/ScriptNodeTypeRegistry.h>
#include <Script/ScriptStringRegistry.h>
#include <Script/ScriptNodeBase.h>


class CreatePlayerNode : public Tga::ScriptNodeBase
{
	Tga::ScriptPinId myXPin;
	Tga::ScriptPinId myYPin;
	Tga::ScriptPinId myOutPinId;

public:
	void Init(const Tga::ScriptCreationContext& context) override
	{
		using namespace Tga;

		{
			ScriptPin flowPin = {};
			flowPin.dataType = ScriptLinkDataType::Flow;
			flowPin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			flowPin.node = context.GetNodeId();
			flowPin.role = ScriptPinRole::Input;

			context.FindOrCreatePin(flowPin);
		}

		{
			ScriptPin intPin = {};
			intPin.dataType = ScriptLinkDataType::Int;
			intPin.name = ScriptStringRegistry::RegisterOrGetString("X");
			intPin.node = context.GetNodeId();
			intPin.defaultValue = { LEVEL_WIDTH / 2 };
			intPin.role = ScriptPinRole::Input;

			myXPin = context.FindOrCreatePin(intPin);
		}

		{
			ScriptPin intPin = {};
			intPin.dataType = ScriptLinkDataType::Int;
			intPin.name = ScriptStringRegistry::RegisterOrGetString("Y");
			intPin.node = context.GetNodeId();
			intPin.defaultValue = { LEVEL_HEIGHT / 2 };
			intPin.role = ScriptPinRole::Input;

			myYPin = context.FindOrCreatePin(intPin);
		}

		{
			ScriptPin flowOutPin = {};
			flowOutPin.dataType = ScriptLinkDataType::Flow;
			flowOutPin.name = ScriptStringRegistry::RegisterOrGetString("");
			flowOutPin.node = context.GetNodeId();
			flowOutPin.role = ScriptPinRole::Output;

			myOutPinId = context.FindOrCreatePin(flowOutPin);
		}
	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	{
		using namespace Tga;

		int x = std::get<int>(context.ReadInputPin(myXPin).data);
		int y = std::get<int>(context.ReadInputPin(myYPin).data);

		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(context.GetUpdateContext());

		Player* p = updateContext.gameLevel.entities.CreateEntity<Player>(Tga::ScriptStringRegistry::RegisterOrGetString("Player"));
		p->position = { x, y };
		p->renderOffset = { 0, 2 };
		p->sprite = SpriteId::CharacterRed;

		context.TriggerOutputPin(myOutPinId);

		return ScriptNodeResult::Finished;
	}
};


void RegisterGameNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<CreatePlayerNode>("Game/CreatePlayer", "Creates a player at the given location");
}

