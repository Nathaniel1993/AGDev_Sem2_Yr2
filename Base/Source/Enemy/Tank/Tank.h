#ifndef TANK_H
#define TANK_H
#include "../Enemy.h"
#include "../../SceneGraph/SceneGraph.h"
#include "../EnemyStates/StateMachine.h"
#include "TankPart.h"

class CTank : public CEnemy
{
	int ID;
	int currWayPoint;
public:
	CTank(void);
	~CTank(void);
	void Init(void);

	CSceneNode* TankNode;
	StateMachine *sm;
	Vector3 PrevNode;
	Vector3 NextNode;
	Vector3 PlayerPos;
	float Speed;

	int GetTankID() { return ID; }
	void SetTankID(int _id) { ID = _id; }
	int GetCurrWaypoint() { return currWayPoint; }
	void SetCurrWaypoint(int _waypoint) { currWayPoint = _waypoint; }

	std::vector<CTankPart*> ChildList;

	void Render(void);
};

#endif TANK_H