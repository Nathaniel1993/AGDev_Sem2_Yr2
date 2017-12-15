#include "SpatialPartition.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../SceneGraph\SceneGraph.h"
#include "KeyboardController.h"

CSpatialPartition *CSpatialPartition::sp_instance = 0;

CSpatialPartition::CSpatialPartition(void)
	: theGrid(NULL)
	, xSize(0)
	, zSize(0)
	, xGridSize(0)
	, zGridSize(0)
	, xNumOfGrid(0)
	, zNumOfGrid(0)
	, yOffset(0.0f)
	, _meshName("")
	, theCamera(NULL)
	, selectedGrid(0)
	, prevGrid(0)
	, gridChanged(true)
	, selectedGridMesh(NULL)
{
}

CSpatialPartition::~CSpatialPartition(void)
{
	theCamera = NULL;
	delete[] theGrid;

	if (selectedGridMesh)
	{
		delete selectedGridMesh;
		selectedGridMesh = NULL;
	}
}

/********************************************************************************
Initialise the spatial partition
********************************************************************************/
bool CSpatialPartition::Init(const int xGridSize, const int zGridSize,
	const int xNumOfGrid, const int zNumOfGrid,
	const float yOffset)
{
	if ((xGridSize>0) && (zGridSize>0)
		&& (xNumOfGrid>0) && (zNumOfGrid>0))
	{
		this->xNumOfGrid = xNumOfGrid;
		this->zNumOfGrid = zNumOfGrid;
		this->xGridSize = xGridSize;
		this->zGridSize = zGridSize;
		this->xSize = xGridSize * xNumOfGrid;
		this->zSize = zGridSize * zNumOfGrid;
		this->yOffset = yOffset;
		// Create an array of grids
		theGrid = new CGrid[xNumOfGrid*zNumOfGrid];
		// Initialise the array of grids
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i*zNumOfGrid + j].Init(i, j, xGridSize, zGridSize, (xSize >> 1), (zSize >> 1));
			}
		}
		// Assign a Mesh to each Grid if available.
		ApplyMesh();
		// Create a migration list vector
		MigrationList.clear();
		return true;
	}
	return false;
}

/********************************************************************************
Update the spatial partition
********************************************************************************/
void CSpatialPartition::Update(double _dt)
{
	GridSelection();

	// Update all entities
	for (int i = 0; i<xNumOfGrid; i++)
	{
		for (int j = 0; j<zNumOfGrid; j++)
		{
			// Update entity
			int gridNum = i * zNumOfGrid + j;
			theGrid[gridNum].Update(&MigrationList, _dt);

			// Check visibility
			if (IsVisible(theCamera->GetCameraPos(), theCamera->GetCameraTarget() - theCamera->GetCameraPos(), i, j) == true)
			{
				// Calculate LOD for this Cgrid
				float distance = CalculateDistanceSquare(&(theCamera->GetCameraPos()), i, j);
				if (distance < LevelOfDetails_Distances[0])
				{
					theGrid[gridNum].SetDetailLevel(CLevelOfDetails::HIGH_DETAILS);
				}
				else if (distance < LevelOfDetails_Distances[1])
				{
					theGrid[gridNum].SetDetailLevel(CLevelOfDetails::MID_DETAILS);
				}
				else
				{
					theGrid[gridNum].SetDetailLevel(CLevelOfDetails::LOW_DETAILS);
				}
			}
			else
				theGrid[gridNum].SetDetailLevel(CLevelOfDetails::NO_DETAILS);


		}
	}

	// Update the Scene Graph
	CSceneGraph::GetInstance()->Update();

	// If there are objects due for migration, then process them
	if (MigrationList.empty() == false)
	{
		// Check each object to see if they are no longer in this grid
		for (int i = 0; i < MigrationList.size(); ++i)
		{
			Add(MigrationList[i]);
		}
		MigrationList.clear();
	}

	// Check for Collision amongst entities with collider properties
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < zNumOfGrid; j++)
		{
			int gridNum = i * zNumOfGrid + j;

			// Check only specific grids around grid that is checked
			// X X O
			// X C O
			// X O O
			// 'X' is the extra grid to check, 'C' is the grid being checked, 
			// 'O' is the other grid that doesn't need to be checked
			vector<EntityBase*> ObjectsToCheck;
			int gridNumToCheck;
			if (i < xNumOfGrid - 1)
			{
				if (j > 0)
				{ // Top Left
					gridNumToCheck = (i + 1) * zNumOfGrid + (j - 1);
					for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
					{
						ObjectsToCheck.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
					}
				}

				// Top Mid
				gridNumToCheck = (i + 1) * zNumOfGrid + (j);
				for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
				{
					ObjectsToCheck.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
				}
			}

			if (j > 0)
			{
				if (i > 0)
				{ // Bottom Left
					gridNumToCheck = (i - 1) * zNumOfGrid + (j - 1);
					for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
					{
						ObjectsToCheck.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
					}
				}

				// Middle Left
				gridNumToCheck = (i)* zNumOfGrid + (j - 1);
				for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
				{
					ObjectsToCheck.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
				}
			}

			for (int k = 0; k < theGrid[gridNum].GetListOfObject().size(); ++k)
			{
				ObjectsToCheck.push_back(theGrid[gridNum].GetListOfObject()[k]);
			}

			theGrid[gridNum].SetObjectsToCheck(ObjectsToCheck);
			theGrid[gridNum].CheckForCollision();
		}
	}

	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < zNumOfGrid; j++)
		{
			theGrid[i * zNumOfGrid + j].DeleteDone();
		}
	}
}

/********************************************************************************
Render the spatial partition
********************************************************************************/
void CSpatialPartition::Render(Vector3* theCameraPosition)
{
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < zNumOfGrid; j++)
		{
			theGrid[i * zNumOfGrid + j].RenderObjects();
		}
	}

	// Render the Spatial Partitions
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
		modelStack.Translate(0.0f, yOffset, 0.0f);
		modelStack.PushMatrix();
			modelStack.Scale(xGridSize, 1.0f, zGridSize);
			modelStack.Rotate(-90, 1, 0, 0);
			theGrid[0].Render();
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	float xPos = (float)(xGridSize * (selectedGrid / xNumOfGrid) - (xSize >> 1) + (xGridSize / 2));
	float zPos = (float)(zGridSize * (selectedGrid % zNumOfGrid) - (zSize >> 1) + (zGridSize / 2));

	modelStack.PushMatrix();
		modelStack.Translate(0.0f, yOffset + 0.1f, 0.0f);
		modelStack.PushMatrix();
			modelStack.Translate(xPos, 0.f, zPos);
			modelStack.PushMatrix();
				modelStack.Scale((float)(xGridSize / xNumOfGrid), 1.0f, (float)(zGridSize / zNumOfGrid));
				modelStack.Rotate(-90, 1, 0, 0);
				RenderHelper::RenderMesh(selectedGridMesh, true);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	modelStack.PopMatrix();
	
}

/********************************************************************************
Add a new object model
********************************************************************************/
void CSpatialPartition::Add(EntityBase* theObject)
{
	// Get the indices of the object's position
	int xIndex = (((int)theObject->GetPosition().x - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int zIndex = (((int)theObject->GetPosition().z - (-zSize >> 1)) / (zSize / zNumOfGrid));
	// Add them to each grid
	if (((xIndex >= 0) && (xIndex<xNumOfGrid)) && ((zIndex >= 0) && (zIndex<zNumOfGrid)))
	{
		theGrid[xIndex*zNumOfGrid + zIndex].Add(theObject);
	}
}

void CSpatialPartition::SetMesh(const std::string& _meshName)
{
	this->_meshName = _meshName;

	// Assign a Mesh to each Grid if available.
	ApplyMesh();
}

void CSpatialPartition::SetSelectedGridMesh(const std::string & _meshName)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh != nullptr)
	{
		this->selectedGridMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	}
}

void CSpatialPartition::ApplyMesh(void)
{
	if (_meshName.size() != 0)
	{
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i*zNumOfGrid + j].SetMesh(_meshName);
			}
		}
	}
}

// Get xSize of the entire spatial partition
int CSpatialPartition::GetxSize(void) const
{
	return xSize;
}
// Get zSize of the entire spatial partition
int CSpatialPartition::GetzSize(void) const
{
	return zSize;
}
// Get xSize of one Grid
int CSpatialPartition::GetxGridSize(void) const
{
	return xGridSize;
}
// Get zSize of one Grid
int CSpatialPartition::GetzGridSize(void) const
{
	return zGridSize;
}
// Get xNumOfGrid
int CSpatialPartition::GetxNumOfGrid(void) const
{
	return xNumOfGrid;
}
// Get zNumOfGrid
int CSpatialPartition::GetzNumOfGrid(void) const
{
	return zNumOfGrid;
}

// Get a particular grid
CGrid CSpatialPartition::GetGrid(const int xIndex, const int yIndex) const
{
	return theGrid[xIndex*zNumOfGrid + yIndex];
}

vector<EntityBase*> CSpatialPartition::GetObjects(Vector3 position, const float radius)
{
	// Get the indices of the object's position
	int xIndex = (((int)position.x - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int zIndex = (((int)position.z - (-zSize >> 1)) / (zSize / zNumOfGrid));

	return theGrid[xIndex*zNumOfGrid + zIndex].GetListOfObject();
}

// Remove but not delete object from this grid
void CSpatialPartition::Remove(EntityBase* theObject)
{
	
}

// Calculate the square distance from camera to a grid's centrepoint
float CSpatialPartition::CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int zIndex)
{
	//float xDistance = (xIndex * xNumOfGrid + (xSize / 2)) - theCameraPosition->x;
	//float yDistance = (zIndex * zNumOfGrid + (zSize / 2)) - theCameraPosition->z;

	// return (float)(xDistance*xDistance + yDistance*yDistance);

	float xDistance = (xIndex * xGridSize + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition->x;
	float zDistance = (zIndex * zGridSize + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition->z;

	return (float)(xDistance*xDistance + zDistance*zDistance);
}

// PrintSelf
void CSpatialPartition::PrintSelf() const
{
	cout << "******* Start of CSpatialPartition::PrintSelf() **********************************" << endl;
	cout << "xSize\t:\t" << xSize << "\tzSize\t:\t" << zSize << endl;
	cout << "xNumOfGrid\t:\t" << xNumOfGrid << "\tzNumOfGrid\t:\t" << zNumOfGrid << endl;
	if (theGrid)
	{
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i*zNumOfGrid + j].PrintSelf();
			}
		}
	}
	else
		cout << "theGrid : NULL" << endl;
	cout << "******* End of CSpatialPartition::PrintSelf() **********************************" << endl;
}

/********************************************************************************
Store a camera pointer into this class
********************************************************************************/
void CSpatialPartition::SetCamera(FPSCamera* _cameraPtr)
{
	theCamera = _cameraPtr;
}

/********************************************************************************
Remove the camera pointer from this class
********************************************************************************/
void CSpatialPartition::RemoveCamera(void)
{
	theCamera = nullptr;
}
void CSpatialPartition::SetLevelOfDetails(const float distance_High2Mid, const float distance_Mid2Low)
{
	LevelOfDetails_Distances[0] = distance_High2Mid;
	LevelOfDetails_Distances[1] = distance_Mid2Low;
}

bool CSpatialPartition::IsVisible(Vector3 theCameraPosition, Vector3 theCameraDirection, const int xIndex, const int zIndex)
{
	float xDistance = (xIndex * xGridSize + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition.x;
	float zDistance = (zIndex * zGridSize + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition.z;
	Vector3 gridCentre(xDistance, 0, zDistance);
	if (xDistance * xDistance + zDistance * zDistance < (xGridSize * xGridSize + zGridSize * zGridSize))
		return true;
	
	if (theCameraDirection.Dot(gridCentre) < 0)
		return false;
	return true;
}

void CSpatialPartition::GridSelection()
{
	gridChanged = false;
	if (KeyboardController::GetInstance()->IsKeyReleased('L'))
	{ // Z Plus
		int zAxis = selectedGrid % zNumOfGrid;
		if (zAxis + 1 < zNumOfGrid)
		{
			prevGrid = selectedGrid;
			++selectedGrid;
			gridChanged = true;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('J'))
	{ // Z Minus
		int zAxis = selectedGrid % zNumOfGrid;
		if (zAxis - 1 >= 0)
		{
			prevGrid = selectedGrid;
			--selectedGrid;
			gridChanged = true;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('I'))
	{ // X Plus
		int xAxis = selectedGrid / xNumOfGrid;
		if (xAxis + 1 < xNumOfGrid)
		{
			prevGrid = selectedGrid;
			selectedGrid += zNumOfGrid;
			gridChanged = true;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('K'))
	{ // X Minus
		int xAxis = selectedGrid / xNumOfGrid;
		if (xAxis - 1 >= 0)
		{
			prevGrid = selectedGrid;
			selectedGrid -= zNumOfGrid;
			gridChanged = true;
		}
	}

	if (gridChanged)
	{
		if (selectedList.size() > 0)
		{
			for (int i = 0; i < theGrid[prevGrid].GetListOfObject().size(); i++)
			{
				theGrid[prevGrid].GetListOfObject()[i]->SetSelectedGrid(false);
			}
		}
		
		// Check all grids surrounding the current grid
		// X X X
		// X C X
		// X X X
		// 'X' is the extra grid to check, 'C' is the grid being checked, 

		int gridNumToCheck;
		int xSelected = selectedGrid / xNumOfGrid;
		int zSelected = selectedGrid % zNumOfGrid;
		if (xSelected < xNumOfGrid - 1)
		{ // X Grid less than max
			// X X X
			// O C O
			// O O O
			if (zSelected > 0)
			{ // Top Left
				gridNumToCheck = (xSelected + 1) * zNumOfGrid + (zSelected - 1);
				for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
				{
					selectedList.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
				}
			}

			if (zSelected < zNumOfGrid - 1)
			{ // Top Right
				gridNumToCheck = (xSelected + 1) * zNumOfGrid + (zSelected + 1);
				for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
				{
					selectedList.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
				}
			}

			// Top Mid
			gridNumToCheck = (xSelected + 1) * zNumOfGrid + (zSelected);
			for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
			{
				selectedList.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
			}
		}

		if (zSelected > 0)
		{ // Z Grid more than 0
			// O O O
			// X C O
			// X O O
			if (xSelected > 0)
			{ // Bottom Left
				gridNumToCheck = (xSelected - 1) * zNumOfGrid + (zSelected - 1);
				for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
				{
					selectedList.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
				}
			}

			// Middle Left
			gridNumToCheck = (xSelected)* zNumOfGrid + (zSelected - 1);
			for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
			{
				selectedList.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
			}
		}

		if (zSelected < zNumOfGrid - 1)
		{ // Z Grid less than man num of grid
			// O O O
			// O C X
			// O O X
			if (xSelected > 0)
			{ // Bottom Right
				gridNumToCheck = (xSelected - 1) * zNumOfGrid + (zSelected + 1);
				for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
				{
					selectedList.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
				}
			}

			// Middle Right
			gridNumToCheck = (xSelected) * zNumOfGrid + (zSelected + 1);
			for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
			{
				selectedList.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
			}
		}

		if (xSelected > 0)
		{ // XGrid mode than 0
		  // O O O
		  // O C O
		  // O X O
		  // Bottom Middle
			gridNumToCheck = (xSelected - 1) * zNumOfGrid + (zSelected);
			for (int k = 0; k < theGrid[gridNumToCheck].GetListOfObject().size(); ++k)
			{
				selectedList.push_back(theGrid[gridNumToCheck].GetListOfObject()[k]);
			}
		}

		for (int k = 0; k < theGrid[selectedGrid].GetListOfObject().size(); ++k)
		{
			selectedList.push_back(theGrid[selectedGrid].GetListOfObject()[k]);
		}

		for (int i = 0; i < selectedList.size(); i++)
		{
			if(selectedList[i]->GetPosition().x <= theGrid[selectedGrid].GetMax().x 
				&& selectedList[i]->GetPosition().x >= theGrid[selectedGrid].GetMin().x
				&& selectedList[i]->GetPosition().z <= theGrid[selectedGrid].GetMax().z
				&& selectedList[i]->GetPosition().z >= theGrid[selectedGrid].GetMin().z)
				selectedList[i]->SetSelectedGrid(true);
		}
	}
}
