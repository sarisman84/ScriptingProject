#pragma once

#include <Script/ScriptCommon.h>
#include <Script/ScriptNodeBase.h>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Tga
{

// todo change this to use static constructor trick to avoid having to ensure everything is registered early
class ScriptNodeTypeRegistry
{
public:
	struct CategoryInfo
	{
		ScriptStringId name;
		std::vector<CategoryInfo> childCategories;
		std::vector<ScriptNodeTypeId> nodeTypes;
	};

private:
	struct TypeInfo
	{
		const char* fullName;
		const char* shortName;
		const char* toolTip;
		std::vector<ScriptStringId> path;
		std::unique_ptr<ScriptNodeBase>(*createNodeFunctionPtr)(void);
	};

	static std::unordered_map<std::string_view, ScriptNodeTypeId> myStringToTypeId;
	static std::vector<TypeInfo> myTypeInfos;
	static CategoryInfo myRootCategory;

	static TypeInfo& RegisterTypeInternal(const char* fullName, const char* toolTip);

public:
	static const CategoryInfo& GetRootCategory() { return myRootCategory; }

	static ScriptNodeTypeId GetTypeId(std::string_view typeName);

	static std::unique_ptr<ScriptNodeBase> CreateNode(ScriptNodeTypeId typeId);
	static std::string_view GetNodeTypeShortName(ScriptNodeTypeId typeId);
	static std::string_view GetNodeTypeFullName(ScriptNodeTypeId typeId);

	static std::string_view GetNodeTooltip(ScriptNodeTypeId typeId);

	/// <summary>
	/// Registers a node type. Must be called for all node types used in a script, before loading the script. Full name should have the structure "ExampleCategory/OtherCategory/.../NodeTypeName"
	/// Full name as well as the node type name both have to be unique
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="fullName"></param>
	/// <param name="toolTip"></param>
	/// <returns></returns>
	template<typename T>
	static ScriptNodeTypeId RegisterType(const char* fullName, const char* toolTip);
};

namespace detail
{
	template<typename T>
	static std::unique_ptr<ScriptNodeBase> CreateScriptNodeInstance()
	{
		return std::make_unique<T>();
	};
}

template<typename T>
ScriptNodeTypeId ScriptNodeTypeRegistry::RegisterType(const char* fullName, const char* toolTip)
{
	ScriptNodeTypeId typeId = { (unsigned int)myTypeInfos.size() };
	TypeInfo& typeInfo = RegisterTypeInternal(fullName, toolTip);
	typeInfo.createNodeFunctionPtr = &detail::CreateScriptNodeInstance<T>;

	return typeId;
}

} // namespace Tga