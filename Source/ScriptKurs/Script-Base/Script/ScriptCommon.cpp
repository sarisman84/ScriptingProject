#include "ScriptCommon.h"

#include <Script/ScriptJson.h>
#include <Script/ScriptStringRegistry.h>

using namespace Tga;

void ScriptLinkData::LoadFromJson(ScriptLinkDataType type, const ScriptJson& json)
{
	if (!json.json.is_null())
	{
		switch (type)
		{
		case ScriptLinkDataType::Bool:
			data = json.json.get<bool>();
			break;
		case ScriptLinkDataType::Int:
			data = json.json.get<int>();
			break;
		case ScriptLinkDataType::Float:
			data = json.json.get<float>();
			break;
		case ScriptLinkDataType::String:
			std::string string = json.json.get<std::string>();
			data = ScriptStringRegistry::RegisterOrGetString(string);
			break;
		}
	}
}

void ScriptLinkData::WriteToJson(ScriptLinkDataType type, ScriptJson& json) const
{
	if (!std::holds_alternative<std::monostate>(data))
	{
		switch (type)
		{
		case ScriptLinkDataType::Bool:
			json.json = std::get<bool>(data);
			break;
		case ScriptLinkDataType::Int:
			json.json = std::get<int>(data);
			break;
		case ScriptLinkDataType::Float:
			json.json = std::get<float>(data);
			break;
		case ScriptLinkDataType::String:
			ScriptStringId stringId = std::get<ScriptStringId>(data);
			json.json = ScriptStringRegistry::GetStringFromStringId(stringId).data();
			break;
		}
	}
}