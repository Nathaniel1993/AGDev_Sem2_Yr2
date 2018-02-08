#include "TankPart.h"
#include "../../SpacialPartition/SpatialPartition.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CTankPart::CTankPart(void)
{
	this->position.Set(0.f, 0.f, 0.f);
}

CTankPart::~CTankPart(void)
{
}

void CTankPart::Init(void)
{
	// Set the default values
	this->defaultPosition.Set(0, 0, 10);
	this->defaultTarget.Set(0, 0, 0);
	this->defaultUp.Set(0, 1, 0);

	// Set the current values
	//position.Set(10.0f, 0.0f, 0.0f);
	this->target.Set(10.0f, 0.0f, 450.0f);
	this->up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundry
	this->maxBoundry.Set(1000.f, 1.f, 1000.f);
	this->minBoundry.Set(-1000.f, -1.f, -1000.f);

	// Set Speed
	this->m_dSpeed = 0.0;

	// Initialise the collider
	this->SetCollider(true);

	// Add to the EntityManager
	CSpatialPartition::GetInstance()->Add(this);
}

void CTankPart::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			RenderHelper::RenderMesh(GetLODMesh(), selected_Grid, selected_Gun);
		}
	}
	modelStack.PopMatrix();
}
