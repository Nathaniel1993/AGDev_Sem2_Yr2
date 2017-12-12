#include "Grid.h"
#include "stdio.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "Collider/Collider.h"
#include "../Projectile\Laser.h"
#include "../SceneGraph\SceneGraph.h"

CGrid::CGrid(void)
	: index(Vector3(-1, -1, -1))
	, size(Vector3(-1, -1, -1))
	, offset(Vector3(-1, -1, -1))
	, min(Vector3(-1, -1, -1))
	, max(Vector3(-1, -1, -1))
	, theMesh(NULL)
	, ListOfObjects(NULL)
	, theDetailLevel(CLevelOfDetails::NO_DETAILS)
{
}

CGrid::~CGrid(void)
{
	if (theMesh)
	{
		delete theMesh;
		theMesh = NULL;
	}
	Remove();
}

// Init
void CGrid::Init(	const int xIndex, const int zIndex,
					const int xGridSize, const int zGridSize,
					const float xOffset, const float zOffset)
{
	index.Set(xIndex, 0, zIndex);
	size.Set(xGridSize, 0, zGridSize);
	offset.Set(xOffset, 0, zOffset);
	min.Set(index.x * size.x - offset.x, 0.0f, index.z * size.z - offset.z);
	max.Set(index.x * size.x - offset.x + xGridSize, 0.0f, index.z * size.z - offset.z + zGridSize);
}

// Set a particular grid's Mesh
void CGrid::SetMesh(const std::string& _meshName)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh != nullptr)
	{
		theMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	}
}

/********************************************************************************
Update the grid
********************************************************************************/
void CGrid::Update(vector<EntityBase*>* migrationList, double _dt)
{
	// Update all entities
	std::vector<EntityBase*>::iterator it;
	it = ListOfObjects.begin();
	while (it != ListOfObjects.end())
	{
		(*it)->Update(_dt);
		++it;
	}

	// Check each object to see if they are no longer in this grid
	it = ListOfObjects.begin();
	while (it != ListOfObjects.end())
	{
		Vector3 position = (*it)->GetPosition();
		if (((min.x >= position.x) || (position.x >= max.x)) ||
			((min.z >= position.z) || (position.z >= max.z)))
		{
			migrationList->push_back(*it);

			// Remove from this Grid
			it = ListOfObjects.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render the grid
void CGrid::Render(void)
{
	if (theMesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		RenderHelper::RenderMesh(theMesh);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

// RenderObjects
void CGrid::RenderObjects(void)
{
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		ListOfObjects[i]->Render();
	}
}

/********************************************************************************
Add a new object to this grid
********************************************************************************/
void CGrid::Add(EntityBase* theObject)
{
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		if (ListOfObjects[i] == theObject)
			return;
	}
	ListOfObjects.push_back( theObject );
}

/********************************************************************************
 Remove but not delete object from this grid
********************************************************************************/
void CGrid::Remove(void)
{
	for( int i = 0 ; i < ListOfObjects.size(); i++)
	{
		// Do not delete the objects as they are stored in EntityManager and will be deleted there.
		delete ListOfObjects[i];
		ListOfObjects[i] = NULL;
	}
	ListOfObjects.clear();
}

// Remove but not delete an object from this grid
bool CGrid::Remove(EntityBase* theObject)
{
	// Clean up entities that are done
	std::vector<EntityBase*>::iterator it, end;
	it = ListOfObjects.begin();
	end = ListOfObjects.end();
	while (it != end)
	{
		if ((*it) == theObject)
		{
			delete (*it);
			(*it) = NULL;
			it = ListOfObjects.erase(it);
			return true;
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
	return false;
}

// Check if an object is in this grid
bool CGrid::IsHere(EntityBase* theObject) const
{
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		if (ListOfObjects[i] == theObject)
			return true;
	}
	return false;
}

// Get list of objects in this grid
vector<EntityBase*> CGrid::GetListOfObject(void)
{
	return ListOfObjects;
}

void CGrid::SetObjectsToCheck(vector<EntityBase*> ToCheck)
{
	ObjectsToCheck = ToCheck;
}

// PrintSelf
void CGrid::PrintSelf()
{
	cout << "CGrid::PrintSelf()" << endl;
	cout << "\tIndex\t:\t" << index << "\t\tOffset\t:\t" << offset << endl;
	cout << "\tMin\t:\t" << min << "\tMax\t:\t" << max << endl;
	if (ListOfObjects.size() > 0)
	{
		cout << "\tList of objects in this grid: (LOD:" <<this->theDetailLevel << ")" << endl;
		cout << "\t------------------------------------------------------------------------" << endl;
	}
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		cout << "\t" << i << "\t:\t" << ListOfObjects[i]->GetPosition() << endl;
	}
	if (ListOfObjects.size()>0)
		cout << "\t------------------------------------------------------------------------" << endl;
	cout << "********************************************************************************" << endl;
}

void CGrid::SetDetailLevel(const CLevelOfDetails::DETAIL_LEVEL theDetailLevel)
{
	this->theDetailLevel = theDetailLevel;

	//Check each object to see if they are no longer in this grid
	std::vector<EntityBase*>::iterator it;
	it = ListOfObjects.begin();
	while (it != ListOfObjects.end())
	{
		GenericEntity* theEntity = (GenericEntity*)(*it);
		if (theEntity->GetLODStatus() == true)
		{
			theEntity->SetDetailLevel(theDetailLevel);
		}
		++it;
	}
}

void CGrid::DeleteDone()
{
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		if (ListOfObjects[i]->IsDone())
		{
			Remove(ListOfObjects[i]);
		}
	}
}

// Check for overlap
bool CGrid::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool CGrid::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool CGrid::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
	//	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
	//	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

bool CGrid::GetIntersection(const float fDist1, const float fDist2, Vector3 P1, Vector3 P2, Vector3 & Hit)
{
	if ((fDist1 * fDist2) >= 0.f)
		return false;
	if (fDist1 == fDist2)
		return false;
	Hit = P1 + (P2 - P1) * (-fDist1 / (fDist2 - fDist1));
	return true;
}

bool CGrid::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 & Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x,
		line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y -
			minAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z -
			minAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x -
			maxAABB.x, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y -
			maxAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z -
			maxAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

bool CGrid::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool CGrid::CheckForCollision(void)
{
	// Check for Collision
	std::vector<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::vector<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = ObjectsToCheck.end();
	for (colliderThis = ObjectsToCheck.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		// Check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			// Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = ObjectsToCheck.end();
			int counter = 0;
			for (colliderThat = ObjectsToCheck.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;
				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPosition = Vector3(0.f, 0.f, 0.f);

					// Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (CheckLineSegmentPlane(thisEntity->GetPosition(),
						thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
						thatMinAABB, thatMaxAABB,
						hitPosition) == true)
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);

						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
						{
							cout << "*** This Entity removed ***" << endl;
						}
						//Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
						{
							cout << "*** That Entity removed ***" << endl;
						}
					}
				}

			}
		}
		else if ((*colliderThis)->HasCollider())
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = ObjectsToCheck.end();
			int counter = 0;
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					// This object was derived from a CCollider class, then it will have Collision Detection methods
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity) == true)
					{
						if (CheckAABBCollision(thisEntity, thatEntity) == true)
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);

							// Remove from Scene Graph
							if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
							{
								cout << "*** This Entity removed ***" << endl;
							}
							//Remove from Scene Graph
							if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
							{
								cout << "*** That Entity removed ***" << endl;
							}
						}
					}
				}
			}
		}
	}
	return false;
}