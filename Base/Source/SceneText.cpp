#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"

#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//MultiTexture.fragmentshader");

	// Tell the shader program to store these uniform locations
	{
		currProg->AddUniform("MVP");
		currProg->AddUniform("MV");
		currProg->AddUniform("MV_inverse_transpose");
		currProg->AddUniform("material.kAmbient");
		currProg->AddUniform("material.kDiffuse");
		currProg->AddUniform("material.kSpecular");
		currProg->AddUniform("material.kShininess");
		currProg->AddUniform("lightEnabled");
		currProg->AddUniform("numLights");
		currProg->AddUniform("lights[0].type");
		currProg->AddUniform("lights[0].position_cameraspace");
		currProg->AddUniform("lights[0].color");
		currProg->AddUniform("lights[0].power");
		currProg->AddUniform("lights[0].kC");
		currProg->AddUniform("lights[0].kL");
		currProg->AddUniform("lights[0].kQ");
		currProg->AddUniform("lights[0].spotDirection");
		currProg->AddUniform("lights[0].cosCutoff");
		currProg->AddUniform("lights[0].cosInner");
		currProg->AddUniform("lights[0].exponent");
		currProg->AddUniform("lights[1].type");
		currProg->AddUniform("lights[1].position_cameraspace");
		currProg->AddUniform("lights[1].color");
		currProg->AddUniform("lights[1].power");
		currProg->AddUniform("lights[1].kC");
		currProg->AddUniform("lights[1].kL");
		currProg->AddUniform("lights[1].kQ");
		currProg->AddUniform("lights[1].spotDirection");
		currProg->AddUniform("lights[1].cosCutoff");
		currProg->AddUniform("lights[1].cosInner");
		currProg->AddUniform("lights[1].exponent");
		currProg->AddUniform("colorTextureEnabled[0]");
		currProg->AddUniform("colorTextureEnabled[1]");
		currProg->AddUniform("colorTextureEnabled[2]");
		currProg->AddUniform("colorTexture[0]");
		currProg->AddUniform("colorTexture[1]");
		currProg->AddUniform("colorTexture[2]");
		currProg->AddUniform("textEnabled");
		currProg->AddUniform("textColor");
	}

	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);

	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	{
		MeshBuilder::GetInstance()->GenerateAxes("reference");
		MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
		MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("quad")->textureArray[0] = LoadTGA("Image//calibri.tga");
		MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
		MeshBuilder::GetInstance()->GetMesh("text")->textureArray[0] = LoadTGA("Image//calibri.tga");
		MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
		MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
		MeshBuilder::GetInstance()->GetMesh("Chair")->textureArray[0] = LoadTGA("Image//chair.tga");
		MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
		MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
		MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
		MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
		
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);

		MeshBuilder::GetInstance()->GenerateOBJ("obj_cube", "OBJ//Cube.obj");
		MeshBuilder::GetInstance()->GetMesh("obj_cube")->textureArray[0] = LoadTGA("Image//color2.tga");
		MeshBuilder::GetInstance()->GetMesh("obj_cube")->textureArray[1] = LoadTGA("Image//Red.tga");
		MeshBuilder::GetInstance()->GetMesh("obj_cube")->textureArray[2] = LoadTGA("Image//LightGreen.tga");

		MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
		MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
		MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureArray[0] = LoadTGA("Image//terrain_texture.tga");
		MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureArray[0] = LoadTGA("Image//terrain_texture.tga");
		MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(0.0f, 1.0f, 1.0f), 1.0f);
		MeshBuilder::GetInstance()->GenerateCube("cubeBarrel", Color(0.0f, 0.0f, 1.0f), 1.0f);

		// Skybox
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureArray[0] = LoadTGA("Image//SkyBox//skybox_front3.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureArray[0] = LoadTGA("Image//SkyBox//skybox_back3.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureArray[0] = LoadTGA("Image//SkyBox//skybox_left3.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureArray[0] = LoadTGA("Image//SkyBox//skybox_right3.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureArray[0] = LoadTGA("Image//SkyBox//skybox_top3.tga");
		MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureArray[0] = LoadTGA("Image//SkyBox//skybox_bottom3.tga");
		
		// Laser
		MeshBuilder::GetInstance()->GenerateRay("laser", Color(1.f, 0.f, 0.f), 10.0f);

		MeshBuilder::GetInstance()->GenerateRay("selector", Color(0.5f, 1.f, 0.5f), 10.0f);
		
		// Grid Mesh
		MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);
		MeshBuilder::GetInstance()->GetMesh("GRIDMESH")->textureArray[0] = LoadTGA("Image//GridWhole.tga");

		// Selected Grid Mesh
		MeshBuilder::GetInstance()->GenerateQuad("SelectedGrid", Color(1, 1, 1), 10.f);
		MeshBuilder::GetInstance()->GetMesh("SelectedGrid")->textureArray[0] = LoadTGA("Image//Grid.tga");
		MeshBuilder::GetInstance()->GetMesh("SelectedGrid")->textureArray[1] = LoadTGA("Image//Red.tga");
	}

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetSelectedGridMesh("SelectedGrid");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f, 160000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	Create::Asset("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Asset("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	// ==================== Target cube ======================
	// Parent
	//GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f));
	////CUpdateTransformation* aCubeBaseMtx = new CUpdateTransformation();
	////CSceneNode* aCubeBaseNode = CSceneGraph::GetInstance()->AddNode(aCube);
	//aCube->SetScale(Vector3(5.f, 5.f, 7.0f));
	//aCube->SetCollider(true);
	//aCube->SetAABB(Vector3(2.5f, 2.5f, 3.5f), Vector3(-2.5f, -2.5f, -3.5f)); // half of the original scale
	////aCube->InitLOD("cube", "sphere", "cubeSG");
	////aCubeBaseMtx->ApplyUpdate(0.0f, 0.0f, 1.0f);
	////aCubeBaseMtx->SetSteps(-60, 60);
	////aCubeBaseNode->SetUpdateTransformation(aCubeBaseMtx);
	//
	//// Add the pointer to this new entity to the Scene Graph
	//CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(aCube);
	//if (theNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}
	//
	//// Child
	//GenericEntity* rightThread = Create::Entity("cubeSG", Vector3(-15.0f, -2.0f, -20.0f));
	//rightThread->SetScale(Vector3(2.5f, 3.0f, 5.0f));
	//rightThread->SetCollider(true);
	//rightThread->SetAABB(Vector3(rightThread->GetScale().x / 2, rightThread->GetScale().y / 2, rightThread->GetScale().z / 2 ), 
	//	Vector3(-rightThread->GetScale().x / 2, -rightThread->GetScale().y / 2, -rightThread->GetScale().z / 2));
	//CSceneNode* rightThreadNode = theNode->AddChild(rightThread);
	//if (rightThreadNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}
	//GenericEntity* leftThread = Create::Entity("cubeSG", Vector3(-25.0f, -2.0f, -20.0f));
	//leftThread->SetScale(Vector3(2.5f, 3.0f, 5.0f));
	//leftThread->SetCollider(true);
	//leftThread->SetAABB(Vector3(leftThread->GetScale().x / 2, leftThread->GetScale().y / 2, leftThread->GetScale().z / 2),
	//	Vector3(-leftThread->GetScale().x / 2, -leftThread->GetScale().y / 2, -leftThread->GetScale().z / 2));
	//CSceneNode* leftThreadNode = theNode->AddChild(leftThread);
	//if (leftThreadNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}

	//GenericEntity* turret = Create::Entity("cubeSG", Vector3(-20.0f, 5.0f, -20.0f));
	//turret->SetScale(Vector3(4.0f, 4.0f, 4.0f));
	//turret->SetCollider(true);
	//turret->SetAABB(Vector3(turret->GetScale().x / 2, turret->GetScale().y / 2, turret->GetScale().z / 2),
	//	Vector3(-turret->GetScale().x / 2, -turret->GetScale().y / 2, -turret->GetScale().z / 2));
	//CSceneNode* turretNode = theNode->AddChild(turret);
	//if (turretNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}

	//GenericEntity* barrel = Create::Entity("cubeBarrel", Vector3(-20.0f, 5.0f, -15.0f));
	//barrel->SetScale(Vector3(1.0f, 1.0f, 4.0f));
	//barrel->SetCollider(true);
	//barrel->SetAABB(Vector3(barrel->GetScale().x / 2, barrel->GetScale().y / 2, barrel->GetScale().z / 2),
	//	Vector3(-barrel->GetScale().x / 2, -barrel->GetScale().y / 2, -barrel->GetScale().z / 2));
	//CSceneNode* barrelNode = turretNode->AddChild(barrel);
	//if (barrelNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}
	// ==================================================================================

	// Moving cube thing
	/*GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	CUpdateTransformation* baseMtx = new CUpdateTransformation();
	baseMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	baseMtx->SetSteps(-60, 60);
	baseNode->SetUpdateTransformation(baseMtx);

	GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* childNode = baseNode->AddChild(childCube);
	childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);

	GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);
	CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	aRotateMtx->SetSteps(-120, 60);
	grandchildNode->SetUpdateTransformation(aRotateMtx);*/
	// =======================================================

	// Create a CEnemy instance
	CEnemy* theEnemyList = new CEnemy();
	theEnemy.push_back(theEnemyList);

	theEnemy[0] = new CEnemy();
	theEnemy[0]->Init();
	theEnemy[0]->SetScale(Vector3(5.f, 5.f, 7.f));
	theEnemy[0]->SetAABB(Vector3(theEnemy[0]->GetScale().x / 2, theEnemy[0]->GetScale().y / 2, theEnemy[0]->GetScale().z / 2), 
		Vector3(-(theEnemy[0]->GetScale().x / 2) ,-(theEnemy[0]->GetScale().y / 2), -(theEnemy[0]->GetScale().z / 2)));
	CSceneNode* theEnemyNode = CSceneGraph::GetInstance()->AddNode(theEnemy[0]);
	theEnemy[0]->InitLOD("obj_cube", "sphere", "cube");
	theEnemy[0]->SetCollider(true);
	/*CUpdateTransformation *enemyBaseMtx = new CUpdateTransformation();
	enemyBaseMtx->ApplyUpdate(0, 20, 0);
	enemyBaseMtx->SetSteps(-60, 60);
	theEnemyNode->SetUpdateTransformation(enemyBaseMtx);*/

	if (theEnemyNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}
	theEnemy.push_back(theEnemyList);
	theEnemy[1] = new CEnemy();
	theEnemy[1]->Init();
	theEnemy[1]->InitLOD("obj_cube", "sphere", "cube");
	theEnemy[1]->SetScale(Vector3(2.5f, 2.5f, 5.f));
	theEnemy[1]->SetPosition(Vector3(theEnemy[0]->GetPos().x - 5, theEnemy[0]->GetPos().y - 2, theEnemy[0]->GetPos().z + 1));
	theEnemy[1]->SetAABB(Vector3( theEnemy[1]->GetPosition().x + (theEnemy[1]->GetScale().x / 2), 
								  theEnemy[1]->GetPosition().y + (theEnemy[1]->GetScale().y / 2), 
								  theEnemy[1]->GetPosition().z + (theEnemy[1]->GetScale().z / 2)),
						 Vector3((theEnemy[1]->GetPosition().x - (theEnemy[1]->GetScale().x / 2)), 
								 (theEnemy[1]->GetPosition().y - (theEnemy[1]->GetScale().y / 2)), 
								 (theEnemy[1]->GetPosition().z - (theEnemy[1]->GetScale().z / 2))));
	theEnemy[1]->SetCollider(true);
	CSceneNode* theEnemyChildNode = theEnemyNode->AddChild(theEnemy[1]);
	if (theEnemyChildNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}
	theEnemy.push_back(theEnemyList);
	theEnemy[2] = new CEnemy();
	theEnemy[2]->Init();
	theEnemy[2]->InitLOD("obj_cube", "sphere", "cube");
	theEnemy[2]->SetScale(Vector3(2.5f, 2.5f, 5.f));
	theEnemy[2]->SetPosition(Vector3(theEnemy[0]->GetPos().x + 5, theEnemy[0]->GetPos().y - 2, theEnemy[0]->GetPos().z + 1));
	theEnemy[2]->SetAABB(Vector3(theEnemy[2]->GetPosition().x + (theEnemy[2]->GetScale().x / 2),
								theEnemy[2]->GetPosition().y + (theEnemy[2]->GetScale().y / 2),
								theEnemy[2]->GetPosition().z + (theEnemy[2]->GetScale().z / 2)),
						Vector3((theEnemy[2]->GetPosition().x - (theEnemy[2]->GetScale().x / 2)),
								(theEnemy[2]->GetPosition().y - (theEnemy[2]->GetScale().y / 2)),
								(theEnemy[2]->GetPosition().z - (theEnemy[2]->GetScale().z / 2))));
	theEnemy[2]->SetCollider(true);
	CSceneNode* theEnemyChildNode2 = theEnemyNode->AddChild(theEnemy[2]);
	if (theEnemyChildNode2 == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}
	theEnemy.push_back(theEnemyList);
	theEnemy[3] = new CEnemy();
	theEnemy[3]->Init();
	theEnemy[3]->InitLOD("obj_cube", "sphere", "cube");
	theEnemy[3]->SetScale(Vector3(4.f, 4.f, 4.f));
	theEnemy[3]->SetPosition(Vector3(theEnemy[0]->GetPos().x, theEnemy[0]->GetPos().y + 5, theEnemy[0]->GetPos().z + 1.5));
	theEnemy[3]->SetAABB(Vector3(theEnemy[3]->GetPosition().x + (theEnemy[3]->GetScale().x / 2),
								theEnemy[3]->GetPosition().y + (theEnemy[3]->GetScale().y / 2),
								theEnemy[3]->GetPosition().z + (theEnemy[3]->GetScale().z / 2)),
						Vector3((theEnemy[3]->GetPosition().x - (theEnemy[3]->GetScale().x / 2)),
								(theEnemy[3]->GetPosition().y - (theEnemy[3]->GetScale().y / 2)),
								(theEnemy[3]->GetPosition().z - (theEnemy[3]->GetScale().z / 2))));
	theEnemy[3]->SetCollider(true);
	CSceneNode* theEnemyChildNode3 = theEnemyNode->AddChild(theEnemy[3]);
	if (theEnemyChildNode2 == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}


	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
		"SKYBOX_LEFT", "SKYBOX_RIGHT",
		"SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	theEnemy[0]->SetTerrain(groundEntity);
	
	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");
}

void SceneText::Update(double dt)
{
	// Update our entities
	CSpatialPartition::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	theEnemy[1]->SetPosition(Vector3(theEnemy[0]->GetPos().x - 5, theEnemy[0]->GetPos().y - 2, theEnemy[0]->GetPos().z + 1));
	theEnemy[2]->SetPosition(Vector3(theEnemy[0]->GetPos().x + 5, theEnemy[0]->GetPos().y - 2, theEnemy[0]->GetPos().z + 1));
	theEnemy[3]->SetPosition(Vector3(theEnemy[0]->GetPos().x, theEnemy[0]->GetPos().y + 5, theEnemy[0]->GetPos().z + 1.5));

	/*theEnemy[1]->SetAABB(Vector3(theEnemy[1]->GetPos().x + (theEnemy[1]->GetScale().x / 2),
								theEnemy[1]->GetPos().y + (theEnemy[1]->GetScale().y / 2),
								theEnemy[1]->GetPos().z + (theEnemy[1]->GetScale().z / 2)),
						Vector3((theEnemy[1]->GetPos().x - (theEnemy[1]->GetScale().x / 2)),
								(theEnemy[1]->GetPos().y - (theEnemy[1]->GetScale().y / 2)),
								(theEnemy[1]->GetPos().z - (theEnemy[1]->GetScale().z / 2))));*/


	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();
	CSpatialPartition::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}