/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JETileMap.h
author      Minsu Kang
email       minsu9486@gmail.com
course      GAM250

*******************************************************************************/

#ifndef JENGA_ENGINE_TILEMAP
#define JENGA_ENGINE_TILEMAP

#include <map>
#include <vector>
#include <list>
#include "JEObjects.h"
#include "JEObjectManager.h"
#include "JEObjectTypes.h"
#include "JEDebug.h"
#include "JETileMapStructs.h"
#include "JEResource.h"
#include "JEDayAndNight.h"

struct ObjectData
{
	ObjectData(JE::OBJECTS _objectType) : objectType(_objectType) {}
	ObjectData(JE::OBJECTS _objectType, JE::BaseObject* _object) : objectType(_objectType), object(_object) {}

	JE::OBJECTS objectType;
	JE::BaseObject* object;
};

struct Contents
{
	// 0 = emptyID
	Contents(void){ tileIDs = 0; tileHP = 0; mineral = false; };
	Contents(int _tileIDs, int _tileHP){ tileIDs = _tileIDs; tileHP = _tileHP; mineral = false; };

	int tileIDs;
	int tileHP;
	bool mineral;
	std::vector<ObjectData> objectData;
};
typedef std::vector<ObjectData>::iterator   ObjVecItor;
typedef std::pair<TilePosition, Contents>   PosConsPair;
typedef std::map<TilePosition, Contents>    TileMap;
typedef TileMap::iterator					TileMapItor;
typedef TileMap::const_iterator				TileMapCItor;

// For Changing stones
struct Stone
{
	void ChangingStone(TilePosition position);
	bool RandChanging(void);

	int rightLastCreatedTileXAxis;
	int leftLastCreatedTileXAxis;
};

// For creating deers
struct Deer
{
	void CreateDeer(const TilePosition position);
	bool RandCreate(void);

	int rightLastCreatedTileXAxis;
	int leftLastCreatedTileXAxis;
};

// For deploying shops
struct Barrel
{
	void DeployShop(const TilePosition position);
	bool RandDeploy(void);

	int rightLastCreatedTileXAxis;
	int leftLastCreatedTileXAxis;
};

// For planting trees
struct Tree
{
	void PlantingTree(const TilePosition position, const JE::OBJECTS treeType);
	JE::OBJECTS RandPlanting(void);

	int rightLastCreatedTileXAxis;
	int leftLastCreatedTileXAxis;
};

// For sowing minerals
struct Mineral
{
	void SowingAMineral(TilePosition position);
	bool RandSowing(void);

	int rightLastCreatedTileXAxis;
	int leftLastCreatedTileXAxis;
};

// For floating small terrain
struct Bird
{
	int RandBirdTerm(void);
	int RandHeightTerm(const double totalTime);
	friend int GetLandHeight(int x);
	double GetSlope(TilePosition startPosition, TilePosition endPosition);

	int rightLastCreatedTileXAxis;
	int leftLastCreatedTileXAxis;
};

// For underground
class Mole
{
public:
	Mole(void);
	~Mole(void);

	void InitMole(const JEPoint2D cameraPosition, JEPoint2D randVelocity);
	void UpdateMole(const JEPoint2D cameraPosition, const double totalTime, const double dt);

	JEPoint2D GetMolePosition(void);
	JEPoint2D GetMoleVelocity(void);

	void SetMolePosition(JEPoint2D position);
	void SetMoleVelocity(JEPoint2D velocity);

private:
	int moleID;
	double radius;
	JEPoint2D position;
	JEPoint2D velocity;
	double lifeTime;
	bool isAlive;

	double rightLastCreatedTileXAxis;
	double leftLastCreatedTileXAxis;
};

class TileMapManager
{
public:
	TileMap tiles;

	TileIDs     tileIDs;
	JE::ResourceID resourceIDs;
	ObjectIDs   objectIDs;

	TileMapManager(void);
	~TileMapManager(void);

	void InitTiles(JE::ObjectManager::ObjMap objMap);
	void InitTiles_2(JE::ObjectManager::ObjMap objMap);
	void InitTiles_OpenWorld(JEPoint2D cameraPosition);
	void UpdateTiles(double totalTime);
	void UpdateOpenWorldTiles(const JEPoint2D cameraPosition, const double totalTime, const JE::DAY_NIGHT currState, const double dt);
	void ClearTiles(void);
	void LoadTileMapImages(void);
	void UnloadTileMapImages(void);

	inline void AddTile(TilePosition tilePosition, Contents contents);
	inline void AddObject(TilePosition tilePosition, JE::OBJECTS objectType, JE::BaseObject* object);

	friend void UpdateCollisionArea(const TileMapManager& tileMapManager, TilePosition tilePosition, Contents* collisionArea);

	// It generate multiply. (flatland, hill, valley, and ... cliff as bug)
	void GenerateNormalTerrain(int startX, int length);
	// It starts from the given x to a random y until the length. (only flatland)
	void GenerateFlatTerrain(int startX, int length);
	// It starts from the given x to a random x around y-axis's 0. (HIGHER,	HIGH, NORMAL, LOW, or LOWER)
	void GenerateDifficultTerrain(int startX);

	// Rand Functions
	friend inline int RandValue(const int min, const int max);
	friend inline int RandOutOf10(void);
	friend inline int RandOutOf100(void);

	inline int RandFlatHeight(void) const;
	inline int RandHillValleyHeight(const NextColumnType nextColumnType) const;
	int RandDifficultHeight(const LandType landType) const;

	// Convert Functions
	friend TilePosition ConvertWorldToTilePosition(JEPoint2D position);
	friend JEPoint2D ConvertTileToWorldPosition(TilePosition tilePosition);
	friend JEPoint2D RightConvertTileToWorldPosition(TilePosition tilePosition);
	friend JEPoint2D FindImageGap(JEPoint2D position);

	// Draw Functions
	void DrawVirtualGrid(int imageID, JEPoint2D position);
	void DrawTiles(const JEPoint2D cameraPosition);

	// Print Data
	void PrintCharactersTilePosition(void);
	void PrintCollisionAreaTileTypeInfo(const Contents* collisionArea, unsigned int size);
	void PrintCollisionAreaContentsInfo(const Contents* collisionArea, unsigned int size);

	// Getter
	inline TilePosition Get1stCharacterTilePosition(void);
	inline TilePosition Get2ndCharacterTilePosition(void);

	inline bool GetTileMineral(TilePosition tilePosition);
	inline int GetTileType(TilePosition tilePosition);
	void GetCollisionArea(TilePosition tilePosition, Contents* collisionArea, unsigned int size);

	JEPoint2D GetMolePosition(void);

	// Setter
	inline void SetTileType(TilePosition tilePosition, int tileID);
	inline void SetTileMieral(TilePosition tilePosition, bool mineral);

private:
	typedef std::vector<JE::OBJECTS>::iterator ObjectsItor;
	typedef std::vector<JE::OBJECTS>::const_iterator ObjectsCItor;

	Mole mole;       // Make undergrand
	Bird bird;       // Floting land
	Mineral mineral; // Mineral in tile
	Tree tree;       // Long & short
	Barrel barrel;   // Shop
	Deer deer;       // Deer
	Stone stone;     // Stone

	// The last tile will be that position.
	TilePosition rightLandEnd;
	TilePosition leftLandEnd;
};

TileMapManager& GetTileMapManager(void);

// TestFunctions
void CoordinatesConversionTest(const JEPoint2D playerPosition);
void TileArrangementTest(const int rightLandEndX, const int leftLandEndX);

#endif