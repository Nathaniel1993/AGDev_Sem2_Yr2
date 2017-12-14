#pragma once

#include "EntityBase.h"
#include "Vector3.h"
#include <vector>
using namespace std;

//Include GLEW
#include <GL/glew.h>

#include "../LevelOfDetails/LevelOfDetails.h"
#include "../GenericEntity.h"

class Mesh;

class CGrid
{
protected:
	// We use a Vector3 to store the indices of this Grid within the Spatial Partition array.
	Vector3 index;
	// We use a Vector3 to store the size of this Grid within the Spatial Partition array.
	Vector3 size;
	// We use a Vector3 to store the x- and z-offset of this Grid.
	Vector3 offset;
	// We use a Vector3 to store the x- and z-offset of this Grid.
	Vector3 min, max;

	// The mesh to represent the grid
	Mesh* theMesh;

	// List of objects in this grid
	vector<EntityBase*> ListOfObjects;

	vector<EntityBase*> ObjectsToCheck;

	CLevelOfDetails::DETAIL_LEVEL theDetailLevel;

	//=============================================================================
	//==================================COLLISION==================================
	//=============================================================================
	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);

	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);

	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);

	// Check for intersection between a line segment and a plane
	bool GetIntersection(const float fDist1, const float fDist2, Vector3 P1, Vector3 P2, Vector3 &Hit);

	// Check for intersection between a line segment and a plane
	bool CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 &Hit);

	// Check two positions are within a box region
	bool InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);

public:
	// Constructor
	CGrid(void);
	// Destructor
	~CGrid(void);

	// Init
	void Init(	const int xIndex, const int zIndex, 
				const int xGridSize, const int zGridSize,
				const float xOffset = 0, const float zOffset = 0);

	// Set a particular grid's Mesh
	void SetMesh(const std::string& _meshName);

	// Update the grid
	void Update(vector<EntityBase*>* migrationList, double _dt);
	// Render the grid
	void Render(void);
	// RenderObjects
	void RenderObjects(void);

	// Add a new object to this grid
	void Add(EntityBase* theObject);
	// Remove but not delete all objects from this grid
	void Remove(void);
	// Remove but not delete an object from this grid
	bool Remove(EntityBase* theObject);

	// Check if an object is in this grid
	bool IsHere(EntityBase* theObject) const;

	// Get list of objects in this grid
	vector<EntityBase*> GetListOfObject(void);

	void SetObjectsToCheck(vector<EntityBase*> ToCheck);

	// PrintSelf
	void PrintSelf();

	// Set the Level of Detail for objects in this CGrid
	void SetDetailLevel(const CLevelOfDetails::DETAIL_LEVEL theDetailLevel);

	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(void);

	// Delete entities that are done
	void DeleteDone();
};
