#pragma once

#include <Script/ScriptCommon.h>
#include <Script/ScriptNodeBase.h>
#include <Script/ScriptNodeTypeRegistry.h>
#include <Script/ScriptStringRegistry.h>
#include <iostream>

class ReturnFiveNode : public Tga::ScriptNodeBase
{
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{
		using namespace Tga;
		ScriptPin outputPin = {};
		outputPin.dataType = ScriptLinkDataType::Int;
		outputPin.name = ScriptStringRegistry::RegisterOrGetString("Value");
		outputPin.node = context.GetNodeId();
		outputPin.role = ScriptPinRole::Output;

		context.FindOrCreatePin(outputPin);
	}

	Tga::ScriptLinkData ReadPin(Tga::ScriptExecutionContext&, Tga::ScriptPinId) const override
	{
		return { 5 };
	}
};



class PrintIntNode : public Tga::ScriptNodeBase
{
	Tga::ScriptPinId myIntPinId;
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
			ScriptPin flowOutPin = {};
			flowOutPin.dataType = ScriptLinkDataType::Flow;
			flowOutPin.name = ScriptStringRegistry::RegisterOrGetString("");
			flowOutPin.node = context.GetNodeId();
			flowOutPin.role = ScriptPinRole::Output;

			myOutPinId = context.FindOrCreatePin(flowOutPin);
		}

		{
			ScriptPin intPin = {};
			intPin.dataType = ScriptLinkDataType::Int;
			intPin.name = ScriptStringRegistry::RegisterOrGetString("Value");
			intPin.node = context.GetNodeId();
			intPin.defaultValue = { 0 };
			intPin.role = ScriptPinRole::Input;

			myIntPinId = context.FindOrCreatePin(intPin);
		}
	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	{
		using namespace Tga;

		ScriptLinkData data = context.ReadInputPin(myIntPinId);
		std::cout << std::get<int>(data.data) << "\n";

		context.TriggerOutputPin(myOutPinId);

		return ScriptNodeResult::Finished;
	}
};

class PrintStringNode : public Tga::ScriptNodeBase
{
	Tga::ScriptPinId myStringPinId;
	Tga::ScriptPinId myOutPinId;
public:
	void Init(const Tga::ScriptCreationContext& context) override
	{
		using namespace Tga;

		{
			ScriptPin flowInPin = {};
			flowInPin.dataType = ScriptLinkDataType::Flow;
			flowInPin.name = ScriptStringRegistry::RegisterOrGetString("Run");
			flowInPin.node = context.GetNodeId();
			flowInPin.role = ScriptPinRole::Input;

			context.FindOrCreatePin(flowInPin);
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
			ScriptPin intPin = {};
			intPin.dataType = ScriptLinkDataType::String;
			intPin.name = ScriptStringRegistry::RegisterOrGetString("");
			intPin.node = context.GetNodeId();
			intPin.defaultValue = { ScriptStringRegistry::RegisterOrGetString("Text to print") };
			intPin.role = ScriptPinRole::Input;

			myStringPinId = context.FindOrCreatePin(intPin);
		}
	}

	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& context, Tga::ScriptPinId) const override
	{
		using namespace Tga;

		ScriptLinkData data = context.ReadInputPin(myStringPinId);

		ScriptStringId stringId = std::get<ScriptStringId>(data.data);
		std::cout << ScriptStringRegistry::GetStringFromStringId(stringId) << "\n";

		context.TriggerOutputPin(myOutPinId);

		return ScriptNodeResult::Finished;
	}
};


void RegisterExampleNodes()
{
	Tga::ScriptNodeTypeRegistry::RegisterType<PrintIntNode>("Examples/PrintInt", "Prints an integer");
	Tga::ScriptNodeTypeRegistry::RegisterType<PrintStringNode>("Examples/PrintString", "Prints a string");
	Tga::ScriptNodeTypeRegistry::RegisterType<ReturnFiveNode>("Examples/ReturnFive", "Returns five");
}
