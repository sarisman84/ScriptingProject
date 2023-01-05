#include "GameNodes.h"

#include <Game/GameCommon.h>
#include <Game/GameLevel.h>
#include <Game/GameUpdateContext.h>
#include <Game/Player.h>
#include <Game/PressurePlate.h>

#include <Script/ScriptNodeTypeRegistry.h>
#include <Script/ScriptStringRegistry.h>
#include <Script/ScriptNodeBase.h>


class CreatePlayerNode : public Tga::ScriptNodeBase
{
	Tga::ScriptPinId myXPin;
	Tga::ScriptPinId myYPin;
	Tga::ScriptPinId myOutPinId;

	Tga::ScriptPinId myInputPinId;
	Tga::ScriptPinId myEntityIDPinId;

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
			ScriptPin stringPin = {};
			stringPin.dataType = ScriptLinkDataType::String;
			stringPin.name = ScriptStringRegistry::RegisterOrGetString("Name");
			stringPin.node = context.GetNodeId();
			stringPin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("Player") };
			stringPin.role = ScriptPinRole::Input;

			myInputPinId = context.FindOrCreatePin(stringPin);
		}


		{
			ScriptPin flowOutPin = {};
			flowOutPin.dataType = ScriptLinkDataType::Flow;
			flowOutPin.name = ScriptStringRegistry::RegisterOrGetString("");
			flowOutPin.node = context.GetNodeId();
			flowOutPin.role = ScriptPinRole::Output;

			myOutPinId = context.FindOrCreatePin(flowOutPin);
		}


		{
			ScriptPin resultPin = {};
			resultPin.dataType = ScriptLinkDataType::String;
			resultPin.name = ScriptStringRegistry::RegisterOrGetString("Result");
			resultPin.node = context.GetNodeId();
			resultPin.role = ScriptPinRole::Output;
			resultPin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("Player") };

			myEntityIDPinId = context.FindOrCreatePin(resultPin);
		}
	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	{
		using namespace Tga;

		int x = std::get<int>(context.ReadInputPin(myXPin).data);
		int y = std::get<int>(context.ReadInputPin(myYPin).data);

		auto id = std::get<ScriptStringId>(context.ReadInputPin(myInputPinId).data);


		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(context.GetUpdateContext());

		Player* p = updateContext.gameLevel.entities.CreateEntity<Player>(id);
		p->position = { x, y };
		p->renderOffset = { 0, 2 };
		p->sprite = SpriteId::CharacterRed;

		context.TriggerOutputPin(myOutPinId);

		return ScriptNodeResult::Finished;
	}
	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	{
		return { std::get<Tga::ScriptStringId>(context.ReadInputPin(myInputPinId).data) };

	}
};

class CreatePushPlateNode : public Tga::ScriptNodeBase
{
public:
	CreatePushPlateNode() : ScriptNodeBase() {};
	// Inherited via ScriptNodeBase
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
			ScriptPin spawnPosXPin = {};
			spawnPosXPin.dataType = ScriptLinkDataType::Int;
			spawnPosXPin.name = ScriptStringRegistry::RegisterOrGetString("X");
			spawnPosXPin.node = context.GetNodeId();
			spawnPosXPin.role = ScriptPinRole::Input;
			spawnPosXPin.defaultValue = { LEVEL_WIDTH / 2 };
			myXPin = context.FindOrCreatePin(spawnPosXPin);
		}


		{
			ScriptPin spawnPosYPin = {};
			spawnPosYPin.dataType = ScriptLinkDataType::Int;
			spawnPosYPin.name = ScriptStringRegistry::RegisterOrGetString("Y");
			spawnPosYPin.node = context.GetNodeId();
			spawnPosYPin.role = ScriptPinRole::Input;
			spawnPosYPin.defaultValue = { LEVEL_HEIGHT / 2 };
			myYPin = context.FindOrCreatePin(spawnPosYPin);
		}


		{
			ScriptPin namePin = {};
			namePin.dataType = ScriptLinkDataType::String;
			namePin.name = ScriptStringRegistry::RegisterOrGetString("Name");
			namePin.node = context.GetNodeId();
			namePin.role = ScriptPinRole::Input;
			namePin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("PressurePlate") };
			myNamePin = context.FindOrCreatePin(namePin);
		}




		{
			ScriptPin flowOutPin = {};
			flowOutPin.dataType = ScriptLinkDataType::Flow;
			flowOutPin.name = ScriptStringRegistry::RegisterOrGetString("");
			flowOutPin.node = context.GetNodeId();
			flowOutPin.role = ScriptPinRole::Output;

			myOutPinId = context.FindOrCreatePin(flowOutPin);
		}

		{
			ScriptPin plateOutPin = {};
			plateOutPin.dataType = ScriptLinkDataType::String;
			plateOutPin.name = ScriptStringRegistry::RegisterOrGetString("Result");
			plateOutPin.node = context.GetNodeId();
			plateOutPin.role = ScriptPinRole::Output;

			myPlateOutId = context.FindOrCreatePin(plateOutPin);
		}


	}
	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		using namespace Tga;

		int x = std::get<int>(someContext.ReadInputPin(myXPin).data);
		int y = std::get<int>(someContext.ReadInputPin(myYPin).data);
		Tga::ScriptStringId nameID = std::get<Tga::ScriptStringId>(someContext.ReadInputPin(myNamePin).data);

		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(someContext.GetUpdateContext());

		PressurePlate* plate = updateContext.gameLevel.entities.CreateEntity<PressurePlate>(nameID);
		plate->position = { x, y };
		plate->renderOffset = { 0,-1 };
		plate->sprite = SpriteId::PlateUp;

		someContext.TriggerOutputPin(myOutPinId);


		return ScriptNodeResult::Finished;
	}
private:
	Tga::ScriptPinId myXPin;
	Tga::ScriptPinId myYPin;
	Tga::ScriptPinId myNamePin;
	Tga::ScriptPinId myOutPinId;
	Tga::ScriptPinId myPlateOutId;
};

class InteractionNode : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{
		using namespace Tga;

		{
			ScriptPin outputPin = {};
			outputPin.dataType = ScriptLinkDataType::Flow;
			outputPin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			outputPin.node = context.GetNodeId();
			outputPin.role = ScriptPinRole::Input;

			context.FindOrCreatePin(outputPin);
		}

		{
			ScriptPin outputPin = {};
			outputPin.dataType = ScriptLinkDataType::Flow;
			outputPin.name = ScriptStringRegistry::RegisterOrGetString("If interacted");
			outputPin.node = context.GetNodeId();
			outputPin.role = ScriptPinRole::Output;

			mySetTruePinId = context.FindOrCreatePin(outputPin);
		}

		{
			ScriptPin outputPin = {};
			outputPin.dataType = ScriptLinkDataType::Flow;
			outputPin.name = ScriptStringRegistry::RegisterOrGetString("Else");
			outputPin.node = context.GetNodeId();
			outputPin.role = ScriptPinRole::Output;

			mySetFalsePinId = context.FindOrCreatePin(outputPin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::String;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Source");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("PressurePlate") };
			mySourcePinId = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin targetPin = {};
			targetPin.dataType = ScriptLinkDataType::String;
			targetPin.name = ScriptStringRegistry::RegisterOrGetString("Target");
			targetPin.node = context.GetNodeId();
			targetPin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("Player") };
			targetPin.role = ScriptPinRole::Input;

			myTargetPinId = context.FindOrCreatePin(targetPin);
		}
	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		auto sourceID = std::get<Tga::ScriptStringId>(someContext.ReadInputPin(mySourcePinId).data);
		auto targetID = std::get<Tga::ScriptStringId>(someContext.ReadInputPin(myTargetPinId).data);
		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(someContext.GetUpdateContext());

		PressurePlate* source = dynamic_cast<PressurePlate*>(updateContext.gameLevel.entities.GetEntity(sourceID));
		Player* target = dynamic_cast<Player*>(updateContext.gameLevel.entities.GetEntity(targetID));

		if (!source || !target) return Tga::ScriptNodeResult::Finished;

		if (source->IsBeingPressed(target))
			someContext.TriggerOutputPin(mySetTruePinId);
		else
			someContext.TriggerOutputPin(mySetFalsePinId);

		return Tga::ScriptNodeResult::Finished;
	}
private:

	Tga::ScriptPinId myTargetPinId;
	Tga::ScriptPinId mySourcePinId;

	Tga::ScriptPinId mySetTruePinId;
	Tga::ScriptPinId mySetFalsePinId;
};

void RegisterGameNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<CreatePlayerNode>("Game/CreatePlayer", "Creates a player at the given location");
	Tga::ScriptNodeTypeRegistry::RegisterType<CreatePushPlateNode>("Game/CreatePushPlate", "Creates a pressure plate at the given location");

	Tga::ScriptNodeTypeRegistry::RegisterType<InteractionNode>("Logic/CheckPressurePlate", "Checks whenever or not a source pressure plate is being pressed by a target player");
}


