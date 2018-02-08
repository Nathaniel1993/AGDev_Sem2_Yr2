#ifndef TANK_PART_H
#define TANK_PART_H
#include "../Enemy.h"
#include "../../SceneGraph/SceneGraph.h"

class CTankPart : public CEnemy
{
	std::string Name;
public:
	CTankPart(void);
	~CTankPart(void);
	void Init(void);

	CSceneNode* TankPartNode;
	Vector3 PartOffset;
	std::string GetName() { return Name; }
	void SetName(std::string _name) { Name = _name; }

	void Render(void);
};

#endif TANK_H