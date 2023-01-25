#include "ScriptRuntimeInstance.h"

using namespace Tga;


ScriptRuntimeInstance::ScriptRuntimeInstance(std::shared_ptr<const Script>& script)
	: myScript(script)
{
	assert(script);
}

void ScriptRuntimeInstance::Init()
{
	ScriptNodeId lastNodeId = myScript->GetLastNodeId();
	myNodeInstances.resize(lastNodeId.id + 1);

	for (ScriptNodeId currentNodeId = myScript->GetFirstNodeId(); currentNodeId.id != ScriptNodeId::InvalidId; currentNodeId = myScript->GetNextNodeId(currentNodeId))
	{
		const ScriptNodeBase& node = myScript->GetNode(currentNodeId);
		myNodeInstances[currentNodeId.id] = node.CreateRuntimeInstanceData();

		if (node.ShouldExecuteAtStart())
		{
			myActiveNodes.push_back(currentNodeId);
		}
	}
}

void ScriptRuntimeInstance::Update(const ScriptUpdateContext& updateContext)
{
	for (int i = 0; i < myActiveNodes.size(); i++)
	{
		ScriptNodeId nodeId = myActiveNodes[i];
		ScriptExecutionContext executionContext(*this, updateContext, nodeId, myCallOrder[nodeId], myNodeInstances[nodeId.id].get());
		const ScriptNodeBase& node = myScript->GetNode(nodeId);

		ScriptNodeResult result = node.Execute(executionContext, { ScriptPinId::InvalidId });
		if (result == ScriptNodeResult::Finished)
		{
			myActiveNodes.erase(begin(myActiveNodes) + i);
			i--;
		}
	}
}

void ScriptRuntimeInstance::TriggerPin(ScriptPinId pinId, const ScriptUpdateContext& updateContext)
{
	ScriptPin pin = myScript->GetPin(pinId);
	ScriptNodeId nodeId = pin.node;
	ScriptExecutionContext executionContext(*this, updateContext, nodeId, myCallOrder[nodeId], myNodeInstances[nodeId.id].get());
	const ScriptNodeBase& node = myScript->GetNode(nodeId);

	assert(pin.dataType == ScriptLinkDataType::Flow);
	if (pin.role == ScriptPinRole::Input)
	{
		ScriptNodeResult result = node.Execute(executionContext, pinId);
		if (result == ScriptNodeResult::KeepRunning)
		{
			ActivateNode(nodeId, myCallOrder[nodeId]);
		}
		else
		{
			DeactivateNode(nodeId, myCallOrder[nodeId]);
		}
	}
	else
	{
		executionContext.TriggerOutputPin(pinId);
	}
}

const Script& ScriptRuntimeInstance::GetScript() const
{
	return *myScript;
}

ScriptNodeRuntimeInstanceBase* ScriptRuntimeInstance::GetRuntimeInstance(ScriptNodeId nodeId)
{
	assert("Invalid node" && nodeId.id < myNodeInstances.size());
	return myNodeInstances[nodeId.id].get();
}

void ScriptRuntimeInstance::ActivateNode(ScriptNodeId nodeId, ScriptNodeId prevNodeId)
{
	myCallOrder[nodeId] = prevNodeId;
	for (int i = 0; i < myActiveNodes.size(); i++)
	{
		if (myActiveNodes[i] == nodeId)
		{
			return;
		}
	}

	myActiveNodes.push_back(nodeId);
}

void ScriptRuntimeInstance::DeactivateNode(ScriptNodeId nodeId, ScriptNodeId prevNodeId)
{
	myCallOrder[nodeId] = prevNodeId;
	for (int i = 0; i < myActiveNodes.size(); i++)
	{
		if (myActiveNodes[i] == nodeId)
		{
			myActiveNodes.erase(begin(myActiveNodes) + i);
			return;
		}
	}
}
