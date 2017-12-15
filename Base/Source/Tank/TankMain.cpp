//#include "TankMain.h"
//
//TankMain::TankMain(void)
//{
//}
//
//TankMain::TankMain(Vector3 _mainPosition) : defaultPosition(_mainPosition)
//{
//	
//}
//
//TankMain::~TankMain()
//{
//}
//
//void TankMain::Init(void)
//{
//	// Set the default values
//	defaultTarget.Set(0, 0, 0);
//	defaultUp.Set(0, 1, 0);
//
//	// Set the current values
//	position.Set(10.0f, 0.0f, 0.0f);
//	target.Set(10.0f, 0.0f, 450.0f);
//	up.Set(0.0f, 1.0f, 0.0f);
//
//	// Set Boundry
//	maxBoundry.Set(1, 1, 1);
//	minBoundry.Set(-1, -1, -1);
//
//	// Set Speed
//	m_dSpeed = 0.0;
//
//	// Initialise the LOD meshes
//	InitLOD("cube", "sphere", "cubeSG");
//
//	// Initialise the collider
//	this->SetCollider(false);
//	//this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
//
//	// Add to the EntityManager
//	CSpatialPartition::GetInstance()->Add(this);
//
//	CSceneNode* theEnemyNode = CSceneGraph::GetInstance()->AddNode(this);
//	threadChild1 = new CEnemy();
//	threadChild2 = new CEnemy();
//	turretChild1 = new CEnemy();
//	turretChild2 = new CEnemy();
//	
//	Vector3 threadOffset1 = Vector3(-5, -2, 1);
//	threadChild1->Init();
//	threadChild1->SetScale(Vector3(2.5f, 2.5f, 4.f));
//	threadChild1->SetPosition(defaultPosition + threadOffset1);
//	CSceneNode* threadNode1 = theEnemyNode->AddChild(threadChild1);
//	
//	Vector3 threadOffset2 = Vector3(5, -2, 1);
//	threadChild2->Init();
//	threadChild2->SetScale(Vector3(2.5f, 2.5f, 4.f));
//	threadChild2->SetPosition(defaultPosition + threadOffset2);
//	CSceneNode* threadNode2 = theEnemyNode->AddChild(threadChild2);
//	
//	Vector3 turretOffset1 = Vector3(0, 5, 1.5);
//	turretChild1->Init();
//	turretChild1->InitLOD("cubeSG", "sphere", "cube");
//	turretChild1->SetScale(Vector3(1.5f, 1.5f, 1.5f));
//	turretChild1->SetPosition(defaultPosition + turretOffset1);
//	CSceneNode* turretNode1 = theEnemyNode->AddChild(turretChild1);
//
//	Vector3 turretOffset2 = Vector3(0, 0, 5);
//	turretChild2->Init();
//	turretChild2->SetScale(Vector3(2.f, 2.f, 2.f));
//	turretChild2->SetPosition(defaultPosition + turretOffset2);
//	CSceneNode* turretNode2 = turretNode1->AddChild(turretChild2);
//	
//}
//
//void TankMain::SetChild(CEnemy * _theChild)
//{
//	this->threadChild1 = _theChild;
//}
//
//void TankMain::SetChild2(CEnemy * _theChild2)
//{
//	this->threadChild2 = _theChild2;
//}
//
//void TankMain::SetChild3(CEnemy * _theChild3)
//{
//	this->turretChild1 = _theChild3;
//}
//void TankMain::SetChild4(CEnemy * _theChild4)
//{
//	this->turretChild2 = _theChild4;
//}
//
//
//CEnemy * TankMain::GetChild()
//{
//	return threadChild1;
//}
//
//CEnemy * TankMain::Getchild2()
//{
//	return threadChild2;
//}
//
//CEnemy * TankMain::Getchild3()
//{
//	return turretChild1;
//}
//CEnemy * TankMain::Getchild4()
//{
//	return turretChild2;
//}
//void TankMain::SetPosition(Vector3 _position)
//{
//	this->position = _position;
//}
//
//void TankMain::SetScale(Vector3 _scale)
//{
//	this->scale = _scale;
//}
//
//Vector3 TankMain::GetPosition()
//{
//	return position;
//}
//
//Vector3 TankMain::GetScale()
//{
//	return scale;
//}
