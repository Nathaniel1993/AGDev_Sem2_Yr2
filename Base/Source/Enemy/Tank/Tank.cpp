#include "Tank.h"
#include "TankPart.h"
#include "../../SpacialPartition/SpatialPartition.h"

CTank::CTank(void)
{
}

CTank::~CTank(void)
{
}

void CTank::Init(void)
{
	// Initialise self
	// Set the default values
	this->defaultPosition.Set(0, 0, 10);
	this->defaultTarget.Set(0, 0, 0);
	this->defaultUp.Set(0, 1, 0);
	// Set the current values
	this->target.Set(10.0f, 0.0f, 450.0f);
	this->up.Set(0.0f, 1.0f, 0.0f);
	// Set Boundry
	this->maxBoundry.Set(1000.f, 1000.f, 1000.f);
	this->minBoundry.Set(-1000.f, -1000.f, -1000.f);
	// Set Speed
	this->m_dSpeed = 0.0;
	this->SetScaleEntity(Vector3(5.f, 5.f, 7.f));
	// Initialise the collider
	this->SetCollider(true);
	this->SetAABB(Vector3(this->GetScaleEntity().x / 2.f,
						this->GetScaleEntity().y / 2.f,
						this->GetScaleEntity().z / 2.f),
				Vector3(-(this->GetScaleEntity().x / 2.f),
					-(this->GetScaleEntity().y / 2.f),
					-(this->GetScaleEntity().z / 2.f)));
	this->InitLOD("obj_cube", "sphere", "cubeSG");
	TankNode = CSceneGraph::GetInstance()->AddNode(this);
	// Add to the EntityManager
	CSpatialPartition::GetInstance()->Add(this);

	//=====================================================================================
	// Left Thread
	CTankPart* ThreadLeft = new CTankPart();
	ThreadLeft->Init();
	ThreadLeft->SetScaleEntity(Vector3(2.5f, 2.5f, 5.f));
	ThreadLeft->SetPosition(Vector3(this->GetPosition().x - 5.f,
									this->GetPosition().y - 2.f, 
									this->GetPosition().z + 1.f));
	ThreadLeft->SetAABB(Vector3(ThreadLeft->GetScaleEntity().x / 2.f, 
								ThreadLeft->GetScaleEntity().y / 2.f, 
								ThreadLeft->GetScaleEntity().z / 2.f),
						Vector3(-(ThreadLeft->GetScaleEntity().x / 2.f), 
								-(ThreadLeft->GetScaleEntity().y / 2.f), 
								-(ThreadLeft->GetScaleEntity().z / 2.f)));
	ThreadLeft->InitLOD("obj_cube", "sphere", "cubeSG");
	ThreadLeft->TankPartNode = this->TankNode->AddChild(ThreadLeft);
	CSpatialPartition::GetInstance()->Add(ThreadLeft);

	// Right Thread
	CTankPart* ThreadRight = new CTankPart();
	ThreadRight->Init();
	ThreadRight->SetScaleEntity(Vector3(2.5f, 2.5f, 5.f));
	ThreadRight->SetPosition(Vector3(this->GetPosition().x + 5.f,
									this->GetPosition().y - 2.f,
									this->GetPosition().z + 1.f));
	ThreadRight->SetAABB(Vector3(ThreadRight->GetScaleEntity().x / 2.f,
								ThreadRight->GetScaleEntity().y / 2.f,
								ThreadRight->GetScaleEntity().z / 2.f),
						Vector3(-(ThreadRight->GetScaleEntity().x / 2.f),
							-(ThreadRight->GetScaleEntity().y / 2.f),
							-(ThreadRight->GetScaleEntity().z / 2.f)));
	ThreadRight->InitLOD("obj_cube", "sphere", "cubeSG");
	ThreadRight->TankPartNode = this->TankNode->AddChild(ThreadRight);
	CSpatialPartition::GetInstance()->Add(ThreadRight);

	// Turret
	CTankPart* Turret = new CTankPart();
	Turret->Init();
	Turret->SetScaleEntity(Vector3(4.f, 4.f, 4.f));
	Turret->SetPosition(Vector3(this->GetPosition().x,
								this->GetPosition().y + 5.f,
								this->GetPosition().z + 1.5f));
	Turret->SetAABB(Vector3(Turret->GetScaleEntity().x / 2.f,
							Turret->GetScaleEntity().y / 2.f,
							Turret->GetScaleEntity().z / 2.f),
					Vector3(-(Turret->GetScaleEntity().x / 2.f),
						-(Turret->GetScaleEntity().y / 2.f),
						-(Turret->GetScaleEntity().z / 2.f)));
	Turret->InitLOD("obj_cube", "sphere", "cubeSG");
	Turret->TankPartNode = this->TankNode->AddChild(Turret);
	CSpatialPartition::GetInstance()->Add(Turret);
}