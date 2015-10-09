/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JETileMapStructs.h
author      Minsu Kang
email       minsu9486@gmail.com
course      GAM250

*******************************************************************************/

#ifndef JENGA_ENGINE_TILEMAPSTRUCTS
#define JENGA_ENGINE_TILEMAPSTRUCTS

#define TILE_SPACE 10
#define INIT_HIGHT 150
#define MIN_NON_DETECT_TERM 30

#define CHARACTER_COLLISION_AREA 7
#define MOLE_SPEED 10
#define MOLE_LIFT_TIME 5

#define MOLE_TERM 200
#define CAMERA_TERM 400
#define MINERAL_TERM 1
#define TREE_TERM 3
#define BARREL_TERM 75
#define DEER_TERM 10
#define STONE_TERM 5

enum NextColumnType
{
	NCY_BEGINE,

	NCT_DIFFERENT_HEIGHT,
	NCT_HILL,
	NCT_VALLEY,

	NCT_END
};

enum LandType
{
	LT_BEGINE = 3,

	LT_HIGHER = 2,
	LT_HIGH = 1,
	LT_NORMAL = 0,
	LT_LOW = -1,
	LT_LOWER = -2,

	LT_END = -3
};

enum TILE_POSITION
{
	TL_LEFTUP,
	TL_UP,
	TL_RIGHTUP,
	TL_LEFT,
	TL_O,
	TL_RIGHT,
	TL_LEFTDOWN,
	TL_DOWN,
	TL_RIGHTDOWN
};

struct ObjectIDs
{
	int shortTree;
	int longTree;
};

struct TileIDs
{
	int emptyID;
	int emptyDirtID;

	// Common Objects
	int dirtID;
	int stoneID;
	int commonEnemyID;
	int unbreakableTileID;

	// Forest
	int trunkID;
	int leafID;
	int forestEnemyID;

	// Snow
	int snowID;
	int iceID;
	int penguinID;

	// Desert
	int sandID;
	int cactusID;
	int desertEnemyID;

	// Sky
	int cloudID;
	int skyEnemyID;

	// In Water
	int waterID;
	int piranhaID;
};

struct TilePosition
{
	TilePosition() : x(0), y(0) {};
	TilePosition(int ix, int iy) : x(ix), y(iy) {};

	int x;
	int y;

	friend	TilePosition operator-(const TilePosition& tilePosition1, const TilePosition& tilePosition2) {
		return TilePosition(tilePosition1.x - tilePosition2.x, tilePosition1.y - tilePosition2.y);
	}
	friend	TilePosition operator+(const TilePosition& tilePosition1, const TilePosition& tilePosition2) {
		return TilePosition(tilePosition1.x + tilePosition2.x, tilePosition1.y + tilePosition2.y);
	}

	bool operator==(const TilePosition &tilePosition) const {
		return x == tilePosition.x && y == tilePosition.y;
	}

	bool operator!=(const TilePosition &tilePosition) const {
		return x != tilePosition.x || y != tilePosition.y;
	}

	bool operator<(const TilePosition &tilePosition) const {
		return x < tilePosition.x || (x == tilePosition.x && y < tilePosition.y);
	}
};

#endif