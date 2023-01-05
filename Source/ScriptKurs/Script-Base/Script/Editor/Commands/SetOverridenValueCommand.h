#pragma once

#include <Script/Editor/ScriptEditorCommand.h>

namespace Tga
{
	class SetOverridenValueCommand : public ScriptEditorCommand
	{
		ScriptPinId myPinId;
		ScriptLinkData myNewData;
		ScriptLinkData myOldData;

	public:
		SetOverridenValueCommand(Script& script, ScriptEditorSelection& selection, ScriptPinId pinId, ScriptLinkData data)
			: ScriptEditorCommand(script, selection)
			, myPinId(pinId)
			, myNewData(data)
		{}

		void ExecuteImpl() override;
		void UndoImpl() override;
	};
} // namespace Tga