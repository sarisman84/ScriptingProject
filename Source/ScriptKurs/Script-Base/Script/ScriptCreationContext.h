#pragma once

#include <Script/ScriptCommon.h>

namespace Tga
{

class Script;

class ScriptCreationContext
{
	Script& myScript;
	ScriptNodeId myNodeId;
public:
	ScriptCreationContext(Script& script, ScriptNodeId nodeId);
	ScriptNodeId GetNodeId() const { return myNodeId; };
	ScriptPinId FindOrCreatePin(ScriptPin pinData) const;
};

} // namespace Tga