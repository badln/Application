#include "Entity.h"
std::unique_ptr<ComponentManager> Object::mComponentManager;
std::unique_ptr<EntityManager> Object::mEntityManager;
std::unique_ptr<EntitySystemManager> Object::mSystemManager;
