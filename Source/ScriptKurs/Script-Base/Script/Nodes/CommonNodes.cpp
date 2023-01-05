#include "CommonNodes.h"

#include <Script/ScriptNodeTypeRegistry.h>
#include <Script/ScriptStringRegistry.h>
#include <Script/ScriptCommon.h>
#include <Script/ScriptNodeBase.h>


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

void Tga::RegisterCommonNodes()
{
	ScriptNodeTypeRegistry::RegisterType<StartNode>("Common/Start", "A node that executes once when the script starts");
	ScriptNodeTypeRegistry::RegisterType<UpdateNode>("Common/Update", "A node that executes constantly when the script starts");
}
