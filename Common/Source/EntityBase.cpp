#include "EntityBase.h"

EntityBase::EntityBase()
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, isDone(false)
	, m_bCollider(false)
	, bLaser(false)
	, bSelector(false)
	, selected_Grid(false)
	, selected_Gun(false)
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::Update(double _dt)
{
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone(bool _value)
{
	isDone = _value;
}

// Check if this entity has a collider class parent
bool EntityBase::HasCollider(void) const
{
	return m_bCollider;
}

// Set the flag to indicate if this entity has a collider class parent
void EntityBase::SetCollider(const bool _value)
{
	m_bCollider = _value;
}

// Set the flag, bLaser
void EntityBase::SetIsLaser(const bool bLaser)
{
	this->bLaser = bLaser;
}
// Get the flag, bLaser
bool EntityBase::GetIsLaser(void) const
{
	return bLaser;
}

void EntityBase::SetIsSelector(const bool bSelector)
{
	this->bSelector = bSelector;
}

bool EntityBase::GetIsSelector(void) const
{
	return this->bSelector;
}

void EntityBase::SetSelectedGrid(bool _selectedGrid)
{
	this->selected_Grid = _selectedGrid;
}

void EntityBase::SetSelectedGun(bool _selectedGun)
{
	this->selected_Gun = _selectedGun;
}

bool EntityBase::GetSelectedGrid()
{
	return this->selected_Grid;
}

bool EntityBase::GetSelectedGun()
{
	return this->selected_Gun;
}