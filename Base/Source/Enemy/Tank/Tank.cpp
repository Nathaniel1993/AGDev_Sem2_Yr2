#include "Tank.h"
#include "TankPart.h"
#include "../../SpacialPartition/SpatialPartition.h"
#include "../../Enemy/EnemyStates/StatesTank.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CTank::CTank(void)
{
	this->position.SetZero();
	this->PrevNode.SetZero();
	this->NextNode.SetZero();
	this->sm = new StateMachine();
	sm->AddState(new StateTankChase("Chase", this));
	sm->AddState(new StateTankPatrol("Patrol", this));
	sm->SetNextState("Patrol");
}

CTank::~CTank(void)
{
	delete sm;
	sm = NULL;
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
	this->InitLOD("obj_cube", "cube", "cube");
	// Add to the EntityManager
	CSpatialPartition::GetInstance()->Add(this);

	//=====================================================================================
	// Left Thread
	CTankPart* ThreadLeft = new CTankPart();
	ThreadLeft->Init();
	ThreadLeft->SetScaleEntity(Vector3(2.5f, 2.5f, 5.f));
	ThreadLeft->PartOffset.Set(-5.f, -2.f, 1.f);
	ThreadLeft->SetPosition(Vector3(this->position.x + ThreadLeft->PartOffset.x, 
									this->position.y + ThreadLeft->PartOffset.y,
									this->position.z + ThreadLeft->PartOffset.z));
	ThreadLeft->SetAABB(Vector3(ThreadLeft->GetScaleEntity().x / 2.f, 
								ThreadLeft->GetScaleEntity().y / 2.f, 
								ThreadLeft->GetScaleEntity().z / 2.f),
						Vector3(-(ThreadLeft->GetScaleEntity().x / 2.f), 
								-(ThreadLeft->GetScaleEntity().y / 2.f), 
								-(ThreadLeft->GetScaleEntity().z / 2.f)));
	ThreadLeft->InitLOD("obj_cube", "cube", "cube");
	ThreadLeft->SetName("ThreadLeft");
	ThreadLeft->TankPartNode = this->TankNode->AddChild(ThreadLeft);
	CSpatialPartition::GetInstance()->Add(ThreadLeft);
	this->ChildList.push_back(ThreadLeft);

	// Right Thread
	CTankPart* ThreadRight = new CTankPart();
	ThreadRight->Init();
	ThreadRight->SetScaleEntity(Vector3(2.5f, 2.5f, 5.f));
	ThreadRight->PartOffset.Set(5.f, -2.f, 1.f);
	ThreadRight->SetPosition(Vector3(this->position.x + ThreadRight->PartOffset.x,
									this->position.y + ThreadRight->PartOffset.y,
									this->position.z + ThreadRight->PartOffset.z));
	ThreadRight->SetAABB(Vector3(ThreadRight->GetScaleEntity().x / 2.f,
								ThreadRight->GetScaleEntity().y / 2.f,
								ThreadRight->GetScaleEntity().z / 2.f),
						Vector3(-(ThreadRight->GetScaleEntity().x / 2.f),
							-(ThreadRight->GetScaleEntity().y / 2.f),
							-(ThreadRight->GetScaleEntity().z / 2.f)));
	ThreadRight->InitLOD("obj_cube", "cube", "cube");
	ThreadRight->SetName("ThreadRight");
	ThreadRight->TankPartNode = this->TankNode->AddChild(ThreadRight);
	CSpatialPartition::GetInstance()->Add(ThreadRight);
	this->ChildList.push_back(ThreadRight);

	// Turret
	CTankPart* Turret = new CTankPart();
	Turret->Init();
	Turret->SetScaleEntity(Vector3(4.f, 4.f, 4.f));
	Turret->PartOffset.Set(0.f, 5.f, 1.5f);
	Turret->SetPosition(Vector3(this->position.x + Turret->PartOffset.x,
						this->position.y + Turret->PartOffset.y,
						this->position.z + Turret->PartOffset.z));
	Turret->SetAABB(Vector3(Turret->GetScaleEntity().x / 2.f,
							Turret->GetScaleEntity().y / 2.f,
							Turret->GetScaleEntity().z / 2.f),
					Vector3(-(Turret->GetScaleEntity().x / 2.f),
						-(Turret->GetScaleEntity().y / 2.f),
						-(Turret->GetScaleEntity().z / 2.f)));
	Turret->InitLOD("obj_cube", "cube", "cube");
	Turret->SetName("Turret");
	Turret->TankPartNode  = this->TankNode->AddChild(Turret);
	CSpatialPartition::GetInstance()->Add(Turret);
	this->ChildList.push_back(Turret);
}

void CTank::Render(void)
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