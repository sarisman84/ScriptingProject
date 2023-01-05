#include "SetOverridenValueCommand.h"

#include <Script/Script.h>

using namespace Tga;

void SetOverridenValueCommand::ExecuteImpl()
{
	ScriptPin pin = myScript.GetPin(myPinId);

	assert("New overriden value has different type than current value" && pin.defaultValue.data.index() == myNewData.data.index());

	myOldData = pin.overridenValue;
	pin.overridenValue = myNewData;

	myScript.SetPin(myPinId, pin);
}

void SetOverridenValueCommand::UndoImpl()
{
	ScriptPin pin = myScript.GetPin(myPinId);

	pin.overridenValue = myOldData;

	myScript.SetPin(myPinId, pin);
}

