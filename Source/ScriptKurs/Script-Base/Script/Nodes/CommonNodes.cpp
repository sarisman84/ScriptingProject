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

public:
	void Init(const ScriptCreationContext& context) override
	{
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Flow;
		outputPin.name = ScriptStringRegistry::RegisterOrGetString("Start");
		outputPin.node = context.GetNodeId();
		outputPin.role = ScriptPinRole::Output;

		myOutputPin = context.FindOrCreatePin(outputPin);
	}

	ScriptNodeResult Execute(ScriptExecutionContext& context, ScriptPinId) const override
	{
		context.TriggerOutputPin(myOutputPin);

		return ScriptNodeResult::Finished;
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
			ScriptPin outputPin = {};
			outputPin.dataType = ScriptLinkDataType::Flow;
			outputPin.name = ScriptStringRegistry::RegisterOrGetString("");
			outputPin.node = context.GetNodeId();
			outputPin.role = ScriptPinRole::Output;
			myFlowOutputPin = context.FindOrCreatePin(outputPin);
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
		context.TriggerOutputPin(myFlowOutputPin);
		const GameUpdateContext& updateContext = static_cast<const GameUpdateContext&>(context.GetUpdateContext());
		auto levelName = std::get<Tga::ScriptStringId>(context.ReadInputPin(myLevelNamePin).data);
		updateContext.game.LoadLevel(Tga::ScriptStringRegistry::GetStringFromStringId(levelName).data(), false);
		return ScriptNodeResult::Finished;
	}
private:
	Tga::ScriptPinId myFlowOutputPin;
	Tga::ScriptPinId myLevelNamePin;

};
void Tga::RegisterCommonNodes()
{
	ScriptNodeTypeRegistry::RegisterType<StartNode>("Common/Start", "A node that executes once when the script starts");
	ScriptNodeTypeRegistry::RegisterType<UpdateNode>("Common/Update", "A node that executes constantly when the script starts");

	ScriptNodeTypeRegistry::RegisterType<LoadLevel>("Level/Load new level", "A node that loads a new level based on a target name.");
}
