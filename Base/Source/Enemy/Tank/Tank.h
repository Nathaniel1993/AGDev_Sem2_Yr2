#ifndef TANK_H
#define TANK_H
#include "../Enemy.h"
#include "../../SceneGraph/SceneGraph.h"

class CTank : public CEnemy
{
public:
	CTank(void);
	~CTank(void);
	void Init(void);

	CSceneNode* TankNode;
};

#endif TANK_H