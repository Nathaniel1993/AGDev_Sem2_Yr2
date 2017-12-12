#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"

class Mesh;

class CEnemy :
	public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundry, minBoundry;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

public:
	CEnemy(void);
	virtual ~CEnemy();

	void Init(void);
	// Reset this enemy instance to default
	void Reset(void);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3 up);
	// Set the boundry for enemy info
	void SetBoundry(Vector3 max, Vector3 min);
	// Set the terrain
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the enemy info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	//Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);


};