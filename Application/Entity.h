#ifndef KTY_OBJ
#define KTY_OBJ

#include "main.h"
#include <bitset>
#include <queue>
#include <array>
#include <unordered_map>
#include <set>

using Entity = std::uint32_t;
using ComponentType = std::uint8_t;

const Entity MaxEntities = 5000;
const ComponentType MaxComponents = 32;

using EntitySignature = std::bitset<MaxComponents>;

class ObjectBehaviour {
public:

	Entity parent;

	bool isActive = true;

	virtual void Update() {}
	virtual void Start() {}
};
struct Transform : public ObjectBehaviour {
	Vector3 position = Vector3::zero();
	Vector3 eulerAngles = Vector3::zero();
	Vector3 scale = Vector3::one();
};

class EntityManager {
	// Queue of unused entity IDs
	std::queue<Entity> mAvailableEntities{};

	// Array of signatures where the index corresponds to the entity ID
	std::array<EntitySignature, MaxEntities> mSignatures{};

	// Total living entities - used to keep limits on how many exist
	uint32_t mLivingEntityCount{};
	template <class DstType, class SrcType>
	bool IsType(const SrcType* src)
	{
		return dynamic_cast<const DstType*>(src) != nullptr;
	}
public:
	EntityManager() {
		for (Entity e = 0; e < MaxEntities; e++) {
			mAvailableEntities.push(e);
		}
	}
	Entity CreateEntity() {
		assert(mLivingEntityCount < MaxEntities && "Max entity count reached!");

		Entity thisId = mAvailableEntities.front();
		mAvailableEntities.pop();
		mLivingEntityCount++;
		return thisId;
	}
	void DestroyEntity(Entity e) {
		assert(e < MaxEntities && "Entity out of range.");

		mSignatures[e].reset();
		mAvailableEntities.push(e);
		mLivingEntityCount--;
	}
	void SetEntitySignature(Entity e, EntitySignature es) {
		assert(e < MaxEntities && "Entity out of range.");
		mSignatures[e] = es;

	}
	EntitySignature GetSignature(Entity e)
	{
		assert(e < MaxEntities && "Entity out of range.");
		return mSignatures[e];
	}
};
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity e) = 0;
	virtual void UpdateBehaviour() {}
};
template <typename T>
class ComponentArray : public IComponentArray {
private:
	// Map from an entity ID to an array index.
	std::unordered_map<Entity, size_t> mEntityToIndexMap;

	// Map from an array index to an entity ID.
	std::unordered_map<size_t, Entity> mIndexToEntityMap;

	// Total size of valid entries in the array.
	size_t mSize;

	std::array<T, MaxEntities> mComponentArray;
public:
	void UpdateBehaviour() override {

		for (int i = 0; i < mSize; i++) {
			if(mComponentArray[i].isActive)
				mComponentArray[i].Update();
		}
	}
	void InsertData(Entity e, T component) {
		assert(mEntityToIndexMap.find(e) == mEntityToIndexMap.end() && "Component added to same entity more than once.");
		
		size_t newIndex = mSize;
		mEntityToIndexMap[e] = newIndex;
		mIndexToEntityMap[newIndex] = e;
		mComponentArray[newIndex] = component;
		
		if (component.isActive) {
			component.Start();
			component.parent = e;
		}
		mSize++;
	}
	void RemoveData(Entity e) {
		assert(mEntityToIndexMap.find(e) == mEntityToIndexMap.end() && "Removing non-existent component!");

		size_t mIndexOfRemovedEntity = mEntityToIndexMap[e];
		size_t mIndexOfLastElement = mSize - 1;
		mComponentArray[mIndexOfRemovedEntity] = mComponentArray[mIndexOfLastElement];

		//Update map
		Entity lastElementEntity = mIndexToEntityMap[mIndexOfLastElement];
		mEntityToIndexMap[lastElementEntity] = mIndexOfRemovedEntity;
		mIndexToEntityMap[mIndexOfRemovedEntity] = lastElementEntity;

		mEntityToIndexMap.erase(e);
		mIndexToEntityMap.erase(mIndexOfLastElement);

		mSize--;
	}
	T& GetData(Entity e) {
		assert(mEntityToIndexMap.find(e) != mEntityToIndexMap.end() && "Retrieving nonexistent entity!");
		return mComponentArray[mEntityToIndexMap[e]];
	}
	void EntityDestroyed(Entity e) override {
		if (mEntityToIndexMap.find(e) != mEntityToIndexMap.end())
		{
			RemoveData(e);
		}
	}
};
class ComponentManager {
private:
	// Map from type string pointer to a component type
	std::unordered_map<const char*, ComponentType> mComponentTypes{};

	// Map from type string pointer to a component array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

	// The component type to be assigned to the next registered component - starting at 0
	ComponentType mNextComponentType{};

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		if (mComponentTypes.find(typeName) == mComponentTypes.end()) {
			RegisterComponent<T>();
		}

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
	template <typename SrcType, typename T>
	bool IsType(const SrcType* src)
	{
		return (dynamic_cast<T>(src) != NULL);
	}
public:
	template <typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		mComponentTypes.insert({typeName, mNextComponentType});

		mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		mNextComponentType++;
	}
	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return mComponentTypes[typeName];
	}
	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		if (typeid(T) == typeid(Transform)) {
			Debug::Warning("Cannot remove transform from component.");
			return; 
		}
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}
	void Update() {
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->UpdateBehaviour();
		}
	}
};

class EntitySystem {
public:
	std::set<Entity> mEntities;
};
class EntitySystemManager {
private:
	// Map from system type string pointer to a signature
	std::unordered_map<const char*, EntitySignature> mSignatures{};

	// Map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<EntitySystem>> mSystems{};
public:
	template <typename T>
	std::shared_ptr<T> RegisterEntitySystem() {
		const char* typeName = typeid(T).name;
		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		auto system = std::make_shared<T>();
		mSystems.insert({ typeName, system });
		return system;
	}
	template <typename T>
	void SetSignature(EntitySignature es) {
		const char* typeName = typeid(T).name;
		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");
		mSignatures.insert({ typeName, es });
	}
	void EntityDestroyed(Entity e) {
		for (auto const& pair : mSystems) {
			auto const& system = pair.second;
			system->mEntities.erase(e);
		}
	}
	void EntitySignatureChanged(Entity e, EntitySignature es) {
		for (auto const& pair : mSystems) {

			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			if ((es & systemSignature) == systemSignature) {
				system->mEntities.insert(e);
			}
			else { system->mEntities.erase(e); }
		}
	}
};
class ObjectSystem {
private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<EntitySystemManager> mSystemManager;
public:
	void Init() {
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<EntitySystemManager>();
	}
	void Update() {
		mComponentManager->Update();
	}
	Entity CreateEntity(Transform t = Transform{}) {
		Entity e = mEntityManager->CreateEntity();
		mComponentManager->AddComponent<Transform>(e, t);
		return e;
	}
	void DestroyEntity(Entity e)
	{
		mEntityManager->DestroyEntity(e);

		mComponentManager->EntityDestroyed(e);

		mSystemManager->EntityDestroyed(e);
	}
	template <typename T>
	void RegisterComponent() {
		mComponentManager->RegisterComponent<T>();
	}
	template<typename T>
	void AddComponent(Entity e, T component)
	{
		mComponentManager->AddComponent<T>(e, component);

		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetEntitySignature(e, signature);

		mSystemManager->EntitySignatureChanged(e, signature);
	}
	template<typename T>
	void RemoveComponent(Entity e)
	{
		mComponentManager->RemoveComponent<T>(e);

		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetEntitySignature(e, signature);

		mSystemManager->EntitySignatureChanged(e, signature);
	}
	template<typename T>
	T& GetComponent(Entity e)
	{
		return mComponentManager->GetComponent<T>(e);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}
	// System methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterEntitySystem<T>();
	}

	template<typename T>
	void SetSystemSignature(EntitySignature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}
};
#endif