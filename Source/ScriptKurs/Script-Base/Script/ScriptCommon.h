#pragma once

#include <variant>

namespace Tga
{
	class ScriptRuntimeInstance;
	struct ScriptUpdateContext;
	class ScriptCreationContext;
	class ScriptExecutionContext;
	class ScriptNodeBase;
	class ScriptNodeRuntimeInstanceBase;
	struct ScriptJson;

	// TODO: setup .natvis for all the common ID types
	// might need to add a Script pointer to IDs for debugging purposes

	struct ScriptStringId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptStringId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptStringId& other) const { return this->id != other.id; }
	};

	// TODO should change this to be extensible
	// use static constructor trick to register types efficiently
	enum class ScriptLinkDataType
	{
		Unknown,
		Flow,
		Bool,
		Int,
		Float,
		String,
		Count
	};

	constexpr char* ScriptLinkDataTypeNames[(size_t)ScriptLinkDataType::Count] =
	{
		"Unknown",
		"Flow",
		"Bool",
		"Int",
		"Float",
		"String"
	};

	struct ScriptLinkData
	{
		std::variant<std::monostate, bool, int, float, ScriptStringId> data;

		void LoadFromJson(ScriptLinkDataType type, const ScriptJson& data);
		void WriteToJson(ScriptLinkDataType type, ScriptJson& data) const;
	};

	struct ScriptPinId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptPinId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptPinId& other) const { return this->id != other.id; }
	};

	struct ScriptNodeId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptNodeId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptNodeId& other) const { return this->id != other.id; }
	};

	struct ScriptLinkId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptLinkId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptLinkId& other) const { return this->id != other.id; }
	};

	struct ScriptNodeTypeId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptNodeTypeId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptNodeTypeId& other) const { return this->id != other.id; }
	};

	enum class ScriptNodeResult
	{
		Finished,
		KeepRunning,
	};

	struct ScriptLink
	{
		ScriptPinId sourcePinId;
		ScriptPinId targetPinId;
	};

	enum class ScriptPinRole
	{
		Input,
		Output,
	};

	struct ScriptPin
	{
		ScriptPinRole role;
		int sortingNumber;
		ScriptNodeId node;
		ScriptStringId name;
		ScriptLinkDataType dataType;
		ScriptLinkData defaultValue;
		ScriptLinkData overridenValue;
	};
} // namespace Tga

template<>
struct std::hash<Tga::ScriptStringId>
{
	std::size_t operator()(Tga::ScriptStringId const& id) const noexcept
	{
		return id.id;
	}
};

template<>
struct std::hash<Tga::ScriptPinId>
{
	std::size_t operator()(Tga::ScriptPinId const& id) const noexcept
	{
		return id.id;
	}
};

template<>
struct std::hash<Tga::ScriptNodeId>
{
	std::size_t operator()(Tga::ScriptNodeId const& id) const noexcept
	{
		return id.id;
	}
};

template<>
struct std::hash<Tga::ScriptLinkId>
{
	std::size_t operator()(Tga::ScriptLinkId const& id) const noexcept
	{
		return id.id;
	}
};

template<>
struct std::hash<Tga::ScriptNodeTypeId>
{
	std::size_t operator()(Tga::ScriptNodeTypeId const& id) const noexcept
	{
		return id.id;
	}
};

