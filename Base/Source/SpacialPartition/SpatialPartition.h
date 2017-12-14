#pragma once
#include "Vector3.h"
#include "Grid.h"
#include "EntityBase.h"
#include "../FPSCamera.h"

class Mesh;

class CSpatialPartition
{
protected:
	static CSpatialPartition *sp_instance;
	// Constructor
	CSpatialPartition(void);

	// Variables 
	CGrid* theGrid;
	int xSize;
	int zSize;
	int xGridSize;
	int zGridSize;
	int xNumOfGrid;
	int zNumOfGrid;
	float yOffset;
	std::string _meshName;

	FPSCamera* theCamera;
	float LevelOfDetails_Distances[2];

	int selectedGrid;
	int prevGrid;
	bool gridChanged;
	Mesh* selectedGridMesh;

public:
	static CSpatialPartition *GetInstance()
	{
		if (!sp_instance)
			sp_instance = new CSpatialPartition;
		return sp_instance;
	}

	static bool DropInstance()
	{
		if (sp_instance)
		{
			delete sp_instance;
			sp_instance = NULL;
			return true;
		}
		return false;
	}
	~CSpatialPartition(void);

	// Initialise the spatial partition
	bool Init(const int xGridSize, const int zGridSize,
		const int xNumOfGrid, const int zNumOfGrid,
		const float yOffset = -9.9f);

	// Set a particular grid's Mesh
	void SetMesh(const std::string& _meshName);
	// Set selected grid's Mesh
	void SetSelectedGridMesh(const std::string& _meshName);

	// Apply Mesh
	void ApplyMesh(void);

	// Update the spatial partition
	void Update(double _dt);
	// Render the spatial partition
	void Render(Vector3* theCameraPosition = NULL);

	// Get xSize of the entire spatial partition
	int GetxSize(void) const;
	// Get zSize of the entire spatial partition
	int GetzSize(void) const;
	// Get xSize of one Grid
	int GetxGridSize(void) const;
	// Get zSize of one Grid
	int GetzGridSize(void) const;
	// Get xNumOfGrid
	int GetxNumOfGrid(void) const;
	// Get zNumOfGrid
	int GetzNumOfGrid(void) const;

	// Get a particular grid
	CGrid GetGrid(const int xIndex, const int zIndex) const;

	// Get vector of objects from this spatial partition
	vector<EntityBase*> GetObjects(Vector3 position, const float radius);

	// Add a new object
	void Add(EntityBase* theObject);
	// Remove but not delete object from this grid
	void Remove(EntityBase* theObject);

	// Calculate the square distance from camera to a grid's centrepoint
	float CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int zIndex);

	// PrintSelf
	void PrintSelf() const;

	// The vector of objects due for migration to another grid
	vector<EntityBase*> MigrationList;

	// Handling Camera
	void SetCamera(FPSCamera* _camerPtr);
	void RemoveCamera(void);

	// Set LOD distances
	void SetLevelOfDetails(const float distance_High2Mid, const float distance_Mid2Low);
	// Check if a CGrid is visible to the camera
	bool IsVisible(Vector3 theCameraPosition, Vector3 theCameraDirection, const int xIndex, const int zIndex);

	void GridSelection();
};