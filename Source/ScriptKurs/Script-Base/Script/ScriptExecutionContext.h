#pragma once

#include <Script/ScriptCommon.h>

namespace Tga
{

class ScriptExecutionContext
{
	// todo: report all nodes and links activated the last few seconds to some kind of debug service, to be able to show execution flow
	// use to color edges and nodes somehow
	// also, could implement breakpoints in a debug service

	static constexpr int MAX_TRIGGERED_OUTPUTS = 8;

	ScriptRuntimeInstance& myScriptRuntimeInstance;
	const ScriptUpdateContext& myUpdateContext;
	ScriptNodeId myNodeId;
	ScriptNodeId myPreviousNodeId;
	ScriptNodeRuntimeInstanceBase* myNodeRuntimeInstance;

	ScriptPinId myTriggeredOutputQueue[MAX_TRIGGERED_OUTPUTS];
	int myTriggeredOutputCount;

public:
	ScriptExecutionContext(ScriptRuntimeInstance& scriptRuntimeInstance, const ScriptUpdateContext& updateContext, ScriptNodeId nodeId,ScriptNodeId previousId, ScriptNodeRuntimeInstanceBase* nodeRuntimeInstance);
	~ScriptExecutionContext();
	const ScriptUpdateContext& GetUpdateContext();
	ScriptNodeRuntimeInstanceBase* GetRuntimeInstanceData();

	/// <summary>
	/// Triggers and output pin. The execution is deferred until the ScriptExecutionContext is destroyed.
	/// </summary>
	/// <param name="outputPin"></param>
	void TriggerOutputPin(ScriptPinId outputPin);

	/// <summary>
	/// Reads an input pin. This reading functions to be called on the corresponding node immediately. 
	/// </summary>
	/// <param name="inputPin"></param>
	/// <returns></returns>
	ScriptLinkData ReadInputPin(ScriptPinId inputPin);
	const ScriptNodeBase& GetPreviousNode() const ;
};

} // namespace Tga