#pragma once

#include <vector>
#include <Script/ScriptCommon.h>

namespace Tga
{

	struct ScriptEditorSelection
	{
		std::vector<ScriptNodeId> mySelectedNodes;
		std::vector<ScriptLinkId> mySelectedLinks;
	};
} // namespace Tga