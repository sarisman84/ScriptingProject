#include "GameplayNodes.h"

#include <Script/ScriptNodeTypeRegistry.h>
#include <Script/ScriptStringRegistry.h>
#include <Script/ScriptCommon.h>
#include <Script/ScriptNodeBase.h>


void CreatePushPlateNode::Init(const Tga::ScriptCreationContext& context)
{
}

Tga::ScriptNodeResult CreatePushPlateNode::Execute(Tga::ScriptExecutionContext& someContext, Tga::ScriptPinId aPin) const
{
	return Tga::ScriptNodeResult();
}

void Tga::RegisterGameplayNodes()
{
	ScriptNodeTypeRegistry::RegisterType<CreatePushPlateNode>("Game/CreatePushPlate", "A node that creates a push plate at a set coordinate");
}
