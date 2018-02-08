#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph\UpdateTransformation.h"
#include "Enemy\Enemy.h"
#include "Lua\LuaInterface.h"
#include "Waypoint\WaypointManager.h"
#include <vector>

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class CTank;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	// SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[5];
	Light* lights[2];

	GenericEntity* theCube;

	typedef std::vector<CTank*>tankList;
	tankList theTanks_;
	int walkDist;
	double elapsedTime, bounceTime;
	std::vector<int>ammoCount;
	bool ammoFilled;

	//std::vector<EntityManager*>entityList;


	// static SceneText* sInstance; // The pointer to the object that gets registered
};

#endif