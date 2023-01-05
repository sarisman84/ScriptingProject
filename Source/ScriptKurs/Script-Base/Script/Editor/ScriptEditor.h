#pragma once

#include <Script/Editor/ScriptEditorSelection.h>
#include <Script/ScriptCommon.h>
#include <Script/Script.h>

struct ImNodesEditorContext;

namespace Tga
{
	
constexpr uint8_t ScriptLinkColors[(size_t)ScriptLinkDataType::Count][3] =
{
	{245, 0, 0},
	{185, 185, 185},
	{126, 0, 0},
	{13, 206, 151},
	{137, 235, 43},
	{206, 43, 206},
};

constexpr uint8_t ScriptLinkHoverColors[(size_t)ScriptLinkDataType::Count][3] =
{
	{255, 50, 50},
	{255, 255, 255},
	{196, 50, 50},
	{83, 255, 221},
	{207, 255, 113},
	{255, 113, 255},
};

constexpr uint8_t ScriptLinkSelectedColors[(size_t)ScriptLinkDataType::Count][3] =
{
	{255, 50, 50},
	{255, 255, 255},
	{196, 50, 50},
	{83, 255, 221},
	{207, 255, 113},
	{255, 113, 255},
};

class MoveNodesCommand;

class ScriptEditor
{
	struct EditorScriptData
	{
		Script* script;
		ScriptEditorSelection selection = {};
		ImNodesEditorContext* nodeEditorContext = nullptr;
		ScriptPinId inProgressLinkPin = { ScriptPinId::InvalidId };
		ScriptNodeId hoveredNode = { ScriptNodeId::InvalidId };
		int latestSavedSequenceNumber = 0;
	};

	std::map<std::string_view, EditorScriptData> myOpenScripts;
	std::string_view myActiveScript;

	std::shared_ptr<MoveNodesCommand> myInProgressMove;
public:
	ScriptEditor();
	~ScriptEditor();

	void Init();
	void Update(float aTimeDelta);
};

} // namespace Tga