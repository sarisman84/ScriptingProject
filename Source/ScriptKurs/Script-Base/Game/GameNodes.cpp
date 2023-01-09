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

	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	{
		return { std::get <Tga::ScriptStringId>(context.ReadInputPin(myNamePin).data) };

	}

private:
	Tga::ScriptPinId myXPin;
	Tga::ScriptPinId myYPin;
	Tga::ScriptPinId myNamePin;
	Tga::ScriptPinId myOutPinId;
	Tga::ScriptPinId myPlateOutId;
};

class CreateEntity : public Tga::ScriptNodeBase
{
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
			namePin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("New Entity") };
			myNamePin = context.FindOrCreatePin(namePin);
		}


		{
			ScriptPin spritePin = {};
			spritePin.dataType = ScriptLinkDataType::Int;
			spritePin.name = ScriptStringRegistry::RegisterOrGetString("Sprite");
			spritePin.node = context.GetNodeId();
			spritePin.role = ScriptPinRole::Input;
			spritePin.defaultValue = { 0 };
			mySpritePin = context.FindOrCreatePin(spritePin);
		}

		{
			ScriptPin statePin = {};
			statePin.dataType = ScriptLinkDataType::Bool;
			statePin.name = ScriptStringRegistry::RegisterOrGetString("Walkable");
			statePin.node = context.GetNodeId();
			statePin.role = ScriptPinRole::Input;
			statePin.defaultValue = { false };
			myStatePin = context.FindOrCreatePin(statePin);
		}

		{
			ScriptPin flowOutPin = {};
			flowOutPin.dataType = ScriptLinkDataType::Flow;
			flowOutPin.name = ScriptStringRegistry::RegisterOrGetString("");
			flowOutPin.node = context.GetNodeId();
			flowOutPin.role = ScriptPinRole::Output;

			myOutPin = context.FindOrCreatePin(flowOutPin);
		}

		{
			ScriptPin entityOutPin = {};
			entityOutPin.dataType = ScriptLinkDataType::String;
			entityOutPin.name = ScriptStringRegistry::RegisterOrGetString("Result");
			entityOutPin.node = context.GetNodeId();
			entityOutPin.role = ScriptPinRole::Output;

			myEntityPin = context.FindOrCreatePin(entityOutPin);
		}


	}

	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{



		return { std::get<Tga::ScriptStringId>(someContext.ReadInputPin(myNamePin).data) };


		/*return { std::get<bool>(context.ReadInputPin(myConditionPin).data) };*/

	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		using namespace Tga;

		int x = std::get<int>(someContext.ReadInputPin(myXPin).data);
		int y = std::get<int>(someContext.ReadInputPin(myYPin).data);
		SpriteId spriteId = (SpriteId)std::get<int>(someContext.ReadInputPin(mySpritePin).data);
		bool walkable = std::get<bool>(someContext.ReadInputPin(myStatePin).data);


		Tga::ScriptStringId nameID = std::get<Tga::ScriptStringId>(someContext.ReadInputPin(myNamePin).data);

		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(someContext.GetUpdateContext());

		Entity* entity = updateContext.gameLevel.entities.CreateEntity<Entity>(nameID);
		entity->position = { x, y };
		entity->sprite = spriteId;
		entity->canStandOn = walkable;

		someContext.TriggerOutputPin(myOutPin);


		return ScriptNodeResult::Finished;
	}
private:
	Tga::ScriptPinId myXPin;
	Tga::ScriptPinId myYPin;
	Tga::ScriptPinId myNamePin;
	Tga::ScriptPinId mySpritePin;
	Tga::ScriptPinId myStatePin;
	Tga::ScriptPinId myOutPin;
	Tga::ScriptPinId myEntityPin;
};

class BranchNode : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{

		using namespace Tga;
		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Flow;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			myFlowInputPin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin conditionPin = {};
			conditionPin.dataType = ScriptLinkDataType::Bool;
			conditionPin.name = ScriptStringRegistry::RegisterOrGetString("Condition");
			conditionPin.node = context.GetNodeId();
			conditionPin.role = ScriptPinRole::Input;
			conditionPin.defaultValue = { false };
			myConditionPin = context.FindOrCreatePin(conditionPin);
		}

		{
			ScriptPin resultPin = {};
			resultPin.dataType = ScriptLinkDataType::Flow;
			resultPin.name = ScriptStringRegistry::RegisterOrGetString("True");
			resultPin.node = context.GetNodeId();
			resultPin.role = ScriptPinRole::Output;
			myTrueOutputPin = context.FindOrCreatePin(resultPin);
		}

		{
			ScriptPin resultPin = {};
			resultPin.dataType = ScriptLinkDataType::Flow;
			resultPin.name = ScriptStringRegistry::RegisterOrGetString("False");
			resultPin.node = context.GetNodeId();
			resultPin.role = ScriptPinRole::Output;
			myFalseOutputPin = context.FindOrCreatePin(resultPin);
		}



	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		auto condition = std::get<bool>(someContext.ReadInputPin(myConditionPin).data);

		if (condition)
		{
			someContext.TriggerOutputPin(myTrueOutputPin);
		}
		else
		{
			someContext.TriggerOutputPin(myFalseOutputPin);
		}
		return Tga::ScriptNodeResult::Finished;
	}

	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	{
		return { std::get<bool>(context.ReadInputPin(myConditionPin).data) };

	}

private:
	Tga::ScriptPinId myFlowInputPin;
	Tga::ScriptPinId myTrueOutputPin;
	Tga::ScriptPinId myFalseOutputPin;
	Tga::ScriptPinId myConditionPin;
};

class SetSprite : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{

		using namespace Tga;
		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Flow;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::String;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Source Tile");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("None") };
			mySourceEntityPin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Int;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Target Sprite");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { 0 };
			myTargetTilePin = context.FindOrCreatePin(sourcePin);
		}


		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Flow;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Output;
			myFlowOutputPin = context.FindOrCreatePin(sourcePin);
		}

	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		/*auto condition = std::get<bool>(someContext.ReadInputPin(myConditionPin).data);

		if (condition)
		{
			someContext.TriggerOutputPin(myTrueOutputPin);
		}
		else
		{
			someContext.TriggerOutputPin(myFalseOutputPin);
		}*/



		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(someContext.GetUpdateContext());
		auto dataForID = someContext.ReadInputPin(mySourceEntityPin);
		auto dataForSprite = someContext.ReadInputPin(myTargetTilePin);

		auto id = std::get<Tga::ScriptStringId>(dataForID.data);
		auto spriteIndex = std::get<int>(dataForSprite.data);


		auto entity = updateContext.gameLevel.entities.GetEntity(id);
		entity->sprite = (SpriteId)spriteIndex;

		someContext.TriggerOutputPin(myFlowOutputPin);

		return Tga::ScriptNodeResult::Finished;
	}

	//Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	//{
	//	/*return { std::get<bool>(context.ReadInputPin(myConditionPin).data) };*/

	//}

private:
	Tga::ScriptPinId myFlowOutputPin;
	Tga::ScriptPinId mySourceEntityPin;
	Tga::ScriptPinId myTargetTilePin;

};

class CheckCollision : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{

		using namespace Tga;
		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Flow;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			myFlowInputPin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::String;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Source");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("None") };
			mySourceEntityPin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::String;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Target");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("None") };
			myTargetEntityPin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Flow;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Output;
			myFlowOutputPin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Bool;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Result");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Output;
			sourcePin.defaultValue = { false };
			myResultPin = context.FindOrCreatePin(sourcePin);
		}




	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{

		someContext.TriggerOutputPin(myFlowOutputPin);



		return Tga::ScriptNodeResult::Finished;
	}

	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(someContext.GetUpdateContext());
		auto dataForSource = someContext.ReadInputPin(mySourceEntityPin);
		auto dataForTarget = someContext.ReadInputPin(myTargetEntityPin);

		auto sourceID = std::get<Tga::ScriptStringId>(dataForSource.data);
		auto targetID = std::get<Tga::ScriptStringId >(dataForTarget.data);

		auto sourceEntity = updateContext.gameLevel.entities.GetEntity(sourceID);
		if (targetID == Tga::ScriptStringRegistry::RegisterOrGetString("None"))
		{
			
			bool result = false;
			updateContext.gameLevel.entities.ForEachEntity([&](Entity& anEntity)
				{
					auto targetEntity = anEntity;
					if (sourceEntity->position == targetEntity.position && sourceEntity->id != targetEntity.id)
					{
						result = true;
						return;
					}
						
				});


			return { result };
		}

		auto targetEntity = updateContext.gameLevel.entities.GetEntity(targetID);


		if (!sourceEntity || !targetEntity) return { false };

		auto sourcePos = sourceEntity->position;
		auto targetPos = targetEntity->position;

		return { sourcePos == targetPos };


		/*return { std::get<bool>(context.ReadInputPin(myConditionPin).data) };*/

	}

private:
	Tga::ScriptPinId myFlowOutputPin;
	Tga::ScriptPinId myFlowInputPin;
	Tga::ScriptPinId mySourceEntityPin;
	Tga::ScriptPinId myTargetEntityPin;
	Tga::ScriptPinId myResultPin;

};

class AndGate : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{

		using namespace Tga;


		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Bool;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("A");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { false };
			myFirstInputPin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Bool;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("B");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { false };
			mySecondInputPin = context.FindOrCreatePin(sourcePin);
		}


		{
			ScriptPin outputPin = {};
			outputPin.dataType = ScriptLinkDataType::Bool;
			outputPin.name = ScriptStringRegistry::RegisterOrGetString("Result");
			outputPin.node = context.GetNodeId();
			outputPin.role = ScriptPinRole::Output;
			outputPin.defaultValue = { false };
			myResultPin = context.FindOrCreatePin(outputPin);
		}



	}


	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		auto firstCondition = std::get<bool>(someContext.ReadInputPin(myFirstInputPin).data);
		auto secondCondition = std::get<bool>(someContext.ReadInputPin(mySecondInputPin).data);

		return { firstCondition && secondCondition };

	}

private:
	Tga::ScriptPinId myFlowOutputPin;
	Tga::ScriptPinId myResultPin;
	Tga::ScriptPinId myFlowInputPin;
	Tga::ScriptPinId myFirstInputPin;
	Tga::ScriptPinId mySecondInputPin;

};

class SetState : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{

		using namespace Tga;
		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Flow;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::String;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Source Entity");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("None") };
			mySourceEntityPin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Bool;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("New State");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { false };
			myTargetStatePin = context.FindOrCreatePin(sourcePin);
		}

		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Flow;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Output;
			myFlowOutputPin = context.FindOrCreatePin(sourcePin);
		}

	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{

		auto sourceID = std::get<Tga::ScriptStringId>(someContext.ReadInputPin(mySourceEntityPin).data);
		auto state = std::get<bool>(someContext.ReadInputPin(myTargetStatePin).data);

		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(someContext.GetUpdateContext());

		auto entity = updateContext.gameLevel.entities.GetEntity(sourceID);
		entity->canStandOn = state;



		someContext.TriggerOutputPin(myFlowOutputPin);

		return Tga::ScriptNodeResult::Finished;
	}
private:
	Tga::ScriptPinId myFlowOutputPin;
	Tga::ScriptPinId mySourceEntityPin;
	Tga::ScriptPinId myTargetStatePin;

};

void RegisterGameNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<CreatePlayerNode>("Game/Create Player", "Creates a player at the given location");
	Tga::ScriptNodeTypeRegistry::RegisterType<CreatePushPlateNode>("Game/Create PushPlate", "Creates a pressure plate at the given location");
	Tga::ScriptNodeTypeRegistry::RegisterType<CreateEntity>("Game/Create Entity", "Creates a new empty entity with a target sprite and position");

	Tga::ScriptNodeTypeRegistry::RegisterType<SetState>("Entity/Set Interaction State", "Sets a new interaction state against the player to a target entity");
	Tga::ScriptNodeTypeRegistry::RegisterType<SetSprite>("Entity/Set Sprite", "Swaps a sprite in a tile");
	Tga::ScriptNodeTypeRegistry::RegisterType<CheckCollision>("Entity/Check Collision", "Compares two entities positions and sees if one of them is ontop of another");
	//Tga::ScriptNodeTypeRegistry::RegisterType<InteractionNode>("Logic/CheckPressurePlate", "Checks whenever or not a source pressure plate is being pressed by a target player");
	Tga::ScriptNodeTypeRegistry::RegisterType<BranchNode>("Logic/Branch", "Creates a branching node.");
	Tga::ScriptNodeTypeRegistry::RegisterType<AndGate>("Logic/AND", "Creates an AND Gate node.");

}


