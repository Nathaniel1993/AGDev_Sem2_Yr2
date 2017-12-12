#ifndef FURNITURE_H
#define FURNITURE_H
#include "Collider\Collider.h"
#include "../EntityManager.h"
#include "../GenericEntity.h"
#include "../GroundEntity.h"
using namespace std;

class CFurniture : public GenericEntity
{
public:
	CFurniture(Mesh* _modelMesh);
	~CFurniture();

	//// Set the maxAABB and minAABB
	// void SetAABB(Vector3 maxAABB, Vector3 minAABB);
	//// Set the maxAABB
	// void SetMaxAABB(Vector3 maxAABB);
	//// Get the maxAABB
	// Vector3 GetMaxAABB(void);
	//// Set the minAABB
	// void SetMinAABB(Vector3 minAABB);
	//// Get the minAABB
	// Vector3 GetMinAABB(void);

	 // Render
	 void Render(void);

	

protected:
	// Vector3 variables to store coordinates of the AABB bounding box of an object
	/*Vector3 maxAABB;
	Vector3 minAABB*/;

	Mesh* modelMesh;

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;
	//float angle;
};
namespace Create
{
	CFurniture* Furniture3D(const std::string& _meshName,
		const Vector3& _position,
		const float& _angle,
		const Vector3& _rotation,
		const Vector3& _scale = Vector3(2.0f, 2.0f, 2.0f));
}
#endif // ! FURNITURE_H
