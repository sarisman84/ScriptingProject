#pragma once

#include <memory>
#include <Script/ScriptCreationContext.h>
#include <Script/ScriptExecutionContext.h>
#include <Script/ScriptCommon.h>


namespace Tga
{
	class ScriptNodeRuntimeInstanceBase
	{
	public:
		virtual ~ScriptNodeRuntimeInstanceBase() {}
	};


	class ScriptNodeBase
	{
	public:
		virtual ~ScriptNodeBase() {}

		// todo custom editor UI api? also allow it to edit number of pins and be notified on connections
		// would be nice to be able to have a variable number of pins for example!

		virtual void Init(const ScriptCreationContext& context) = 0;
		virtual std::unique_ptr<ScriptNodeRuntimeInstanceBase> CreateRuntimeInstanceData() const { return nullptr; }

		virtual ScriptLinkData ReadPin(ScriptExecutionContext&, ScriptPinId) const { return {}; }

		virtual void LoadFromJson(const ScriptJson&) {}
		virtual void WriteToJson(ScriptJson&) const { return; }

		virtual ScriptNodeResult Execute(ScriptExecutionContext&, ScriptPinId) const { return ScriptNodeResult::Finished; }
		virtual bool ShouldExecuteAtStart() const { return false; }
	};

}