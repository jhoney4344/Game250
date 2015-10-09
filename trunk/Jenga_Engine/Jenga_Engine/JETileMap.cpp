/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JETileMap.cpp
author      Minsu Kang
email       minsu9486@gmail.com
course      GAM250

*******************************************************************************/

#include "JETileMap.h"

#include <cstring> // string
#include <cstdlib> // rand
#include <ctime>  // for seed

#include "JEApplication.h"
#include "JELoadTextureNew.h"
#include "JEGraphicsNew.h"

#include "JECharacterManager.h"
#include "JECharacter.h"
#include "JEParts.h"
#include "JEDayAndNight.h"

namespace{
	TileMapManager tileMapManager;
}

TileMapManager::TileMapManager(void)
{
}

TileMapManager::~TileMapManager(void)
{
}

void TileMapManager::InitTiles_OpenWorld(JEPoint2D cameraPosition)
{
	JEPRINTF("Tile Map is generating\n");
	

	LoadTileMapImages();

	rightLandEnd = Get1stCharacterTilePosition();
	leftLandEnd = TilePosition(rightLandEnd.x + 1, 0);

	GenerateFlatTerrain(rightLandEnd.x + 1, CAMERA_TERM);
	GenerateFlatTerrain(leftLandEnd.x  - 1, CAMERA_TERM);

	// Init for the tree
	tree.rightLastCreatedTileXAxis =  TREE_TERM;
	tree.leftLastCreatedTileXAxis  = -TREE_TERM;

	// Init for the bird
	bird.rightLastCreatedTileXAxis =  bird.RandBirdTerm();
	bird.leftLastCreatedTileXAxis  = -bird.RandBirdTerm();

	// Init for the mole
	JEPoint2D randVelocity = JEPoint2D((double)RandValue(-MOLE_SPEED, MOLE_SPEED) / 10, (double)RandValue(-MOLE_SPEED, MOLE_SPEED) / 10);
	mole.InitMole(cameraPosition, randVelocity);

	// Init for the mineral
	mineral.rightLastCreatedTileXAxis =  MINERAL_TERM;
	mineral.leftLastCreatedTileXAxis  = -MINERAL_TERM;

	// Init for the barrel
	barrel.rightLastCreatedTileXAxis =  BARREL_TERM;
	barrel.leftLastCreatedTileXAxis  = -BARREL_TERM;

	// Init for the deer
	deer.rightLastCreatedTileXAxis = DEER_TERM;
	deer.leftLastCreatedTileXAxis = -DEER_TERM;

	// Init for the stone
	stone.rightLastCreatedTileXAxis = STONE_TERM;
	stone.leftLastCreatedTileXAxis  = -STONE_TERM;
}


void TileMapManager::UpdateTiles(double totalTime)
{
	JE_UNUSED_PARAM(totalTime);

	Contents collisionArea[CHARACTER_COLLISION_AREA * CHARACTER_COLLISION_AREA];
	GetCollisionArea(Get1stCharacterTilePosition(), collisionArea, CHARACTER_COLLISION_AREA);

	PrintCharactersTilePosition();
	PrintCollisionAreaContentsInfo(collisionArea, CHARACTER_COLLISION_AREA);
	PrintCollisionAreaTileTypeInfo(collisionArea, CHARACTER_COLLISION_AREA);
}

void TileMapManager::UpdateOpenWorldTiles(const JEPoint2D cameraPosition, const double totalTime, const JE::DAY_NIGHT currState, const double dt)
{
	srand((unsigned)totalTime);

	/* Update for the Tiles *********************************************************************************************************************/
	TilePosition cameraTilePosition = ConvertWorldToTilePosition(cameraPosition);
	if (rightLandEnd.x - CAMERA_TERM <= cameraTilePosition.x)
	{
		int randVariable = RandValue(1, 10);
		int randLangth = RandValue(50, 100);

		switch (randVariable)
		{
		case 1: // 30%
		case 2:
		case 3:
			GenerateFlatTerrain(rightLandEnd.x + 1, randLangth - 20);
			JEPRINTF("R FlatTerrain\n");

			break;
		case 4: // 20%
		case 5:
			GenerateDifficultTerrain(rightLandEnd.x + 1);
			JEPRINTF("R DifficultTerrain\n");
		
			break;
		default: // 50%
			GenerateNormalTerrain(rightLandEnd.x + 1, randLangth);
			JEPRINTF("R NormalTerrain\n");
		
			break;
		}
	}
	else if (leftLandEnd.x + CAMERA_TERM >= cameraTilePosition.x)
	{
		srand((unsigned)totalTime);

		int randVariable = RandValue(1, 10);
		int randLangth = RandValue(50, 100);

		switch (randVariable)
		{
		case 1: // 30%
		case 2:
		case 3:
			GenerateFlatTerrain(leftLandEnd.x - 1, randLangth - 20);
			JEPRINTF("L FlatTerrain\n");
		
			break;
		case 4: // 20%
		case 5:
			GenerateDifficultTerrain(leftLandEnd.x - 1);
			JEPRINTF("L DifficultTerrain\n");
		
			break;
		default: // 50%
			GenerateNormalTerrain(leftLandEnd.x - 1, randLangth);
			JEPRINTF("L NormalTerrain\n");
	
			break;
		}
	}
	/****************************************************************************************************************************************/

	/* Update for the Bird ****************************************************************************************************/
	if (bird.rightLastCreatedTileXAxis - MOLE_TERM <= cameraTilePosition.x)
	{
		TilePosition startPosition(bird.rightLastCreatedTileXAxis, GetLandHeight(bird.rightLastCreatedTileXAxis));
		int fixedHeight = RandValue(3, 5);
		int length = RandValue(8, 20) + fixedHeight;
		int height = startPosition.y + fixedHeight;

		// Generate the land
		for (int xColumn = startPosition.x; xColumn < startPosition.x + length; xColumn++)
		{
			TilePosition currentPosition(xColumn, GetLandHeight(xColumn));

			if (xColumn < startPosition.x + (length / 2))
				height += bird.RandHeightTerm(totalTime);
			else
				height -= bird.RandHeightTerm(totalTime);

			for (int yColumn = currentPosition.y + fixedHeight; yColumn < height; yColumn++)
			{
				SetTileType(TilePosition(xColumn, yColumn), tileIDs.dirtID);
			}
		}

		bird.rightLastCreatedTileXAxis += bird.RandBirdTerm();
	}
	else if (bird.leftLastCreatedTileXAxis + MOLE_TERM >= cameraTilePosition.x)
	{
		TilePosition startPosition(bird.leftLastCreatedTileXAxis, GetLandHeight(bird.leftLastCreatedTileXAxis));
		int fixedHeight = RandValue(3, 5);
		int length = RandValue(8, 20) + fixedHeight;
		int height = startPosition.y + fixedHeight;

		// Generate the land
		for (int xColumn = startPosition.x; xColumn > startPosition.x - length; xColumn--)
		{
			TilePosition currentPosition(xColumn, GetLandHeight(xColumn));

			if (xColumn > startPosition.x - (length / 2))
				height += bird.RandHeightTerm(totalTime);
			else
				height -= bird.RandHeightTerm(totalTime);

			for (int yColumn = currentPosition.y + fixedHeight; yColumn < height; yColumn++)
			{
				SetTileType(TilePosition(xColumn, yColumn), tileIDs.dirtID);
			}
		}

		bird.leftLastCreatedTileXAxis -= bird.RandBirdTerm();
	}
	/****************************************************************************************************************************************/

	/* Update for the Mole **********************************************************************************************************************/
	mole.UpdateMole(cameraPosition, totalTime, dt);

	TilePosition moleTilePosition = ConvertWorldToTilePosition(mole.GetMolePosition());
	
	const JEPoint2D tempForD = GetMolePosition() - cameraPosition;
	const double distanceX = tempForD.x * tempForD.x; // 25000
	const double distanceY = tempForD.y * tempForD.y; //  9000

	// Check the tile's exist, then check the upper direction. If not, the mole will go up.
	if (tileMapManager.tiles.find(moleTilePosition) == tileMapManager.tiles.end())
	{
		mole.SetMoleVelocity(JEPoint2D(mole.GetMoleVelocity().x, (double)RandValue(10, 5)));
	}
	// if there is empty area in the upper direction, the mole will go down.
	else if (GetTileType(moleTilePosition + TilePosition(0, 4)) == tileIDs.emptyID)
	{
		mole.SetMoleVelocity(JEPoint2D(mole.GetMoleVelocity().x, (double)RandValue(-10, -5)));
	}
	// if Mole is detected, move other way
	else if (distanceX < 25000.f || distanceY < 9000.f)
	{
		JEPoint2D direction;

		// Simple normal vectors
		if (tempForD.x >= 0)
			direction.x = 1.f;
		else
			direction.x = -1.f;

		if (tempForD.y >= 0)
			direction.y = 1.f;
		else
			direction.y = -1.f;

		mole.SetMoleVelocity(JEPoint2D(RandValue(5, 10) * direction.x, RandValue(5, 10) * direction.y));
	}
	else if (GetTileType(moleTilePosition) != tileIDs.emptyDirtID)
	{
		// Removing 2 * 2 tiles
		SetTileType(moleTilePosition, tileIDs.emptyDirtID);
		SetTileType(moleTilePosition + TilePosition(1, 0), tileIDs.emptyDirtID);
		SetTileType(moleTilePosition + TilePosition(1, -1), tileIDs.emptyDirtID);
		SetTileType(moleTilePosition + TilePosition(0, -1), tileIDs.emptyDirtID);
	}
	/****************************************************************************************************************************************/

	/* Update for the stone **********************************************************************************************************************/
	if (stone.rightLastCreatedTileXAxis - MIN_NON_DETECT_TERM <= cameraTilePosition.x)
	{
		TilePosition stonePosition(stone.rightLastCreatedTileXAxis, RandValue(-INIT_HIGHT / 2, GetLandHeight(stone.rightLastCreatedTileXAxis) - STONE_TERM));

		if (GetTileType(stonePosition) != tileIDs.stoneID && stone.RandChanging())
		{
			SetTileType(stonePosition, tileIDs.stoneID);
			stone.ChangingStone(stonePosition);
		}

		stone.rightLastCreatedTileXAxis += STONE_TERM;
	}
	else if (stone.leftLastCreatedTileXAxis + MIN_NON_DETECT_TERM >= cameraTilePosition.x)
	{
		TilePosition stonePosition(stone.leftLastCreatedTileXAxis, RandValue(-INIT_HIGHT / 2, GetLandHeight(stone.rightLastCreatedTileXAxis) - STONE_TERM));

		if (GetTileType(stonePosition) != tileIDs.stoneID && stone.RandChanging())
		{
			SetTileType(stonePosition, tileIDs.stoneID);
			stone.ChangingStone(stonePosition);
		}

		stone.leftLastCreatedTileXAxis -= STONE_TERM;
	}
	/****************************************************************************************************************************************/

	/* Update for the Minerals **********************************************************************************************************************/	
	if (mineral.rightLastCreatedTileXAxis - MIN_NON_DETECT_TERM <= cameraTilePosition.x)
	{
		TilePosition mineralPosition(mineral.rightLastCreatedTileXAxis, RandValue(-INIT_HIGHT / 2, GetLandHeight(mineral.rightLastCreatedTileXAxis)));
		const int constTileType = GetTileType(mineralPosition);

		if (!GetTileMineral(mineralPosition) && constTileType != tileIDs.emptyID && constTileType != tileIDs.emptyDirtID
			&& mineral.RandSowing())
		{
			SetTileMieral(mineralPosition, true);
			mineral.SowingAMineral(mineralPosition);
		}

		mineral.rightLastCreatedTileXAxis += MINERAL_TERM;
	}
	else if (mineral.leftLastCreatedTileXAxis + MIN_NON_DETECT_TERM >= cameraTilePosition.x)
	{
		TilePosition mineralPosition(mineral.leftLastCreatedTileXAxis, RandValue(-INIT_HIGHT / 2, GetLandHeight(mineral.rightLastCreatedTileXAxis)));
		const int constTileType = GetTileType(mineralPosition);

		if (!GetTileMineral(mineralPosition) && constTileType != tileIDs.emptyID && constTileType != tileIDs.emptyDirtID
			&& mineral.RandSowing())
		{
			SetTileMieral(mineralPosition, true);
			mineral.SowingAMineral(mineralPosition);
		}

		mineral.leftLastCreatedTileXAxis -= MINERAL_TERM;
	}
	/****************************************************************************************************************************************/

	/* Update for the Trees **********************************************************************************************************************/
	if (tree.rightLastCreatedTileXAxis - MIN_NON_DETECT_TERM <= cameraTilePosition.x)
	{
		TilePosition treePosition(tree.rightLastCreatedTileXAxis, GetLandHeight(tree.rightLastCreatedTileXAxis) + 1);
		JE::OBJECTS treeType = tree.RandPlanting();

		if (treeType != JE::OBJECTS::OBJ_END)
			tree.PlantingTree(treePosition, treeType);

		tree.rightLastCreatedTileXAxis += TREE_TERM;
	}
	else if (tree.leftLastCreatedTileXAxis + MIN_NON_DETECT_TERM >= cameraTilePosition.x)
	{
		TilePosition treePosition(tree.leftLastCreatedTileXAxis, GetLandHeight(tree.leftLastCreatedTileXAxis) + 1);
		JE::OBJECTS treeType = tree.RandPlanting();

		if (treeType != JE::OBJECTS::OBJ_END)
			tree.PlantingTree(treePosition, treeType);

		tree.leftLastCreatedTileXAxis -= TREE_TERM;
	}
	/****************************************************************************************************************************************/

	/* Update for the barrels **********************************************************************************************************************/
	if (barrel.rightLastCreatedTileXAxis - MIN_NON_DETECT_TERM <= cameraTilePosition.x)
	{
		if (barrel.RandDeploy())
		{
			JEPRINTF("Deploy a barrel\n");


			TilePosition deployPosition(barrel.rightLastCreatedTileXAxis, cameraTilePosition.y + 7);
			barrel.DeployShop(deployPosition);
		}
		else
			JEPRINTF("Deploy failed\n");

		barrel.rightLastCreatedTileXAxis += BARREL_TERM;
	}
	else if (barrel.leftLastCreatedTileXAxis + MIN_NON_DETECT_TERM >= cameraTilePosition.x)
	{
		if (barrel.RandDeploy())
		{
			JEPRINTF("Deploy a barrel\n");
			

			TilePosition deployPosition(barrel.leftLastCreatedTileXAxis, cameraTilePosition.y + 7);
			barrel.DeployShop(deployPosition);
		}
		else
			JEPRINTF("Deploy failed\n");
			

		barrel.leftLastCreatedTileXAxis -= BARREL_TERM;
	}
	/****************************************************************************************************************************************/

	/* Update for the deers **********************************************************************************************************************/
	
	if (currState == JE::DAY)
	{
		if (deer.rightLastCreatedTileXAxis - MIN_NON_DETECT_TERM <= cameraTilePosition.x)
		{
			if (deer.RandCreate())
				deer.CreateDeer(TilePosition(deer.rightLastCreatedTileXAxis + 5, GetLandHeight(deer.rightLastCreatedTileXAxis) + 1));

			deer.rightLastCreatedTileXAxis += DEER_TERM;
		}
		else if (deer.leftLastCreatedTileXAxis + MIN_NON_DETECT_TERM >= cameraTilePosition.x)
		{
			if (deer.RandCreate())
				deer.CreateDeer(TilePosition(deer.leftLastCreatedTileXAxis - 5, GetLandHeight(deer.leftLastCreatedTileXAxis) + 1));

			deer.leftLastCreatedTileXAxis -= DEER_TERM;
		}
	}
	else // currState == JE::NIGHT
	{
		deer.rightLastCreatedTileXAxis = cameraTilePosition.x + DEER_TERM;
		deer.leftLastCreatedTileXAxis  = cameraTilePosition.x - DEER_TERM;
	}
	/****************************************************************************************************************************************/

	// Test
	if (g_Input->IsPressed(JE_CONTROL))
	{
		// Draw tiles around the character
		if (g_Input->IsPressed(JE_1))
		{
			DrawVirtualGrid(tileIDs.unbreakableTileID, ConvertTileToWorldPosition(Get1stCharacterTilePosition()));
			DrawVirtualGrid(tileIDs.unbreakableTileID, ConvertTileToWorldPosition(Get2ndCharacterTilePosition()));
		}

		if (g_Input->IsTriggered(JE_2))
		{
			const unsigned int size = 3;

			Contents collisionArea[size*size];
			GetCollisionArea(Get1stCharacterTilePosition(), collisionArea, size);
			PrintCollisionAreaTileTypeInfo(collisionArea, 3);
			PrintCollisionAreaContentsInfo(collisionArea, 3);
		}

		if (g_Input->IsTriggered(JE_3))
		{
			JEPRINTF("Player1 Test\n");
			CoordinatesConversionTest(JE::CharacterManager::GetPlayerByID(JE::PLAYER_1ST).GetPosition());
			
			JEPRINTF("Player2 Test\n");
			CoordinatesConversionTest(JE::CharacterManager::GetPlayerByID(JE::PLAYER_2ND).GetPosition());
		}

		if (g_Input->IsTriggered(JE_4))
		{
			TileArrangementTest(rightLandEnd.x, leftLandEnd.x);
		}

		if (g_Input->IsTriggered(JE_5)) // Test land hight
		{
			JEPRINTF("Land's Hight Test\n");
			JEPRINTF("Player1 Y Position : %f\n", Get1stCharacterTilePosition().y);
			JEPRINTF("The Hight : %f\n", GetLandHeight(Get1stCharacterTilePosition().x));
		}
	}
}
void TileMapManager::ClearTiles(void)
{
	JEPRINTF("Tile Map is deleting with unloding images\n");
	UnloadTileMapImages();

	// Clear all objectData through all tiles
	for (TileMapItor iterator = tiles.begin(); iterator != tiles.end(); iterator++)
		iterator->second.objectData.clear();

	// Clear tiles
	tiles.clear();
}

void TileMapManager::LoadTileMapImages(void)
{
	// Tile Textures
	tileIDs.emptyID = 0;
	tileIDs.emptyDirtID = JE::GraphicLoadTexture("Texture//TY_EmptyDirt.tga");
	tileIDs.unbreakableTileID = JE::GraphicLoadTexture("Texture//TY_UnbreakableTile.tga");
	tileIDs.dirtID = JE::GraphicLoadTexture("Texture//TY_Dirt.tga");
	tileIDs.stoneID = JE::GraphicLoadTexture("Texture//TY_Stone.tga");

	
	resourceIDs = JE::ResourceSystem::GetResourceID();

	// Object Textures
	//objectIDs.shortTree = JE::GraphicLoadTexture("Texture//Object_ShortTree.tga");
	//objectIDs.longTree = JE::GraphicLoadTexture("Texture//Object_LongTree.tga");
}

void TileMapManager::UnloadTileMapImages(void)
{
	// Tile Textures
	JE::GraphicUnloadTexture(tileIDs.stoneID);
	JE::GraphicUnloadTexture(tileIDs.unbreakableTileID);
	JE::GraphicUnloadTexture(tileIDs.dirtID);
	JE::GraphicUnloadTexture(tileIDs.emptyDirtID);



	// Object Textures
	JE::GraphicUnloadTexture(objectIDs.shortTree);
	JE::GraphicUnloadTexture(objectIDs.longTree);
}

void TileMapManager::AddTile(TilePosition tilePosition, Contents contents)
{
	tiles.insert(PosConsPair(tilePosition, contents));
}

void TileMapManager::AddObject(TilePosition tilePosition, JE::OBJECTS objectType, JE::BaseObject* object)
{
	tiles.find(tilePosition)->second.objectData.push_back(ObjectData(objectType, object));
}

void TileMapManager::GenerateNormalTerrain(int startX, int length)
{
	int prevHeight = 0;
	const int initHight = (-INIT_HIGHT / 2);
	Contents contents(tileIDs.dirtID, 3);                     // Dirt, Empty
	Contents unbreakableTile(tileIDs.unbreakableTileID, 100); // UnbrakableTile, Empty

	// Right Direction
	if (startX == rightLandEnd.x + 1)
	{
		int flatLength = RandValue(4, 8);

		length = startX + length;
		for (int x = startX; x <= length; ++x)
		{
			// Flat State
			if (flatLength)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= prevHeight)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}

				--flatLength;
				rightLandEnd = TilePosition(x, prevHeight);
			}
			// Different height, hill, or valley State
			else
			{
				NextColumnType nextColumnType = (NextColumnType)RandValue(1, 3);

				if (nextColumnType == NCT_DIFFERENT_HEIGHT)
				{
					int differentHeight = prevHeight + RandValue(-2, 2);

					// For unbreakable tiles
					int unbreakableHight = RandValue(2, 5);
					for (int y = 0; y < unbreakableHight; y++)
						AddTile(TilePosition(x, initHight + y), unbreakableTile);

					// For dirt and empty
					for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
					{
						if (y <= differentHeight)
							AddTile(TilePosition(x, y), contents);
						else
							AddTile(TilePosition(x, y), Contents());
					}

					prevHeight = differentHeight;
					rightLandEnd = TilePosition(x, differentHeight);
				}
				else // NCT_HILL or NCT_VALLEY
				{
					int criticalPoint = prevHeight;
					int createdHelfLength = 0;
					int individualHeight = prevHeight;

					if (nextColumnType == NCT_HILL)
						criticalPoint += RandValue(3, 5);
					else // NCT_VALLEY
						criticalPoint -= RandValue(3, 5);

					for (; individualHeight != criticalPoint && x <= length; ++x, ++createdHelfLength)
					{
						// For unbreakable tiles
						int unbreakableHight = RandValue(2, 5);
						for (int y = 0; y < unbreakableHight; y++)
							AddTile(TilePosition(x, initHight + y), unbreakableTile);

						// For dirt and empty
						individualHeight += RandHillValleyHeight(nextColumnType);
						for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
						{
							if (y <= individualHeight)
								AddTile(TilePosition(x, y), contents);
							else
								AddTile(TilePosition(x, y), Contents());
						}
					}

					// Reverse
					int y = 0;
					for (; createdHelfLength >= 0 && x <= length; ++x, --createdHelfLength)
					{
						individualHeight -= RandHillValleyHeight(nextColumnType);
						for (y = -INIT_HIGHT / 2; y < INIT_HIGHT / 2; y++)
						{
							if (y <= individualHeight)
								AddTile(TilePosition(x, y), contents);
							else
								AddTile(TilePosition(x, y), Contents());
						}
					}

					if (tileMapManager.tiles.find(TilePosition(x, 0)) == tileMapManager.tiles.end())
						--x;

					rightLandEnd = TilePosition(x, y);
				}

				flatLength = RandValue(4, 8);
			}
		}
	}
	else // Left Direction(startX == leftLandEnd.x - 1) 
	{
		int flatLength = RandValue(4, 8);

		length = startX - length;
		for (int x = startX; x >= length; --x)
		{
			// Flat State
			if (flatLength)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= prevHeight)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}

				--flatLength;
				leftLandEnd = TilePosition(x, prevHeight);
			}
			// Different height, hill, or valley State
			else
			{
				NextColumnType nextColumnType = (NextColumnType)RandValue(1, 3);

				if (nextColumnType == NCT_DIFFERENT_HEIGHT)
				{
					int differentHeight = prevHeight + RandValue(-2, 2);

					// For unbreakable tiles
					int unbreakableHight = RandValue(2, 5);
					for (int y = 0; y < unbreakableHight; y++)
						AddTile(TilePosition(x, initHight + y), unbreakableTile);

					// For dirt and empty tiles
					for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
					{
						if (y <= differentHeight)
							AddTile(TilePosition(x, y), contents);
						else
							AddTile(TilePosition(x, y), Contents());
					}

					prevHeight = differentHeight;
					leftLandEnd = TilePosition(x, differentHeight);
				}
				else // NCT_HILL or NCT_VALLEY
				{
					int criticalPoint = prevHeight;
					int createdHelfLength = 0;
					int individualHeight = prevHeight;

					if (nextColumnType == NCT_HILL)
						criticalPoint += RandValue(3, 5);
					else // NCT_VALLEY
						criticalPoint -= RandValue(3, 5);

					for (; individualHeight != criticalPoint && x >= length; --x, ++createdHelfLength)
					{
						// For unbreakable tiles
						int unbreakableHight = RandValue(2, 5);
						for (int y = 0; y < unbreakableHight; y++)
							AddTile(TilePosition(x, initHight + y), unbreakableTile);

						// For dirt and empty tiles
						individualHeight += RandHillValleyHeight(nextColumnType);
						for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
						{
							if (y <= individualHeight)
								AddTile(TilePosition(x, y), contents);
							else
								AddTile(TilePosition(x, y), Contents());
						}
					}

					// Reverse
					int y = 0;
					for (; createdHelfLength >= 0 && x >= length; --x, --createdHelfLength)
					{
						// For unbreakable tiles
						int unbreakableHight = RandValue(2, 5);
						for (int y = 0; y < unbreakableHight; y++)
							AddTile(TilePosition(x, initHight + y), unbreakableTile);

						// For dirt and empty tiles
						individualHeight -= RandHillValleyHeight(nextColumnType);
						for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
						{
							if (y <= individualHeight)
								AddTile(TilePosition(x, y), contents);
							else
								AddTile(TilePosition(x, y), Contents());
						}
					}

					if (tileMapManager.tiles.find(TilePosition(x, 0)) == tileMapManager.tiles.end())
						++x;

					leftLandEnd = TilePosition(x, y);
				}

				flatLength = RandValue(4, 8);
			}
		}
	}
}

void TileMapManager::GenerateFlatTerrain(int startX, int length)
{
	int individualHeight = 0;
	const int initHight = (-INIT_HIGHT / 2);
	Contents contents(tileIDs.dirtID, 3);                     // Dirt, Empty
	Contents unbreakableTile(tileIDs.unbreakableTileID, 100); // UnbrakableTile, Empty

	// Right Direction
	if (startX == rightLandEnd.x + 1)
	{
		length = startX + length;
		for (int x = startX; x <= length; ++x)
		{
			// For unbreakable tiles
			int unbreakableHight = RandValue(2, 5);
			for (int y = 0; y < unbreakableHight; y++)
				AddTile(TilePosition(x, initHight + y), unbreakableTile);

			// For dirt and empty tiles
			individualHeight += RandFlatHeight();
			for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
			{
				if (y <= individualHeight)
					AddTile(TilePosition(x, y), contents);
				else
					AddTile(TilePosition(x, y), Contents());
			}
		}

		rightLandEnd = TilePosition(length, individualHeight);
	}
	else // Left Direction(startX == leftLandEnd.x - 1) 
	{
		length = startX - length;
		for (int x = startX; x >= length; --x)
		{
			// For unbreakable tiles
			int unbreakableHight = RandValue(2, 5);
			for (int y = 0; y < unbreakableHight; y++)
				AddTile(TilePosition(x, initHight + y), unbreakableTile);

			// For dirt and empty tiles
			individualHeight += RandFlatHeight();
			for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
			{
				if (y <= individualHeight)
					AddTile(TilePosition(x, y), contents);
				else
					AddTile(TilePosition(x, y), Contents());
			}
		}

		leftLandEnd = TilePosition(length, individualHeight);
	}
}

void TileMapManager::GenerateDifficultTerrain(int startX)
{
	const int initHight = (-INIT_HIGHT / 2);
	Contents contents(tileIDs.dirtID, 3);                     // Dirt, Empty
	Contents unbreakableTile(tileIDs.unbreakableTileID, 100); // UnbrakableTile, Empty

	// Rand Land Type & height
	int criticalPoint = 0;
	LandType landType = LT_BEGINE;
	int randValue = RandOutOf100();

	if (16 <= randValue && randValue <= 80) // 65%
	{
		landType = LT_NORMAL;
		criticalPoint = RandValue(5, 10);
	}
	else if (81 <= randValue && randValue <= 95) // 15%
	{
		landType = LT_HIGH;
		criticalPoint = RandValue(11, 20);

	}
	else if (6 <= randValue && randValue <= 15) // 10%
	{
		landType = LT_LOW;
		criticalPoint = RandValue(-14, -5);
	}
	else if (1 <= randValue && randValue <= 5) // 5%
	{
		landType = LT_LOWER;
		criticalPoint = RandValue(-19, -15);
	}
	else // 5%
	{
		landType = LT_HIGHER;
		criticalPoint = RandValue(21, 25);
	}

	int individualHeight = 0;

	// Right Direction
	if (startX == rightLandEnd.x + 1)
	{
		if (criticalPoint >= 0) // Over the ground 
		{
			int x = 0;
			for (x = startX; individualHeight <= criticalPoint; x++)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				individualHeight += RandDifficultHeight(landType);
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= individualHeight && y <= criticalPoint)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}
			}

			// Reverse
			for (x = startX; individualHeight >= 0; x++)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				individualHeight -= RandDifficultHeight(landType);
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= individualHeight && y <= criticalPoint)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}
			}

			if (tileMapManager.tiles.find(TilePosition(x, 0)) == tileMapManager.tiles.end())
				--x;

			rightLandEnd = TilePosition(x, individualHeight);
		}
		else // Under the ground
		{
			for (int x = startX; individualHeight >= criticalPoint; x++)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				individualHeight += RandDifficultHeight(landType);
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= individualHeight || y <= criticalPoint)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}
			}

			// Reverse
			int x = 0;
			for (x = startX; individualHeight <= 0; x++)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				individualHeight -= RandDifficultHeight(landType);
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= individualHeight || y <= criticalPoint)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}
			}

			if (tileMapManager.tiles.find(TilePosition(x, 0)) == tileMapManager.tiles.end())
				--x;

			rightLandEnd = TilePosition(x, individualHeight);
		}
	}
	else // Left Direction(startX == leftLandEnd.x - 1) 
	{
		if (criticalPoint >= 0) // Over the ground 
		{
			for (int x = startX; individualHeight <= criticalPoint; --x)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				individualHeight += RandDifficultHeight(landType);
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= individualHeight && y <= criticalPoint)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}
			}

			// Reverse
			int x = 0;
			for (x = startX; individualHeight >= 0; --x)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				individualHeight -= RandDifficultHeight(landType);
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= individualHeight && y <= criticalPoint)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}
			}

			if (tileMapManager.tiles.find(TilePosition(x, 0)) == tileMapManager.tiles.end())
				++x;

			leftLandEnd = TilePosition(x, individualHeight);
		}
		else // Under the ground
		{
			for (int x = startX; individualHeight >= criticalPoint; --x)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				individualHeight += RandDifficultHeight(landType);
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= individualHeight || y <= criticalPoint)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}
			}

			// Reverse
			int x = 0;
			for (x = startX; individualHeight <= 0; --x)
			{
				// For unbreakable tiles
				int unbreakableHight = RandValue(2, 5);
				for (int y = 0; y < unbreakableHight; y++)
					AddTile(TilePosition(x, initHight + y), unbreakableTile);

				// For dirt and empty tiles
				individualHeight -= RandDifficultHeight(landType);
				for (int y = initHight + unbreakableHight; y < INIT_HIGHT / 2; y++)
				{
					if (y <= individualHeight || y <= criticalPoint)
						AddTile(TilePosition(x, y), contents);
					else
						AddTile(TilePosition(x, y), Contents());
				}
			}

			if (tileMapManager.tiles.find(TilePosition(x, 0)) == tileMapManager.tiles.end())
				++x;

			leftLandEnd = TilePosition(x, individualHeight);
		}
	}
}

int RandValue(const int min, const int max)
{
	return rand() % (max - min) + min + 1;
}

int RandOutOf10(void)
{
	return rand() % 10 + 1;
}

int RandOutOf100(void)
{
	return rand() % 100 + 1;
}

int TileMapManager::RandFlatHeight(void) const
{
	int randValue = RandOutOf10();

	if (randValue == 5) // 10%
		return 1;
	else if (randValue == 6) // 10%
		return -1;

	return 0; // 80%
}

int TileMapManager::RandHillValleyHeight(const NextColumnType nextColumnType) const
{
	int randValue = RandOutOf10();

	if (1 <= randValue && randValue <= 6) // 60%
		if (nextColumnType == NCT_HILL)
			return 1;
		else // NCT_VALLEY
			return -1;

	return 0; // 40%
}

int TileMapManager::RandDifficultHeight(const LandType landType) const
{
	int randValue = RandOutOf100();

	if (landType == LT_NORMAL)
	{
		if (1 <= randValue && randValue <= 50) // 50%
			return 0;
		else if (51 <= randValue && randValue <= 80) // 30%
			return 1;

		return -1; // 20%
	}
	else if (landType == LT_HIGH)
	{
		if (1 <= randValue && randValue <= 40) // 40%
			return 0;
		else if (41 <= randValue && randValue <= 70) // 30%
			return -1;
		else if (71 <= randValue && randValue <= 90) // 20%
			return 1;

		return 2; // 10%
	}
	else if (landType == LT_LOW)
	{
		if (1 <= randValue && randValue <= 40) // 40%
			return 0;
		else if (41 <= randValue && randValue <= 70) // 30%
			return 1;
		else if (71 <= randValue && randValue <= 90) // 20%
			return -1;

		return -2; // 10%
	}
	else if (landType == LT_HIGHER)
	{
		if (1 <= randValue && randValue <= 30) // 30%
			return 1;
		else if (31 <= randValue && randValue <= 50) // 20%
			return 0;
		else if (51 <= randValue && randValue <= 70) // 20%
			return -1;
		else if (71 <= randValue && randValue <= 85) // 15%
			return 2;
		else if (86 <= randValue && randValue <= 95) // 10%
			return 3;

		return -2; // 5%
	}

	// LT_LOWER
	if (1 <= randValue && randValue <= 30) // 30%
		return -1;
	else if (31 <= randValue && randValue <= 50) // 20%
		return 0;
	else if (51 <= randValue && randValue <= 70) // 20%
		return 1;
	else if (71 <= randValue && randValue <= 85) // 15%
		return -2;
	else if (86 <= randValue && randValue <= 95) // 10%
		return -3;

	return 2; // 5%
}

TilePosition ConvertWorldToTilePosition(JEPoint2D position)
{
	if (position.x < 0.f)
		position.x -= 10.f;

	if (position.y < 0.f)
		position.y -= 10.f;

	return TilePosition(((int)position.x / TILE_SPACE), ((int)position.y / TILE_SPACE));
}

JEPoint2D ConvertTileToWorldPosition(TilePosition tilePosition)
{
	//if (tilePosition.x < 0.f)
		++tilePosition.x;

	//if (tilePosition.y < 0.f)
		++tilePosition.y;

	return JEPoint2D((tilePosition.x * TILE_SPACE), (tilePosition.y * TILE_SPACE));
}

JEPoint2D RightConvertTileToWorldPosition(TilePosition tilePosition)
{
	if (tilePosition.x < 0.f)
	++tilePosition.x;

	if (tilePosition.y < 0.f)
	++tilePosition.y;

	return JEPoint2D((tilePosition.x * TILE_SPACE), (tilePosition.y * TILE_SPACE));
}

JEPoint2D FindImageGap(JEPoint2D position)
{
	if (position.x > 0.f)
		position.x -= TILE_SPACE;

	if (position.y > 0.f)
		position.y -= TILE_SPACE;

	return position;
}

void TileMapManager::DrawVirtualGrid(int imageID, JEPoint2D cameraPosition)
{
	// CHARACTER_COLLISION_AREA is the private collsion area.
	const int GRID_SIZE = 3;
	double gridSpace = (GRID_SIZE / 2 + 1) * TILE_SPACE;
	JEPoint2D gridPositionMin = cameraPosition - JEPoint2D(gridSpace, gridSpace);
	JEPoint2D gridPositionMax = cameraPosition + JEPoint2D(gridSpace, gridSpace);

	for (double x = gridPositionMin.x; x < gridPositionMax.x; x += TILE_SPACE)
	{
		JE::GraphicSetTextureUnit(0);
		JE::GraphicSetTexture(imageID);
		JE::GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetPersTransform(x, cameraPosition.y - (TILE_SPACE / 2), 0.f,
			0.25f, gridSpace * 2.f - TILE_SPACE, 0.f);
	}

	for (double y = gridPositionMin.y; y < gridPositionMax.y; y += TILE_SPACE)
	{
		JE::GraphicSetTextureUnit(0);
		JE::GraphicSetTexture(imageID);
		JE::GraphicSetFilpTex(JE::FLIP_IDLE, 1);
		JE::GraphicSetAnimaTex(1, 0);
		JE::GraphicSetPersTransform(cameraPosition.x - (TILE_SPACE / 2), y, 0.f,
			gridSpace * 2.f - TILE_SPACE, 0.25f, 0.f);
	}
}

void TileMapManager::DrawTiles(const JEPoint2D cameraPosition)
{
	TilePosition tilePosition(ConvertWorldToTilePosition(cameraPosition));
	TilePosition maxPosition = tilePosition + TilePosition(18, 12);
	TilePosition minPosition = tilePosition - TilePosition(17, 10);

	// Draw limitation of left & right side
	if (minPosition.x < leftLandEnd.x && minPosition.x < rightLandEnd.x)
		minPosition.x = leftLandEnd.x;
	else if (maxPosition.x > leftLandEnd.x && maxPosition.x > rightLandEnd.x)
		maxPosition.x = rightLandEnd.x;

	// Draw limitation of down & up side
	int hightLimit = INIT_HIGHT / 2;
	if (minPosition.y < -hightLimit && minPosition.y < hightLimit)
		minPosition.y = -hightLimit;
	else if (maxPosition.y > -hightLimit && maxPosition.y > hightLimit)
		maxPosition.y = hightLimit;

	for (tilePosition.x = minPosition.x; tilePosition.x < maxPosition.x; ++tilePosition.x)
	{
		for (tilePosition.y = minPosition.y; tilePosition.y < maxPosition.y; ++tilePosition.y)
		{
			JEPoint2D drawPosition = ConvertTileToWorldPosition(tilePosition) - JEPoint2D(TILE_SPACE / 2, TILE_SPACE / 2);
			TileMapCItor targetTile = tiles.find(tilePosition);
			int tileType = targetTile->second.tileIDs;

			// Drawing a tile
			if (tileType != tileIDs.emptyID)
			{
				JE::GraphicSetTextureUnit(0);
				JE::GraphicSetTexture(tileType);
				JE::GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				JE::GraphicSetAnimaTex(1, 0);
				JE::GraphicSetPersTransform(drawPosition.x, drawPosition.y, 0.f,
					TILE_SPACE, TILE_SPACE, 0.f);
			}

			// Drawing a mineral
			if (targetTile->second.mineral)
			{
				/*if (GetCurrState() == NIGHT)
				{


				}*/
				JE::GraphicSetTextureUnit(0);
				JE::GraphicSetTexture(resourceIDs.mineral);
				JE::GraphicSetFilpTex(JE::FLIP_IDLE, 1);
				JE::GraphicSetAnimaTex(1, 0);
				JE::GraphicSetPersTransform(drawPosition.x, drawPosition.y, 0.f,
					TILE_SPACE, TILE_SPACE, 0.f);
			}
		}
	}
}

void TileMapManager::PrintCharactersTilePosition(void)
{
	TilePosition tilePosition1 = Get1stCharacterTilePosition();
	TilePosition tilePosition2 = Get2ndCharacterTilePosition();

	//JEPRINTF();
	std::cout << "P1: (" << tilePosition1.x << ", " << tilePosition1.y << ") / " << std::endl;
	std::cout << "P2: (" << tilePosition2.x << ", " << tilePosition2.y << ") / " << std::endl;
	std::cout << "Left End: (" << leftLandEnd.x << ", " << leftLandEnd.y << "), Right End: (" << rightLandEnd.x << ", " << rightLandEnd.y << ")" << std::endl;
}

void TileMapManager::PrintCollisionAreaTileTypeInfo(const Contents* collisionArea, unsigned int size)
{
	std::cout << "<CollisionArea TileType>" << std::endl;

	unsigned int arraySize = size * size;

	for (unsigned int i = 0; i < arraySize; i++)
		std::cout << collisionArea[i].tileIDs << " ";

	std::cout << "" << std::endl;
}

void TileMapManager::PrintCollisionAreaContentsInfo(const Contents* collisionArea, unsigned int size)
{
	std::cout << "<CollisionArea Contents>" << std::endl;

	unsigned int arraySize = size * size;

	for (unsigned i = 0; i < arraySize; i++)
	{
		if (!collisionArea[i].objectData.empty()) // if the data is empty, print the info
		{
			for (std::vector<ObjectData>::const_iterator it = collisionArea[i].objectData.begin(); it != collisionArea[i].objectData.end(); ++it)
			{
				std::cout << "No." << i;
				std::cout << "/ Type:" << it->objectType << std::endl;
				//std::cout << "/ Position:" << it->object->position.x << ", " << it->object->position.y << std::endl;
			}
		}
	}
}

TilePosition TileMapManager::Get1stCharacterTilePosition(void)
{
	return ConvertWorldToTilePosition(JE::CharacterManager::GetPlayerByID(JE::PLAYER_1ST).GetPosition());
}

TilePosition TileMapManager::Get2ndCharacterTilePosition(void)
{
	return ConvertWorldToTilePosition(JE::CharacterManager::GetPlayerByID(JE::PLAYER_2ND).GetPosition());
}

bool TileMapManager::GetTileMineral(TilePosition tilePosition)
{
	TileMapCItor targetTile = tiles.find(tilePosition);

	if (targetTile == tiles.end())
		return false;

	if (targetTile->second.mineral == true)
		return true;

	return false;
}

int TileMapManager::GetTileType(TilePosition tilePosition)
{
	return tiles[tilePosition].tileIDs;
}

void TileMapManager::GetCollisionArea(TilePosition tilePosition, Contents* collisionArea, unsigned int size)
{
	int i = 0;
	int maxSize = size / 2;

	for (int y = tilePosition.y + maxSize; y >= tilePosition.y - maxSize; y--)
	{
		for (int x = tilePosition.x - maxSize; x <= tilePosition.x + maxSize; x++)
		{
			Contents content = tileMapManager.tiles[TilePosition(x, y)];

			collisionArea[i].tileIDs = content.tileIDs;
			collisionArea[i].tileHP = content.tileHP;
			collisionArea[i].objectData = content.objectData;

			++i;
		}
	}
}

JEPoint2D TileMapManager::GetMolePosition(void)
{
	return mole.GetMolePosition();
}

void TileMapManager::SetTileType(TilePosition tilePosition, int tileID)
{
	if (GetTileType(tilePosition) == tileID)
		return;

	if (tileID == tileIDs.emptyID)
	{
		tiles[tilePosition].tileHP = 0;
		tiles[tilePosition].mineral = false;
	}
	else if (tileID == tileIDs.emptyDirtID)
	{
		if (GetLandHeight(tilePosition.x) < tilePosition.y)
			tileID = tileIDs.emptyID;

		tiles[tilePosition].tileHP = 0;
		tiles[tilePosition].mineral = false;
	}
	else if (tileID == tileIDs.dirtID)
		tiles[tilePosition].tileHP = 3;
	else if (tileID == tileIDs.stoneID)
		tiles[tilePosition].tileHP = 5;

	tiles[tilePosition].tileIDs = tileID;
}

void TileMapManager::SetTileMieral(TilePosition tilePosition, bool mineral)
{
	TileMapItor targetTile = tiles.find(tilePosition);

	if (targetTile == tiles.end())
		return;

	targetTile->second.mineral = mineral;
}

/****************************************************************************************************************************************/

void Stone::ChangingStone(TilePosition position)
{
	TileMapManager* pTileMapManager = &GetTileMapManager();

	TilePosition tilePosition[4];
	tilePosition[0] = position + TilePosition(0, 1);
	tilePosition[1] = position + TilePosition(1, 0);
	tilePosition[2] = position + TilePosition(0, -1);
	tilePosition[3] = position + TilePosition(-1, 0);

	const int value = rand() % 4;
	const int constStoneID = pTileMapManager->tileIDs.stoneID;

	// Frist Phase
	if (!value)
	{
		if (pTileMapManager->GetTileType(tilePosition[0]) != constStoneID)
		{
			pTileMapManager->SetTileType(tilePosition[0], constStoneID);
			ChangingStone(tilePosition[0]);
		}
	}

	if (!value)
	{
		if (pTileMapManager->GetTileType(tilePosition[1]) != constStoneID)
		{
			pTileMapManager->SetTileType(tilePosition[1], constStoneID);
			ChangingStone(tilePosition[1]);
		}
	}

	if (!value)
	{
		if (pTileMapManager->GetTileType(tilePosition[2]) != constStoneID)
		{
			pTileMapManager->SetTileType(tilePosition[2], constStoneID);
			ChangingStone(tilePosition[2]);
		}
	}

	if (!value)
	{
		if (pTileMapManager->GetTileType(tilePosition[3]) != constStoneID)
		{
			pTileMapManager->SetTileType(tilePosition[3], constStoneID);
			ChangingStone(tilePosition[3]);
		}
	}

	// Second Phase
	switch (value)
	{
	case 0:
		if (pTileMapManager->GetTileType(tilePosition[0]) != constStoneID)
		{
			pTileMapManager->SetTileType(tilePosition[0], constStoneID);
			ChangingStone(tilePosition[0]);
		}

		break;

	case 1:
		if (pTileMapManager->GetTileType(tilePosition[1]) != constStoneID)
		{
			pTileMapManager->SetTileType(tilePosition[1], constStoneID);
			ChangingStone(tilePosition[1]);
		}

		break;

	case 2:
		if (pTileMapManager->GetTileType(tilePosition[2]) != constStoneID)
		{
			pTileMapManager->SetTileType(tilePosition[2], constStoneID);
			ChangingStone(tilePosition[2]);
		}

		break;

	case 3:
		if (pTileMapManager->GetTileType(tilePosition[3]) != constStoneID)
		{
			pTileMapManager->SetTileType(tilePosition[3], constStoneID);
			ChangingStone(tilePosition[3]);
		}

		break;
	}
}

bool Stone::RandChanging(void)
{
	int value = RandOutOf100();

	switch (value)
	{
	case 1: // 5%
	case 2:
	case 3:
	case 4:
	case 5:
		return false;
	default: // 95%
		return true;
	}
}

/****************************************************************************************************************************************/

void Deer::CreateDeer(const TilePosition position)
{
	JE::ObjectManager::AddObject(JE::OBJECTS::OBJ_DEER, new JE::Deer(ConvertTileToWorldPosition(position)));
}

bool Deer::RandCreate(void)
{
	int value = RandOutOf10();

	switch (value)
	{
	case 1: // 40%
	case 2:
	case 3:
	case 4:
		return false;
	default: // 60%
		return true;
	}
}

/****************************************************************************************************************************************/

void Barrel::DeployShop(const TilePosition position)
{
	TilePosition randPosition(RandValue(-5, 5), RandValue(0, 5));
	JE::ObjectManager::AddObject(JE::OBJECTS::OBJ_SHOP, new JE::Shop(ConvertTileToWorldPosition(position + randPosition)));


}

bool Barrel::RandDeploy(void)
{
	int value = RandOutOf10();

	switch (value)
	{
	case 1: // 30%
	case 2:
	case 3:
		return false;
	default: // 70%
		return true;
	}
}

/****************************************************************************************************************************************/

void Tree::PlantingTree(const TilePosition position, const JE::OBJECTS treeType)
{
	TileMapManager* pTileMapManager = &GetTileMapManager();

	if (treeType == JE::OBJECTS::OBJ_SHORT_TREE)
	{
		// Check Unable or able
		if (pTileMapManager->GetTileType(TilePosition(position.x, position.y + 1)) != pTileMapManager->tileIDs.emptyID)
			return;

		// Add a tree
		JEPoint2D treeWorldPosition = ConvertTileToWorldPosition(position);
		JE::ObjectManager::AddObject(treeType, new JE::ShortTree(treeWorldPosition));
		//JE::ObjectManager::LastAddObject()->textureID = pTileMapManager->objectIDs.shortTree;
	}
	else
	{
		// Check Unable or able
		if (pTileMapManager->GetTileType(TilePosition(position.x, position.y + 1)) != pTileMapManager->tileIDs.emptyID
			|| pTileMapManager->GetTileType(TilePosition(position.x, position.y + 2)) != pTileMapManager->tileIDs.emptyID)
			return;

		// Add a tree
		JEPoint2D treeWorldPosition = ConvertTileToWorldPosition(position);
		JE::ObjectManager::AddObject(treeType, new JE::LongTree(treeWorldPosition));
		//JE::ObjectManager::LastAddObject()->textureID = pTileMapManager->objectIDs.longTree;
	}
}

JE::OBJECTS Tree::RandPlanting(void)
{
	int value = RandOutOf10();

	switch (value)
	{
	case 1: // 30%
	case 2:
	case 3:
		return JE::OBJECTS::OBJ_END;
	case 4: // 20%
	case 5:
		return JE::OBJECTS::OBJ_LONG_TREE;
	default: // 50%
		return JE::OBJECTS::OBJ_SHORT_TREE;
	}
}

/****************************************************************************************************************************************/

void Mineral::SowingAMineral(TilePosition position)
{
	if (RandSowing())
	{
		TileMapManager* pTileMapManager = &GetTileMapManager();

		TilePosition tilePosition[4];
		tilePosition[0] = position + TilePosition(0, 1);
		tilePosition[1] = position + TilePosition(1, 0);
		tilePosition[2] = position + TilePosition(0, -1);
		tilePosition[3] = position + TilePosition(-1, 0);

		int value = rand() % 4;
		const int constDirtID = pTileMapManager->tileIDs.dirtID;

		switch (value)
		{
		case 0:
			if (!pTileMapManager->GetTileMineral(tilePosition[0])
				&& pTileMapManager->GetTileType(tilePosition[0]) == constDirtID)
			{
				pTileMapManager->SetTileMieral(tilePosition[0], true);
				SowingAMineral(tilePosition[0]);
			}

			break;

		case 1:
			if (!pTileMapManager->GetTileMineral(tilePosition[1])
				&& pTileMapManager->GetTileType(tilePosition[1]) == constDirtID)
			{
				pTileMapManager->SetTileMieral(tilePosition[1], true);
				SowingAMineral(tilePosition[1]);
			}

			break;

		case 2:
			if (!pTileMapManager->GetTileMineral(tilePosition[2])
				&& pTileMapManager->GetTileType(tilePosition[2]) == constDirtID)
			{
				pTileMapManager->SetTileMieral(tilePosition[2], true);
				SowingAMineral(tilePosition[2]);
			}

			break;

		case 3:
			if (!pTileMapManager->GetTileMineral(tilePosition[3])
				&& pTileMapManager->GetTileType(tilePosition[3]) == constDirtID)
			{
				pTileMapManager->SetTileMieral(tilePosition[3], true);
				SowingAMineral(tilePosition[3]);
			}

			break;
		}
	}
	else
		return;
}

bool Mineral::RandSowing(void)
{
	int value = RandOutOf100();

	switch (value)
	{
	case 1: // 3%
	case 2:
	case 3:
		return false;
	default: // 97%
		return true;
	}
}

/****************************************************************************************************************************************/

int Bird::RandBirdTerm(void)
{
	return RandValue(20, 80);
}

int Bird::RandHeightTerm(double totalTime)
{
	((unsigned)(totalTime * 10 * (rand() % (90) + 11)));

	int height = RandOutOf10();

	switch (height)
	{
	case 1: // 40%
	case 2:
	case 3:
	case 4:
		return 0;
	default: // 60%
		return 1;
	}
}

int GetLandHeight(int x)
{
	TileMapManager* tileMapManager = &GetTileMapManager();

	int y = -(INIT_HIGHT / 2) + 1;
	for (int tileType = tileMapManager->tiles[TilePosition(x, y)].tileIDs; tileType != tileMapManager->tileIDs.emptyID; ++y)
		tileType = tileMapManager->tiles[TilePosition(x, y)].tileIDs;

	return y - 2;
}

double Bird::GetSlope(TilePosition startPosition, TilePosition endPosition)
{
	double x = endPosition.x - startPosition.x;
	double y = endPosition.y - startPosition.y;

	return y / x;
}

/****************************************************************************************************************************************/

Mole::Mole(void)
{
	moleID = JE::GraphicLoadTexture("Texture//Resource_Meat.tga");
}

Mole::~Mole(void)
{
	//std::cout << "The Mole is dystroyed." << std::endl;

	JE::GraphicUnloadTexture(moleID);
}

void Mole::InitMole(JEPoint2D cameraPosition, JEPoint2D randVelocity)
{
	std::cout << "The Mole is created." << std::endl;

	double x = 10 * cameraPosition.x;
	int rounded = (int)(x + 0.5);

	radius = 5.f;
	position = JEPoint2D(rounded, -MOLE_TERM);
	velocity = randVelocity;
	lifeTime = MOLE_LIFT_TIME;
	isAlive = true;

	// Set the each last Created tile x axis
	rightLastCreatedTileXAxis = rounded + MOLE_TERM;
	leftLastCreatedTileXAxis = rounded - MOLE_TERM;
}

void Mole::UpdateMole(const JEPoint2D cameraPosition, const double totalTime, const double dt)
{
	srand((unsigned)(totalTime * 10));

	// Reposition
	if (!isAlive)
	{
		if (rightLastCreatedTileXAxis <= cameraPosition.x)
		{
			std::cout << "Right" << std::endl;

			lifeTime = MOLE_LIFT_TIME;
			isAlive = true;

			JEPoint2D newCreatingPosition = JEPoint2D(rightLastCreatedTileXAxis, -MOLE_TERM) + JEPoint2D(MOLE_TERM, 0.f);
			position = newCreatingPosition;
			rightLastCreatedTileXAxis = position.x + MOLE_TERM;
		}
		else if (cameraPosition.x < leftLastCreatedTileXAxis)
		{
			std::cout << "Left" << std::endl;

			lifeTime = MOLE_LIFT_TIME;
			isAlive = true;

			JEPoint2D newCreatingPosition = JEPoint2D(leftLastCreatedTileXAxis, -MOLE_TERM) + JEPoint2D(-MOLE_TERM, 0.f);
			position = newCreatingPosition;
			leftLastCreatedTileXAxis = position.x - MOLE_TERM;
		}
	}
	else // isAlive
	{
		if (lifeTime <= 0.f)
			isAlive = false;

		position = position + velocity;

		if (((int)(totalTime * 10)) % 2 == 0)
			velocity = JEPoint2D((double)(rand() % (MOLE_SPEED - -MOLE_SPEED) + -MOLE_SPEED + 1),
			(double)(rand() % ((MOLE_SPEED / 2) - -(MOLE_SPEED / 2)) + -(MOLE_SPEED / 2) + 1));

		lifeTime -= dt;
	}
}

JEPoint2D Mole::GetMolePosition(void)
{
	return position;
}

JEPoint2D Mole::GetMoleVelocity(void)
{
	return velocity;
}

void Mole::SetMolePosition(JEPoint2D position)
{
	this->position = position;
}

void Mole::SetMoleVelocity(JEPoint2D velocity)
{
	this->velocity = velocity;
}

/****************************************************************************************************************************************/

TileMapManager& GetTileMapManager(void)
{
	return tileMapManager;
}

/****************************************************************************************************************************************/
// TestFunctions

void CoordinatesConversionTest(const JEPoint2D playerPosition)
{
	std::cout << "*******************************" << std::endl;
	std::cout << "Player World Position : " << (int)playerPosition.x << ", " << (int)playerPosition.y << std::endl;

	TilePosition playerTilePosition = ConvertWorldToTilePosition(playerPosition);
	JEPoint2D playerWorldPosition(playerTilePosition.x, playerTilePosition.y);// = ConvertTileToWorldPosition(playerTilePosition);

	if (playerWorldPosition.x < 0.f)
		++playerWorldPosition.x;

	if (playerWorldPosition.y < 0.f)
		++playerWorldPosition.x;

	playerWorldPosition.x *= TILE_SPACE;
	playerWorldPosition.y *= TILE_SPACE;

	if ((int)playerPosition.x != playerWorldPosition.x || (int)playerPosition.y != playerWorldPosition.y)
	{
		std::cout << "Conversion Error" << std::endl;
		std::cout << "Converted TilePosition  : " << playerTilePosition.x << ", " << playerTilePosition.y << std::endl;
		std::cout << "Converted WorldPosition : " << playerWorldPosition.x << ", " << playerWorldPosition.y << std::endl;
	}

	std::cout << "Coordinates Conversion Test Completed" << std::endl;
	std::cout << "*******************************" << std::endl;
}

void TileArrangementTest(const int rightLandEndX, const int leftLandEndX)
{
	std::cout << "*******************************" << std::endl;
	std::cout << "LandEndXs : " << leftLandEndX << ", " << rightLandEndX << std::endl;

	for (int x = leftLandEndX; x <= rightLandEndX; x++)
	{
		if (tileMapManager.tiles.find(TilePosition(x, 0)) == tileMapManager.tiles.end())
		{
			if (tileMapManager.tiles.find(TilePosition(x - 1, 0)) == tileMapManager.tiles.end())
				std::cout << "!! The Empty Column : " << x - 1 << std::endl;
			std::cout << "!! The Empty Column : " << x << std::endl;
			if (tileMapManager.tiles.find(TilePosition(x + 1, 0)) == tileMapManager.tiles.end())
				std::cout << "!! The Empty Column : " << x + 1 << std::endl;
		}
	}

	std::cout << "Tile Arrangement Test Completed" << std::endl;
	std::cout << "*******************************" << std::endl;
}