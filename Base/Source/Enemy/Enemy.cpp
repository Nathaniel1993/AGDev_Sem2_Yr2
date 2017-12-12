#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CEnemy::CEnemy()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundry(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundry(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(10.0f, 0.0f, 0.0f);
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundry
	maxBoundry.Set(1, 1, 1);
	minBoundry.Set(-1, -1, -1);

	// Set Speed
	m_dSpeed = 1.0;

	// Initialise the LOD meshes
	InitLOD("cube", "sphere", "cubeSG");

	// Initialise the collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	// Add to the EntityManager
	CSpatialPartition::GetInstance()->Add(this);

}
// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void CEnemy::SetTarget(const Vector3 & target)
{
	this->target = target;
}

void CEnemy::SetUp(const Vector3 up)
{
	this->up = up;
}

void CEnemy::SetBoundry(Vector3 max, Vector3 min)
{
	maxBoundry = max;
	minBoundry = min;
}

void CEnemy::SetTerrain(GroundEntity * m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundry(this->m_pTerrain->GetMaxBoundary(), 
			this->m_pTerrain->GetMinBoundary());
	}
}

Vector3 CEnemy::GetPos(void) const
{
	return position;
}

Vector3 CEnemy::GetTarget(void) const
{
	return target;
}

Vector3 CEnemy::GetUp(void) const
{
	return up;
}

GroundEntity * CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

void CEnemy::Update(double dt)
{
	Vector3 veiwVector = (target - position).Normalized();
	position += veiwVector * (float)m_dSpeed * (float)dt;

	// Constrain the position
	Constrain();

	// Update the target
	if (position.z > 400.f)
		target.z > position.z * -1;
	else if (position.z < -400.f)
		target.z = position.y * 1;
}

void CEnemy::Constrain(void)
{
	// Constrain enemy within the boundry
	if (position.x > maxBoundry.x - 1.0f)
		position.x = maxBoundry.x - 1.0f;
	if (position.z > maxBoundry.z - 1.0f)
		position.z = maxBoundry.z - 1.0f;
	
	if (position.x < minBoundry.x + 1.0f)
		position.x = minBoundry.x + 1.0f;
	if (position.z < minBoundry.z + 1.0f)
		position.z = minBoundry.z + 1.0f;

	// if the y position is not equal to terrain height at that position
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);

}

void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}
