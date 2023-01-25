#pragma once

#include <Script/Script.h>
#include <Script/ScriptNodeBase.h>
#include <vector>

namespace Tga
{
	struct ScriptUpdateContext;

	class ScriptRuntimeInstance
	{
		std::shared_ptr<const Script> myScript;

		std::vector<std::unique_ptr<ScriptNodeRuntimeInstanceBase>> myNodeInstances;
		std::vector<ScriptNodeId> myActiveNodes;
		std::unordered_map<ScriptNodeId, ScriptNodeId> myCallOrder;

	public:
		ScriptRuntimeInstance(std::shared_ptr<const Script>& script);
		void Init();
		void Update(const ScriptUpdateContext& context);
		void TriggerPin(ScriptPinId pinId, const ScriptUpdateContext& updateContext);

		const Script& GetScript() const;
		ScriptNodeRuntimeInstanceBase* GetRuntimeInstance(ScriptNodeId nodeId);
		void ActivateNode(ScriptNodeId nodeId, ScriptNodeId prevNodeId);
		void DeactivateNode(ScriptNodeId nodeId, ScriptNodeId prevNodeId);
	};


} // namespace Tga