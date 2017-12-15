//#pragma once
//#include "../SceneGraph/SceneNode.h"
//#include"../SceneGraph/SceneGraph.h"
//#include "../Enemy/Enemy.h"
//#include "../EntityManager.h"
//
//class TankMain : public CSceneNode, CEnemy
//{
//protected:
//	CEnemy* threadChild1, *threadChild2, *turretChild1, *turretChild2;
//	Vector3 defaultPosition, defaultTarget, defaultUp;
//	Vector3 target, up, position, scale;
//	Vector3 maxBoundry, minBoundry;
//	GroundEntity* m_pTerrain;
//
//	double m_dSpeed;
//	double m_dAcceleration;
//
//public:
//	TankMain(void);
//	TankMain(Vector3 _mainPosition);
//	virtual ~TankMain();
//
//	void Init(void);
//	//void SetTerrain(GroundEntity* m_pTerrain);
//	
//	void SetChild(CEnemy* _theChild);
//	void SetChild2(CEnemy* _theChild2);
//	void SetChild3(CEnemy* _theChild3);
//	void SetChild4(CEnemy* _theChild3);
//	
//	CEnemy* GetChild();
//	CEnemy* Getchild2();
//	CEnemy* Getchild3();
//	CEnemy* Getchild4();
//
//	void SetPosition(Vector3 _position);
//	void SetScale(Vector3 _scale);
//	Vector3 GetPosition();
//	Vector3 GetScale();
//};