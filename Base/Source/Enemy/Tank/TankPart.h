#ifndef TANK_PART_H
#define TANK_PART_H
#include "../Enemy.h"
#include "../../SceneGraph/SceneGraph.h"

class CTankPart : public CEnemy
{
public:
	CTankPart(void);
	~CTankPart(void);
	void Init(void);

	CSceneNode* TankPartNode;
};

#endif TANK_H