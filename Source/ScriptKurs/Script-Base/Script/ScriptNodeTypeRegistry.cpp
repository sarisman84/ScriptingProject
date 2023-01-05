#include "ScriptNodeTypeRegistry.h"

#include <Script/ScriptStringRegistry.h>

#include <cassert>
#include <sstream>

using namespace Tga;

std::unordered_map<std::string_view, ScriptNodeTypeId> ScriptNodeTypeRegistry::myStringToTypeId;
std::vector<ScriptNodeTypeRegistry::TypeInfo> ScriptNodeTypeRegistry::myTypeInfos;
ScriptNodeTypeRegistry::CategoryInfo ScriptNodeTypeRegistry::myRootCategory = {};


ScriptNodeTypeId Tga::ScriptNodeTypeRegistry::GetTypeId(std::string_view typeName)
{
	auto it = myStringToTypeId.find(typeName);
	if (it != myStringToTypeId.end())
		return it->second;

	return { 0xFFFFFFFF };
}

std::unique_ptr<ScriptNodeBase> Tga::ScriptNodeTypeRegistry::CreateNode(ScriptNodeTypeId typeId)
{
	assert("Invalid type id" && typeId.id < myTypeInfos.size());

	return (*myTypeInfos[typeId.id].createNodeFunctionPtr)();
}

std::string_view Tga::ScriptNodeTypeRegistry::GetNodeTypeShortName(ScriptNodeTypeId typeId)
{
	assert("Invalid type id" && typeId.id < myTypeInfos.size());

	return myTypeInfos[typeId.id].shortName;
}

std::string_view Tga::ScriptNodeTypeRegistry::GetNodeTypeFullName(ScriptNodeTypeId typeId)
{
	assert("Invalid type id" && typeId.id < myTypeInfos.size());

	return myTypeInfos[typeId.id].fullName;
}

std::string_view Tga::ScriptNodeTypeRegistry::GetNodeTooltip(ScriptNodeTypeId typeId)
{
	assert("Invalid type id" && typeId.id < myTypeInfos.size());

	return myTypeInfos[typeId.id].toolTip;
}

ScriptNodeTypeRegistry::TypeInfo& ScriptNodeTypeRegistry::RegisterTypeInternal(const char* fullName, const char* toolTip)
{
	ScriptNodeTypeId typeId = { (unsigned int)myTypeInfos.size() };

	TypeInfo& typeInfo = myTypeInfos.emplace_back();

	typeInfo.fullName = fullName;

	const char* shortName = strrchr(fullName, '/');
	if (shortName == nullptr)
		shortName = fullName;
	else
		shortName += 1; // skip the '/'
	typeInfo.shortName = shortName;

	std::istringstream f(fullName);
	std::string s;

	CategoryInfo* category = &myRootCategory;
	while (std::getline(f, s, '/'))
	{
		ScriptStringId stringId = ScriptStringRegistry::RegisterOrGetString(s.c_str());
		typeInfo.path.push_back(stringId);
	}

	for (int pathIndex = 0; pathIndex + 1 < typeInfo.path.size(); pathIndex++)
	{
		ScriptStringId stringId = typeInfo.path[pathIndex];
		int childCategoryIndex = -1;
		for (int i = 0; i < category->childCategories.size(); i++)
		{
			if (category->childCategories[i].name == stringId)
			{
				childCategoryIndex = i;
			}
		}

		if (childCategoryIndex == -1)
		{
			childCategoryIndex = (int)category->childCategories.size();
			CategoryInfo& childCategory = category->childCategories.emplace_back();
			childCategory.name = stringId;
		}

		category = &category->childCategories[childCategoryIndex];
	}

	category->nodeTypes.push_back(typeId);

	typeInfo.toolTip = toolTip;

	assert("Node type name (without category) already exists. Duplicates are not allowed" && myStringToTypeId.find(typeInfo.shortName) == myStringToTypeId.end());
	assert("Node type name (including cagories) already exists. Duplicates are not allowed" && myStringToTypeId.find(typeInfo.fullName) == myStringToTypeId.end());

	myStringToTypeId[typeInfo.shortName] = typeId;
	myStringToTypeId[typeInfo.fullName] = typeId;

	return typeInfo;
}
