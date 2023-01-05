#pragma once
#include "Script/ScriptNodeBase.h"
namespace Tga
{
	void RegisterGameplayNodes();

}


class CreatePushPlateNode : Tga::ScriptNodeBase
{
public:
	// Inherited via ScriptNodeBase
	void Init(const Tga::ScriptCreationContext& context) override;
	Tga::ScriptNodeResult Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId aPin) const override;
private:

};