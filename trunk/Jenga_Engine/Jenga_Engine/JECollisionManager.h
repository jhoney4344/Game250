/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.

filename    JECollisionManager.h
author      Nahye Park
email       skqla12@gmail.com
course      GAM250
due date    12/6/2015

Brief Description:
  This file contains the function declaration for JECollisionManager.

*******************************************************************************/

#ifndef JENGA_ENGINE_COLLISIONMANAGER
#define JENGA_ENGINE_COLLISIONMANAGER

#include "JEMath.h"
#include "JETileMap.h"


class JE_COLLISIONMGR
{
public:
	JE_COLLISIONMGR();
	~JE_COLLISIONMGR();

	//Intersection
	bool IntersectionPointCircle(const JEPoint2D& circle, double radius, const JEPoint2D& point);
	bool IntersectionPointLine(const JEPoint2D& point, const JEPoint2D& lineStart, const JEPoint2D& lineEnd);
	bool IntersectionPointRect(const JEPoint2D& point, const JEPoint2D& centerRect, double width, double height);
	bool IntersectionCircleRect(const JEPoint2D& circle, double radius, const JEPoint2D& centerRect, double width, double height);
	bool IntersectionCircleCircle(const JEPoint2D& circle1, double radius1, const JEPoint2D& circle2, double radius2);
	bool IntersectionCircleLine(const JEPoint2D& circle, double radius, const JEPoint2D& lineStart, const JEPoint2D& lineEnd);
	bool IntersectionRectRect(const JEPoint2D& centerRect1, double width1, double height1 , 
							  const JEPoint2D& centerRect2, double width2, double height2);

	//Collision Response
	void CollisionResponsePlayerMovingObject(JEPoint2D& position,JEPoint2D& speed,double& force, const double radius, JEPoint2D& centerRect, double width, double height);

	void CollisionResponsePlayerGround(JEPoint2D& position, const double radius,const JEPoint2D& lineStart, const JEPoint2D& lineEnd);
	void CollisionResponsePlayerEnemy(JEPoint2D& player,const double radiusPlayer, JEPoint2D& enemy, const double radiusEnemy, double distance);
	void CollisionResponsePlayerPlayer(JEPoint2D& player1, const double radius1, JEPoint2D& player2, const double radius2);
	void CollisionResponseWithTile(JEPoint2D& player, const double length, JEPoint2D tileWorldPos, TILE_POSITION tilePos);
	void CollisionAreaPos(JEPoint2D* tile, TILE_POSITION* tilePos, JEPoint2D worldCharacter, int tileSize);

	void KnockBack(JEPoint2D& player, const JEPoint2D normVec, const double distance);

};

extern JE_COLLISIONMGR *g_Collision;

#endif