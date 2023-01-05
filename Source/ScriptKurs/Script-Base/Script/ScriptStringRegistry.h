#pragma once

#include <Script/ScriptCommon.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace Tga
{

class ScriptStringRegistry
{
private:
	static std::vector<std::unique_ptr<std::string>> myDeduplicatedStrings;
	static std::unordered_map<std::string_view, ScriptStringId> myStringToDeduplicatedString;

public:
	static ScriptStringId RegisterOrGetString(std::string_view string);
	static std::string_view GetStringFromStringId(ScriptStringId stringId);
};

} // namespace Tga