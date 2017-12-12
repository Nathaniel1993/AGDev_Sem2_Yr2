#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "SpacialPartition\SpatialPartition.h"

class EntityBase;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition = false);
	bool RemoveEntity(EntityBase* _existingEntity);
	//  Mark an entity for deletion
	bool MarkForDeletion(EntityBase* _existingEntity);

	// Set a handle to the Spatial Partition to this class
	void SetSpatialPartition(CSpatialPartition* theSpatialPartition);

private:
	EntityManager();
	virtual ~EntityManager();

	std::list<EntityBase*> entityList;

	// Handler to Spatial Partition
	CSpatialPartition* theSpatialPartition;
};

#endif // ENTITY_MANAGER_H