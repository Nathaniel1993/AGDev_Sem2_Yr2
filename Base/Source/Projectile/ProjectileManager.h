#ifndef PROJECTILE_MANAGER_H
#define PROJECTILE_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "../SpacialPartition\SpatialPartition.h"

class CProjectile;

class ProjectileManager : public Singleton<ProjectileManager>
{
	friend Singleton<ProjectileManager>;
public:
	void Update(double _dt);
	void Render();

	void AddProjectile(CProjectile* _newProj);
	bool RemoveProjectile(CProjectile* _existingProj);
	// Mark a projectile for deletion
	bool MarkForDeletion(CProjectile* _existingProj);

	// Set a handle to the Spatial Partition to this class
	void SetSpatialPartition(CSpatialPartition* theSpatialPartition);

private:
	ProjectileManager();
	virtual ~ProjectileManager();
};


#endif