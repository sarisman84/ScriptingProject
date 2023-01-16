#pragma once

#include <Game/Entity.h>
#include <unordered_map>

class EntityContainer
{
	std::unordered_map<uint64_t, std::unique_ptr<Entity>> myEntities;

public:
	inline Entity* GetEntity(Tga::ScriptStringId id);

	/// <summary>
	/// Creates a new entity. All entities in an EntityContainer must have unique id.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="id"></param>
	/// <returns></returns>
	template<typename T>
	inline T* CreateEntity(Tga::ScriptStringId id);

	inline bool DestroyEntity(Tga::ScriptStringId id);

	template<typename F>
	inline void ForEachEntityAtPosition(Tga::Vector2i position, F&& action);
	template<typename F>
	inline void ForEachEntity(F&& action);
};

Entity* EntityContainer::GetEntity(Tga::ScriptStringId id)
{
	auto it = myEntities.find(id.id);
	if (it == myEntities.end())
		return nullptr;

	return it->second.get();
}

template<typename T>
T* EntityContainer::CreateEntity(Tga::ScriptStringId id)
{
	// fail if there is an existing entity with the same name
	Entity* existingEntity = GetEntity(id);
	if (existingEntity)
		return nullptr;

	myEntities[id.id] = std::make_unique<T>();
	Entity* entity = myEntities[id.id].get();
	entity->id = { id };

	return static_cast<T*>(entity);
}

bool EntityContainer::DestroyEntity(Tga::ScriptStringId id)
{
	return myEntities.erase(id.id);
}

template<typename F>
void EntityContainer::ForEachEntityAtPosition(Tga::Vector2i position, F&& action)
{
	for (auto& pair : myEntities)
	{
		Entity& entity = *pair.second;
		if (entity.position == position)
			action(entity);
	}
}

template<typename F>
void EntityContainer::ForEachEntity(F&& action)
{

	for (auto& pair : myEntities)
	{
		if (pair.second)
		{
			Entity& entity = *pair.second;
			action(entity);
		}
		
	}
}