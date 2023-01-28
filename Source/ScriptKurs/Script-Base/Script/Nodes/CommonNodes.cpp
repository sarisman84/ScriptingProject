#include "CommonNodes.h"

#include <Script/ScriptNodeTypeRegistry.h>
#include <Script/ScriptStringRegistry.h>
#include <Script/ScriptCommon.h>
#include <Script/ScriptNodeBase.h>
#include <Game/GameUpdateContext.h>
#include <Game/GameLevel.h>
#include <Game/Game.h>

#include <iostream>

#include <string>
#include <type_traits>


#define GAMEUPDATECONTEXT(context) static_cast<const GameUpdateContext&>(context.GetUpdateContext())
#define GETDATA(Data, context) *static_cast<Data*>(context.GetRuntimeInstanceData())
typedef Tga::ScriptNodeRuntimeInstanceBase NodeState;
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
class NotGate : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{

		using namespace Tga;


		{
			ScriptPin sourcePin = {};
			sourcePin.dataType = ScriptLinkDataType::Bool;
			sourcePin.name = ScriptStringRegistry::RegisterOrGetString("Input");
			sourcePin.node = context.GetNodeId();
			sourcePin.role = ScriptPinRole::Input;
			sourcePin.defaultValue = { false };
			myFirstInputPin = context.FindOrCreatePin(sourcePin);
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

		return { !firstCondition };

	}

private:
	Tga::ScriptPinId myResultPin;
	Tga::ScriptPinId myFirstInputPin;

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


class DelayNode : public ScriptNodeBase
{
private:
	struct Data : public NodeState
	{
		float myCurrentTime = 0;
	};


public:
	void Init(const Tga::ScriptCreationContext& context) override
	{
		{
			Tga::ScriptPin pin = {};
			pin.dataType = Tga::ScriptLinkDataType::Flow;
			pin.role = Tga::ScriptPinRole::Input;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Run") };

			context.FindOrCreatePin(pin);
		}


		{
			Tga::ScriptPin pin = {};
			pin.dataType = Tga::ScriptLinkDataType::Float;
			pin.role = Tga::ScriptPinRole::Input;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Seconds") };
			pin.defaultValue = { 1.0f };

			myTimePin = context.FindOrCreatePin(pin);
		}


		{
			Tga::ScriptPin pin = {};
			pin.dataType = Tga::ScriptLinkDataType::Flow;
			pin.role = Tga::ScriptPinRole::Output;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("") };

			myOutputFlowPin = context.FindOrCreatePin(pin);
		}



	}
	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId /*anIncomingPin*/) const override
	{
		auto& data = GETDATA(Data, someContext);

		auto time = std::get<float>(someContext.ReadInputPin(myTimePin).data);

		const GameUpdateContext& updateContext = GAMEUPDATECONTEXT(someContext);

		if (IncrementTime(data.myCurrentTime, updateContext.deltaTime, time))
		{
			someContext.TriggerOutputPin(myOutputFlowPin);
			return Tga::ScriptNodeResult::Finished;
		}

		return Tga::ScriptNodeResult::KeepRunning;
	}

	std::unique_ptr<Tga::ScriptNodeRuntimeInstanceBase> CreateRuntimeInstanceData() const override
	{
		return std::make_unique<Data>();
	}

private:
	const bool IncrementTime(float& aCurrentTime, const float aDeltaTime, const float aMaxTimeVal) const
	{
		aCurrentTime += aDeltaTime;
		return aCurrentTime >= aMaxTimeVal;
	}
private:
	Tga::ScriptPinId myTimePin;
	Tga::ScriptPinId myOutputFlowPin;
};
class TimerNode : public ScriptNodeBase
{
private:
	struct Data : public NodeState
	{
		float myCurrentTime = 0;
		bool myTriggerDefaultOutput = false;
	};


public:
	void Init(const Tga::ScriptCreationContext& context) override
	{
		{
			Tga::ScriptPin pin = {};
			pin.dataType = Tga::ScriptLinkDataType::Flow;
			pin.role = Tga::ScriptPinRole::Input;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Run") };

			context.FindOrCreatePin(pin);
		}


		{
			Tga::ScriptPin pin = {};
			pin.dataType = Tga::ScriptLinkDataType::Float;
			pin.role = Tga::ScriptPinRole::Input;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Seconds") };
			pin.defaultValue = { 1.0f };

			myTimePin = context.FindOrCreatePin(pin);
		}


		{
			Tga::ScriptPin pin = {};
			pin.dataType = Tga::ScriptLinkDataType::Flow;
			pin.role = Tga::ScriptPinRole::Output;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("") };

			myOutputFlowPin = context.FindOrCreatePin(pin);
		}


		{
			Tga::ScriptPin pin = {};
			pin.dataType = Tga::ScriptLinkDataType::Flow;
			pin.role = Tga::ScriptPinRole::Output;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Per Iteration") };

			myIteratedOutputFlowPin = context.FindOrCreatePin(pin);
		}



	}
	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId anIncomingPin) const override
	{
		auto& data = GETDATA(Data, someContext);

		if (!data.myTriggerDefaultOutput || anIncomingPin.id != anIncomingPin.InvalidId)
		{
			someContext.TriggerOutputPin(myOutputFlowPin);
			data.myTriggerDefaultOutput = true;
		}

		if (anIncomingPin.id == anIncomingPin.InvalidId)
		{
			auto time = std::get<float>(someContext.ReadInputPin(myTimePin).data);

			const GameUpdateContext& updateContext = GAMEUPDATECONTEXT(someContext);

			if (IncrementTime(data.myCurrentTime, updateContext.deltaTime, time))
			{
				someContext.TriggerOutputPin(myIteratedOutputFlowPin);
				data.myCurrentTime = 0;
			}
		}
		return Tga::ScriptNodeResult::KeepRunning;
	}

	std::unique_ptr<Tga::ScriptNodeRuntimeInstanceBase> CreateRuntimeInstanceData() const override
	{
		return std::make_unique<Data>();
	}

private:
	const bool IncrementTime(float& aCurrentTime, const float aDeltaTime, const float aMaxTimeVal) const
	{
		aCurrentTime += aDeltaTime;
		return aCurrentTime >= aMaxTimeVal;
	}
private:
	Tga::ScriptPinId myTimePin;
	Tga::ScriptPinId myOutputFlowPin;
	Tga::ScriptPinId myIteratedOutputFlowPin;
};




template<typename Type>
class VariableNode : public ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{
		{
			Tga::ScriptPin pin = {};
			pin.role = Tga::ScriptPinRole::Output;

			{
				pin.dataType = std::is_same<Type, bool>::value ? Tga::ScriptLinkDataType::Bool : pin.dataType;
				pin.dataType = std::is_same<Type, float>::value ? Tga::ScriptLinkDataType::Float : pin.dataType;
				pin.dataType = std::is_same<Type, Tga::ScriptStringId>::value ? Tga::ScriptLinkDataType::String : pin.dataType;
				pin.dataType = std::is_same<Type, int>::value ? Tga::ScriptLinkDataType::Int : pin.dataType;
			}

			{
				if constexpr (std::is_same<Type, bool>::value)
				{
					pin.defaultValue = { false };
				}

				if constexpr (std::is_same<Type, float>::value)
				{
					pin.defaultValue = { 0.0f };
				}

				if constexpr (std::is_same<Type, int>::value)
				{
					pin.defaultValue = { 0 };
				}

				if constexpr (std::is_same <Type, Tga::ScriptStringId > ::value)
				{
					pin.defaultValue = { Tga::ScriptStringRegistry::RegisterOrGetString("Empty") };
				}

			}

			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Output") };
			pin.node = context.GetNodeId();

			myVariableOutput = context.FindOrCreatePin(pin);

		}
		{
			Tga::ScriptPin pin = {};
			pin.role = Tga::ScriptPinRole::Input;

			{
				pin.dataType = std::is_same<Type, bool>::value ? Tga::ScriptLinkDataType::Bool : pin.dataType;
				pin.dataType = std::is_same<Type, float>::value ? Tga::ScriptLinkDataType::Float : pin.dataType;
				pin.dataType = std::is_same<Type, Tga::ScriptStringId>::value ? Tga::ScriptLinkDataType::String : pin.dataType;
				pin.dataType = std::is_same<Type, int>::value ? Tga::ScriptLinkDataType::Int : pin.dataType;
			}

			{
				if constexpr (std::is_same<Type, bool>::value)
				{
					pin.defaultValue = { false };
				}

				if constexpr (std::is_same<Type, float>::value)
				{
					pin.defaultValue = { 0.0f };
				}

				if constexpr (std::is_same<Type, int>::value)
				{
					pin.defaultValue = { 0 };
				}

				if constexpr (std::is_same <Type, Tga::ScriptStringId > ::value)
				{
					pin.defaultValue = { Tga::ScriptStringRegistry::RegisterOrGetString("Empty") };
				}

			}

			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Input") };
			pin.node = context.GetNodeId();

			myVariableInput = context.FindOrCreatePin(pin);

		}
		{
			Tga::ScriptPin pin = {};
			pin.role = Tga::ScriptPinRole::Input;

			{
				pin.dataType = Tga::ScriptLinkDataType::String;
			}

			{
				pin.defaultValue = { Tga::ScriptStringRegistry::RegisterOrGetString("Empty") };
			}

			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Input") };
			pin.node = context.GetNodeId();

			myVariableName = context.FindOrCreatePin(pin);

		}

		{
			Tga::ScriptPin pin = {};
			pin.role = Tga::ScriptPinRole::Output;
			pin.dataType = Tga::ScriptLinkDataType::Flow;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("") };
			myFlowOutputPin = context.FindOrCreatePin(pin);


		}

		{
			Tga::ScriptPin pin = {};
			pin.role = Tga::ScriptPinRole::Input;
			pin.dataType = Tga::ScriptLinkDataType::Flow;
			pin.node = context.GetNodeId();
			pin.name = { Tga::ScriptStringRegistry::RegisterOrGetString("Run") };
			context.FindOrCreatePin(pin);
		}

	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{

		auto& input = std::get<Type>(someContext.ReadInputPin(myVariableInput).data);
		auto& variableName = std::get<Tga::ScriptStringId>(someContext.ReadInputPin(myVariableName).data);

		someContext.GlobalVariable<Type>(variableName) = input;

		someContext.TriggerOutputPin(myFlowOutputPin);



		return Tga::ScriptNodeResult::Finished;
	}


	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId) const override
	{
		auto& variableName = std::get<Tga::ScriptStringId>(someContext.ReadInputPin(myVariableName).data);

		return { someContext.GlobalVariable<Type>(variableName) }
	}



private:
	Tga::ScriptPinId myVariableName;
	Tga::ScriptPinId myVariableOutput;
	Tga::ScriptPinId myVariableInput;
	Tga::ScriptPinId myOutputFlowPin;


};



void Tga::RegisterCommonNodes()
{
	ScriptNodeTypeRegistry::RegisterType<StartNode>("Common/Start", "A node that executes once when the script starts");
	ScriptNodeTypeRegistry::RegisterType<UpdateNode>("Common/Update", "A node that executes constantly when the script starts");
	ScriptNodeTypeRegistry::RegisterType<LoadLevel>("Level/Load new level", "A node that loads a new level based on a target name.");
	ScriptNodeTypeRegistry::RegisterType<BranchNode>("Logic/Branch", "Creates a branching node.");
	ScriptNodeTypeRegistry::RegisterType<AndGate>("Logic/Gates/AND", "Creates an AND Gate node.");
	ScriptNodeTypeRegistry::RegisterType<NotGate>("Logic/Gates/NOT", "Creates a NOT Gate node.");
	ScriptNodeTypeRegistry::RegisterType<StringCompareNode>("Logic/Comparison/Strings", "Compares two strings to see if they are the same, if they are, the node returns true");
	ScriptNodeTypeRegistry::RegisterType<CheckCollision>("Entity/Check Collision", "Compares two entities positions and sees if one of them is ontop of another");

	ScriptNodeTypeRegistry::RegisterType<SequencerNode<5>>("Logic/Sequencer/Set of 10", "Triggers a set of pins in a sequence.");
	ScriptNodeTypeRegistry::RegisterType<DelayNode>("Logic/Timer/Delay", "Triggers its output after a set delay in seconds");
	ScriptNodeTypeRegistry::RegisterType<TimerNode>("Logic/Timer/Iterate", "Triggers its event output on every iteration in seconds");

	ScriptNodeTypeRegistry::RegisterType<VariableNode<bool>>("Common/Variables/Local Bool", "");
	ScriptNodeTypeRegistry::RegisterType<VariableNode<int>>("Common/Variables/Local Int", "");
	ScriptNodeTypeRegistry::RegisterType<VariableNode<float>>("Common/Variables/Local Float", "");
	ScriptNodeTypeRegistry::RegisterType<VariableNode<Tga::ScriptStringId>>("Common/Variables/Local String", "");
}


