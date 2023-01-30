#pragma once

#include <unordered_map>
#include <memory>
#include <string_view>
namespace Tga
{

	class Script;
	class ScriptManager
	{
		// todo: add simpler path for retail, where scripts aren't editable
		struct InternalScriptData
		{
			std::unique_ptr<Script> script;
			std::shared_ptr<Script> latestSnapshot;
			int latestSnapshotSequenceNumber = -1;
		};

		static std::unordered_map<std::string_view, InternalScriptData> myLoadedScripts;
	public:
		/// <summary>
		/// Gets the latest version of a script. If a script is edited, this script will not be updated. This way a script can be executed while being edited without risking inconsistent data.
		/// To get the latest version, this function has to be called again.
		/// This means that multiple versions of a script can exist, depending on when this got called.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		static std::shared_ptr<Script> GetScript(std::string_view name);

		/// <summary>
		/// Gets an editable version of a script. Only a single editable version exists 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		static Script* GetEditableScript(std::string_view name);
		static void AddEditableScript(std::string_view name, std::unique_ptr<Script>&& script);
	};

} // namespace Tga