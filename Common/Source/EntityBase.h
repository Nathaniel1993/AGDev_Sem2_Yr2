#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScaleEntity(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScaleEntity(){ return scale; };

	inline void SetID(const int id_) { id = id_; };
	inline int GetID() { return id; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);
	// Set the flag, bLaser
	virtual void SetIsLaser(const bool bLaser);
	// Get the flag, bLaser
	virtual bool GetIsLaser(void) const;

	virtual void SetIsSelector(const bool bSelector);
	virtual bool GetIsSelector(void) const;

	virtual void SetSelectedGrid(bool _selectedGrid);
	virtual void SetSelectedGun(bool _selectedGun);
	virtual bool GetSelectedGrid();
	virtual bool GetSelectedGun();

protected:
	Vector3 position;
	Vector3 scale;
	float angle;

	bool isDone;
	bool m_bCollider;
	bool bLaser;
	bool bSelector;

	bool selected_Grid;
	bool selected_Gun;

	int id;
};

#endif // ENTITY_BASE_H