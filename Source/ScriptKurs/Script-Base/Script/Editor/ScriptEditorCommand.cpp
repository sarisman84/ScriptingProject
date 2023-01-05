#include "ScriptEditorCommand.h"

#include <Script/Script.h>

using namespace Tga;

void ScriptEditorCommand::Execute()
{
	mySequenceNumber = myScript.GetSequenceNumber();
	ExecuteImpl();
};

void ScriptEditorCommand::Undo()
{
	UndoImpl();
	myScript.SetSequenceNumber(mySequenceNumber);
};