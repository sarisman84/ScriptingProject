#include "CommonNodes.h"

#include <Script/ScriptNodeTypeRegistry.h>
#include <Script/ScriptStringRegistry.h>
#include <Script/ScriptCommon.h>
#include <Script/ScriptNodeBase.h>
#include <Game/GameUpdateContext.h>
#include <Game/GameLevel.h>
#include <Game/Game.h>

#include <iostream>

using namespace Tga;

class StartNode : public ScriptNodeBase
{
	ScriptPinId myOutputPin;
	ScriptPinId myPreviousLevelPin;

public:
	void Init(const ScriptCreationContext& context) override
	{
		{
			ScriptPin outputPin = {};
			outputPin.dataType = ScriptLinkDataType::Flow;
			outputPin.name = ScriptStringRegistry::RegisterOrGetString("Start");
			outputPin.node = context.GetNodeId();
			outputPin.role = ScriptPinRole::Output;

			myOutputPin = context.FindOrCreatePin(outputPin);
		}

		{
			ScriptPin outputPin = {};
			outputPin.dataType = ScriptLinkDataType::String;
			outputPin.name = ScriptStringRegistry::RegisterOrGetString("Previous Level");
			outputPin.node = context.GetNodeId();
			outputPin.role = ScriptPinRole::Output;

			myPreviousLevelPin = context.FindOrCreatePin(outputPin);
		}

	}

	ScriptNodeResult Execute(ScriptExecutionContext& context, ScriptPinId) const override
	{
		context.TriggerOutputPin(myOutputPin);

		return ScriptNodeResult::Finished;
	}

	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(someContext.GetUpdateContext());

		auto levelID = updateContext.game.GetPreviousLevel();
		return { levelID };

	}

	bool ShouldExecuteAtStart() const override { return true; }
};
class UpdateNode : public ScriptNodeBase
{
	ScriptPinId myOutputPin;

public:
	void Init(const ScriptCreationContext& context) override
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Flow;
		outputPin.name = ScriptStringRegistry::RegisterOrGetString("On Update");
		outputPin.node = context.GetNodeId();
		outputPin.role = ScriptPinRole::Output;

		myOutputPin = context.FindOrCreatePin(outputPin);
	}

	ScriptNodeResult Execute(ScriptExecutionContext& context, ScriptPinId) const override
	{
		context.TriggerOutputPin(myOutputPin);

		return ScriptNodeResult::KeepRunning;
	}

	bool ShouldExecuteAtStart() const override { return true; }
};
class LoadLevel : public Tga::ScriptNodeBase
{
public:
	void Init(const ScriptCreationContext& context) override
	{
		{
			ScriptPin inputPin = {};
			inputPin.dataType = ScriptLinkDataType::Flow;
			inputPin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			inputPin.node = context.GetNodeId();
			inputPin.role = ScriptPinRole::Input;
			context.FindOrCreatePin(inputPin);
		}

		{
			ScriptPin namePin = {};
			namePin.dataType = ScriptLinkDataType::String;
			namePin.name = ScriptStringRegistry::RegisterOrGetString("Level");
			namePin.node = context.GetNodeId();
			namePin.role = ScriptPinRole::Input;
			namePin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("world01") };
			myLevelNamePin = context.FindOrCreatePin(namePin);
		}

	}

	ScriptNodeResult Execute(ScriptExecutionContext& context, ScriptPinId) const override
	{
		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(context.GetUpdateContext());
		auto levelName = std::get<Tga::ScriptStringId>(context.ReadInputPin(myLevelNamePin).data);
		updateContext.game.LoadLevel(Tga::ScriptStringRegistry::GetStringFromStringId(levelName).data(), true);
		return ScriptNodeResult::Finished;
	}
private:
	Tga::ScriptPinId myFlowOutputPin;
	Tga::ScriptPinId myLevelNamePin;

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
			updateContext.gameLevel.entities.ForEachEntity([sourceEntity, &result](Entity& anEntity)
				{
					if (!sourceEntity) return;
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
class StringCompareNode : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{

		using namespace Tga;
		{
			ScriptPin conditionPin = {};
			conditionPin.dataType = ScriptLinkDataType::String;
			conditionPin.name = ScriptStringRegistry::RegisterOrGetString("A");
			conditionPin.node = context.GetNodeId();
			conditionPin.role = ScriptPinRole::Input;
			conditionPin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("None") };
			myAPin = context.FindOrCreatePin(conditionPin);
		}

		{
			ScriptPin conditionPin = {};
			conditionPin.dataType = ScriptLinkDataType::String;
			conditionPin.name = ScriptStringRegistry::RegisterOrGetString("B");
			conditionPin.node = context.GetNodeId();
			conditionPin.role = ScriptPinRole::Input;
			conditionPin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("None") };
			myBPin = context.FindOrCreatePin(conditionPin);
		}

		{
			ScriptPin resultPin = {};
			resultPin.dataType = ScriptLinkDataType::Bool;
			resultPin.name = ScriptStringRegistry::RegisterOrGetString("Result");
			resultPin.node = context.GetNodeId();
			resultPin.role = ScriptPinRole::Output;
			myResultPin = context.FindOrCreatePin(resultPin);
		}



	}


	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	{
		auto dataA = std::get<Tga::ScriptStringId>(context.ReadInputPin(myAPin).data);
		auto dataB = std::get<Tga::ScriptStringId>(context.ReadInputPin(myBPin).data);
		return { dataA == dataB };

	}

private:
	Tga::ScriptPinId myAPin;
	Tga::ScriptPinId myBPin;
	Tga::ScriptPinId myResultPin;
};

template<size_t Amount>
class SequencerNode : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{

		using namespace Tga;
		{
			ScriptPin inputPin = {};
			inputPin.dataType = ScriptLinkDataType::Flow;
			inputPin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			inputPin.node = context.GetNodeId();
			inputPin.role = ScriptPinRole::Input;
			context.FindOrCreatePin(inputPin);
		}


		{
			for (size_t i = 0; i < Amount; i++)
			{
				ScriptPin outputPin = { };
				outputPin.dataType = ScriptLinkDataType::Flow;
				outputPin.name = ScriptStringRegistry::RegisterOrGetString((std::string("Flow ") + std::to_string(i)).c_str());
				outputPin.node = context.GetNodeId();
				outputPin.role = ScriptPinRole::Output;
				myFlowOutputPins[i] = context.FindOrCreatePin(outputPin);
			}
		}
	



	}


	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		for (size_t i = 0; i < Amount; i++)
		{
			someContext.TriggerOutputPin(myFlowOutputPins[i]);
		}
		return Tga::ScriptNodeResult::Finished;
	}

private:
	Tga::ScriptPinId myFlowOutputPins[Amount];

};
void Tga::RegisterCommonNodes()
{
	ScriptNodeTypeRegistry::RegisterType<StartNode>("Common/Start", "A node that executes once when the script starts");
	ScriptNodeTypeRegistry::RegisterType<UpdateNode>("Common/Update", "A node that executes constantly when the script starts");
	ScriptNodeTypeRegistry::RegisterType<LoadLevel>("Level/Load new level", "A node that loads a new level based on a target name.");
	ScriptNodeTypeRegistry::RegisterType<BranchNode>("Logic/Branch", "Creates a branching node.");
	ScriptNodeTypeRegistry::RegisterType<AndGate>("Logic/AND", "Creates an AND Gate node.");
	ScriptNodeTypeRegistry::RegisterType<StringCompareNode>("Logic/Comparison/Strings", "Compares two strings to see if they are the same, if they are, the node returns true");
	ScriptNodeTypeRegistry::RegisterType<CheckCollision>("Entity/Check Collision", "Compares two entities positions and sees if one of them is ontop of another");

	ScriptNodeTypeRegistry::RegisterType<SequencerNode<5>>("Logic/Sequencer/Set of 10", "Triggers a set of pins in a sequence.");
}
