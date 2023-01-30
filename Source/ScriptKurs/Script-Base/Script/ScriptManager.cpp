#include "ScriptManager.h"

#include <Script/Script.h>
#include <Script/ScriptJson.h>
#include <Script/ScriptStringRegistry.h>

#include <tge/settings/settings.h>

#include <fstream>

using namespace Tga;

std::unordered_map<std::string_view, ScriptManager::InternalScriptData> ScriptManager::myLoadedScripts;

std::shared_ptr<Script> ScriptManager::GetScript(std::string_view name)
{
	if (!GetEditableScript(name))
		return nullptr;
	static std::unordered_map<Tga::ScriptStringId, std::shared_ptr<IData>> oldBlackboard;
	if (!oldBlackboard.empty())
	{
		oldBlackboard.clear();
	}

	ScriptManager::InternalScriptData& data = myLoadedScripts[name];
	if (data.script)
		data.script->ClearBlackboard();
	int sequenceNumber = data.script->GetSequenceNumber();
	if (data.latestSnapshotSequenceNumber == sequenceNumber)
		return data.latestSnapshot;

	std::shared_ptr<Script> newSnapshot = std::make_shared<Script>();
	newSnapshot->SetScriptName(name);
	ScriptJson json;
	data.script->WriteToJson(json);
	newSnapshot->LoadFromJson(json);
	newSnapshot->SetSequenceNumber(data.script->GetSequenceNumber());

	data.latestSnapshotSequenceNumber = sequenceNumber;
	if (data.latestSnapshot)
	{
		oldBlackboard = data.latestSnapshot->MoveBlackboard();
	}
	data.latestSnapshot = newSnapshot;

	return newSnapshot;
}

Script* ScriptManager::GetEditableScript(std::string_view name)
{
	auto it = myLoadedScripts.find(name);
	if (it != myLoadedScripts.end())
		return it->second.script.get();

	std::string path = Tga::Settings::ResolveGameAssetPath(("scripts/" + std::string(name) + ".json"));

	std::ifstream file(path);
	if (!file.is_open())
		return false;

	ScriptJson data = { nlohmann::json::parse(file) };
	file.close();

	ScriptStringId nameStringId = ScriptStringRegistry::RegisterOrGetString(name);
	std::string_view nameStringView = ScriptStringRegistry::GetStringFromStringId(nameStringId);

	myLoadedScripts.insert({ nameStringView, InternalScriptData{std::make_unique<Script>(), nullptr, -1} });
	Script& script = *myLoadedScripts[nameStringView].script;

	script.LoadFromJson(data);

	return &script;
}

void ScriptManager::AddEditableScript(std::string_view name, std::unique_ptr<Script>&& script)
{
	ScriptStringId nameStringId = ScriptStringRegistry::RegisterOrGetString(name);
	std::string_view nameStringView = ScriptStringRegistry::GetStringFromStringId(nameStringId);

	myLoadedScripts.insert({ nameStringView, InternalScriptData{std::move(script), nullptr, -1} });
}