/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.

filename    JECollisionManager.cpp
author      Nahye Park
email       skqla12@gmail.com
course      GAM250
due date    12/6/2015

Brief Description:
  This file contains the function definition for JECollisionManager.

*******************************************************************************/

#include "JECollisionManager.h"
#include "JEMath.h"

JE_COLLISIONMGR *g_Collision = NULL;

//constructor

/*******************************************************************************
   Function: JE_COLLISIONMGR

Description: Constructor.


     Inputs: None.


    Outputs: None.
*******************************************************************************/
JE_COLLISIONMGR::JE_COLLISIONMGR()
{
	g_Collision = this;
}

//Point and Circle Intersection

/*******************************************************************************
   Function: IntersectionPointCircle

Description: Point and Circle Intersection check.
             

     Inputs: circle - center point of circle to check the intersection.
             radius - radius of circle
			 point - point to check the intersection

    Outputs: true - if circle and point are intersected.
	         false - if circle and point are not intersected.
*******************************************************************************/
bool JE_COLLISIONMGR::IntersectionPointCircle(const JEPoint2D& circle, double radius, const JEPoint2D& point)
{
	return (g_Math->DistancePointPoint(circle, point) <= radius);
}

//Point and Line Intersection

/*******************************************************************************
   Function: IntersectionPointLine

Description: Point and Line intersection check.
             

     Inputs: point - point to check the intersection.
             lineStart - line start point.
			 lineEnd - line end point.

    Outputs: true - if point and line are intersected.
	         false - if point and line are not intersected.
*******************************************************************************/
bool JE_COLLISIONMGR::IntersectionPointLine(const JEPoint2D& point, const JEPoint2D& lineStart, const JEPoint2D& lineEnd)
{
	return (g_Math->DistancePointLine(point,lineStart,lineEnd) == 0);
}

//Point and Rectangle Intersection
/*******************************************************************************
   Function: IntersectionPointRect

Description: Point and Rectangle intersection check.
             

     Inputs: point - point to check the intersection.
             centerRect - the center point of rectangle.
			 width - width of rectangle.
			 height - height of rectangle.

    Outputs: true - if point and Rectangle are intersected.
	         false - if point and Rectangle are not intersected.
*******************************************************************************/
bool JE_COLLISIONMGR::IntersectionPointRect(const JEPoint2D& point, const JEPoint2D& centerRect, double width, double height)
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	JEPoint2D topLeft;
	JEPoint2D bottomRight;

	g_Math->PointSet(topLeft, centerRect.x - width/2 , centerRect.y - height/2 );
	g_Math->PointSet(bottomRight, centerRect.x + width/2 , centerRect.y + height/2);

	return (    point.x >= topLeft.x && point.x <= bottomRight.x
		     && point.y >= topLeft.y && point.y <= bottomRight.y);

}

//Circle and Rectangle Intersection
/*******************************************************************************
   Function: IntersectionCircleRect

Description: Circle and Rectangle intersection check.
             

     Inputs: circle - center point of circle to check the intersection.
	         radius - radius of circle.
             centerRect - the center point of rectangle.
			 width - width of rectangle.
			 height - height of rectangle.

    Outputs: true - if Circle and Rectangle are intersected.
	         false - if Circle and Rectangle are not intersected.
*******************************************************************************/
bool JE_COLLISIONMGR::IntersectionCircleRect(const JEPoint2D& circle, double radius, const JEPoint2D& centerRect, double width, double height)
{
	JEPoint2D topLeft;
	JEPoint2D topRight;
	JEPoint2D bottomLeft;
	JEPoint2D bottomRight;

	g_Math->PointSet(topLeft, centerRect.x - width/2 , centerRect.y - height/2 );
	g_Math->PointSet(bottomRight, centerRect.x + width/2 , centerRect.y + height/2);
	g_Math->PointSet(topRight, centerRect.x + width/2 , centerRect.y - height/2);
	g_Math->PointSet(bottomLeft, centerRect.x - width/2 , centerRect.y + height/2);


	return (IntersectionCircleLine(circle, radius, topLeft, topRight)      ||
	        IntersectionCircleLine(circle, radius, bottomLeft, topLeft)    ||
	        IntersectionCircleLine(circle, radius, topRight, bottomRight)  ||
	        IntersectionCircleLine(circle, radius, bottomRight, bottomLeft) );

}

//Circle and Circle Intersection
/*******************************************************************************
   Function: IntersectionCircleCircle

Description: Circle and Circle intersection check.
             

     Inputs: circle1 - center point of circle1.
	         radius1 - radius of circle1.
			 circle2 - center point of circle2.
	         radius2 - radius of circle2.

    Outputs: true - if Circle and Circle are intersected.
	         false - if Circle and Circle are not intersected.
*******************************************************************************/
bool JE_COLLISIONMGR::IntersectionCircleCircle(const JEPoint2D& circle1, const double radius1, 
												const JEPoint2D& circle2, const double radius2)
{
	return (g_Math->DistancePointPoint(circle1,circle2) <= radius1 + radius2);
}

//Circle and Line Intersection
/*******************************************************************************
   Function: IntersectionCircleLine

Description: Circle and Line intersection check.
             

     Inputs: circle - center point of circle.
	         radius - radius of circle.
			 startLine - line start point.
	         lineEnd - line end point.

    Outputs: true - if Circle and Line are intersected.
	         false - if Circle and Line are not intersected.
*******************************************************************************/
bool JE_COLLISIONMGR::IntersectionCircleLine(const JEPoint2D& circle, const double radius, 
											const JEPoint2D& startLine, const JEPoint2D& lineEnd)
{
	return (g_Math->DistancePointLine(circle,startLine,lineEnd) <= radius);
}

//Rectangle and Rectangle Intersection
/*******************************************************************************
   Function: IntersectionRectRect

Description: Rectangle and Rectangle intersection check.
             

     Inputs: centerRect1 - the center point of rectangle1.
			 width1 - width of rectangle1.
			 height1 - height of rectangle1.
			 centerRect2 - the center point of rectangle2.
			 width2 - width of rectangle2.
			 height2 - height of rectangle2.

    Outputs: true - if Rectangle and Rectangle are intersected.
	         false - if Rectangle and Rectangle are not intersected.
*******************************************************************************/
bool JE_COLLISIONMGR::IntersectionRectRect(const JEPoint2D& centerRect1, double width1, double height1 , 
											const JEPoint2D& centerRect2, double width2, double height2)
{

	JEPoint2D topLeft1;
	JEPoint2D bottomRight1;

	g_Math->PointSet(topLeft1, centerRect1.x - width1/2 , centerRect1.y - height1/2 );
	g_Math->PointSet(bottomRight1, centerRect1.x + width1/2 , centerRect1.y + height1/2);

	JEPoint2D topLeft2;
	JEPoint2D bottomRight2;

	g_Math->PointSet(topLeft2, centerRect2.x - width2/2 , centerRect2.y - height2/2 );
	g_Math->PointSet(bottomRight2, centerRect2.x + width2/2 , centerRect2.y + height2/2);


	return !( topLeft2.x     >= bottomRight1.x ||
		      bottomRight2.x <= topLeft1.x     ||
		      topLeft2.y >= bottomRight1.y ||
		      bottomRight2.y <= topLeft1.y);

}

/*******************************************************************************
   Function: CollisionResponsePlayerEnemy

Description: Collision response when player and enemy are collided.
             

     Inputs: player - position of player.
	         radiusPlayer - collision radius of player.
             enemy - position of enemy.
			 radiusEnemy - collision radius of enemy

    Outputs: None.
*******************************************************************************/
void JE_COLLISIONMGR::CollisionResponsePlayerEnemy(JEPoint2D& player,const double radiusPlayer, 
	JEPoint2D& enemy, const double radiusEnemy, double distance)
{
	
	//double overlapDistance = (radiusPlayer+radiusEnemy) - g_Math->DistancePointPoint(player,enemy);
	radiusPlayer;
	radiusEnemy;

	JEPoint2D vec = player-enemy;
	JEPoint2D norm = g_Math->Normalize(vec);

	//if(player.x < enemy.x)
	//	player.x -= 8;
	//else if(player.x > enemy.x)
	//	player.x += 8;

	//if(player.y < enemy.y)
	//	player.y -= 8;
	//else if(player.y > enemy.y)
	//	player.y += 8;
	player.x += norm.x*distance;
	player.y += norm.y*distance;


}

/*******************************************************************************
   Function: CollisionResponsePlayerMovingObject

Description: Collision response when player push the moving object.
             

     Inputs: position - position of player.
	         speed - speed of player.
             force - force of player to push.
			 radius - radius of player.
			 centerRect - center position of moving object.
			 width - width of object.
			 height - height of object.

    Outputs: None.
*******************************************************************************/
void JE_COLLISIONMGR::CollisionResponsePlayerMovingObject(JEPoint2D& position,JEPoint2D& speed, double& force, const double radius, JEPoint2D& centerRect, double width, double height)
{
	JEPoint2D topLeft;
	JEPoint2D topRight;
	JEPoint2D bottomLeft;
	JEPoint2D bottomRight;

	g_Math->PointSet(topLeft, centerRect.x - width/2 , centerRect.y - height/2 );
	g_Math->PointSet(bottomRight, centerRect.x + width/2 , centerRect.y + height/2);
	g_Math->PointSet(topRight, centerRect.x + width/2 , centerRect.y - height/2);
	g_Math->PointSet(bottomLeft, centerRect.x - width/2 , centerRect.y + height/2);

	std::pair<JEPoint2D,JEPoint2D> collision[4];

	collision[0] = std::make_pair(bottomLeft,topLeft);
	collision[1] = std::make_pair(topLeft,topRight);
	collision[2] = std::make_pair(topRight,bottomRight);
	collision[3] = std::make_pair(bottomRight,bottomLeft);

	int i;
	for(i = 0 ; i < 4 ; ++i)
		if(IntersectionCircleLine(position,radius,collision[i].first,collision[i].second))
			break;

	JEPoint2D collided;
	switch(i)
	{
	case 0:
		collided.x = topLeft.x;
		collided.y = position.y;
		break;
	case 1:
		collided.x = position.x;
		collided.y = topLeft.y;
		break;
	case 2:
		collided.x = bottomRight.x;
		collided.y = position.y;
		break;
	case 3:
		collided.x = position.x;
		collided.y = bottomRight.y;
		break;
	default:
		break;
	}

	force += 0.1;
	if(force <= 30)
	{
		position.x = collided.x;
		position.y = collided.y;
	}
	else
	{
		position.x += speed.x*(2/3);
		position.y += speed.y*(2/3);

		centerRect.x += speed.x*(2/3);
		centerRect.y += speed.y*(2/3);
	}

	
}

/*******************************************************************************
   Function: CollisionResponsePlayerGround

Description: Collision response when player is collided with the ground.
             

     Inputs: position - position of player.
	         radius - collision radius of player.
             lineStart - line start of ground that player is collided.
			 lineEnd - line end of ground that player is collided.

    Outputs: None.
*******************************************************************************/
void JE_COLLISIONMGR::CollisionResponsePlayerGround(JEPoint2D& position, const double radius, const JEPoint2D& lineStart, const JEPoint2D& lineEnd)
{
	JEPoint2D vec = lineEnd - lineStart;
	JEPoint2D normalVec = g_Math->NormalVec(vec);

	double distance = radius - g_Math->DistancePointLine(position, lineStart, lineEnd);


	if(distance < 0)
		return;


	if(lineStart.x == lineEnd.x) //vertical
	{
		if(normalVec.x < 0)
			position.x = lineStart.x - radius;
		else //if(normalVec.x > 0)
			position.x = lineStart.x + radius;
	}
	else if(lineStart.y == lineEnd.y) //horizontal
	{
		//if(lineStart.x == lineEnd.x)
		//{
		//	//if(lineStart.x > position.x)
		//	//	position.x = lineStart.x - radius;
		//	//else// if(lineStart.x < position.x)
		//	//	position.x = lineStart.x + radius; 
		//}
		//else
		//{

			if(normalVec.y < 0)
				position.y = lineStart.y - radius;
			else //if(normalVec.y > 0)
				position.y = lineStart.y + radius;

//		}
	}

}

/*******************************************************************************
Function: CollisionResponseWithTile

Description: Collision response when player is collided with the ground.

Inputs: player - position of player.
length - collision radius of player.
tileWorldPos- tile position in world.
tilePos - tile position for player.

Outputs: None.
*******************************************************************************/
void JE_COLLISIONMGR::CollisionResponseWithTile(JEPoint2D& player, const double length, 
											JEPoint2D tileWorldPos, TILE_POSITION tilePos)
{
	if (tilePos == TL_DOWN)
	{

		player.y += length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2));

	}

	//Up
	if (tilePos == TL_UP)
	{

		player.y -= length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));

	}

	//Right
	if (tilePos == TL_RIGHT)
	{
		player.x -= length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));

	}


	//Left
	if (tilePos == TL_LEFT)
	{
		player.x += length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));

	}


	if (tilePos == TL_LEFTDOWN)
	{
		//left
		player.x += length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));
		//down
		player.y += length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2));
	}

	if (tilePos == TL_RIGHTDOWN)
	{
		//right
		player.x -= length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));
		//down
		player.y += length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2));
	}

	if (tilePos == TL_LEFTUP)
	{
		//left
		player.x += length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));
		//up
		player.y -= length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));
	}

	if (tilePos == TL_RIGHTUP)
	{
		//right
		player.x -= length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y + TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));
		//up
		player.y -= length - g_Math->DistancePointLine(player,
			JEPoint2D(tileWorldPos.x - TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2),
			JEPoint2D(tileWorldPos.x + TILE_SPACE / 2, tileWorldPos.y - TILE_SPACE / 2));
	}
}

/*******************************************************************************
Function: CollisionAreaPos

Description: Tile position in tile for character world position.


Inputs: tile - tile position array that will be modified.
tilePos - tile type for player that will be modified.
worldCharacter - character position in world.
tileSize - The size of tile.

Outputs: None.
*******************************************************************************/
void JE_COLLISIONMGR::CollisionAreaPos(JEPoint2D* tile, TILE_POSITION* tilePos, JEPoint2D worldCharacter, int tileSize)
{
	int center = tileSize*(tileSize / 2) + (tileSize / 2);
	JEPoint2D leftTop;
	leftTop.x = worldCharacter.x - (tileSize / 2)*TILE_SPACE;
	leftTop.y = worldCharacter.y + (tileSize / 2)*TILE_SPACE;


	for (int i = 0; i < tileSize; ++i)
	{
		for (int j = 0; j < tileSize; ++j)
		{
			int position = i*tileSize + j;
			tile[position].x = leftTop.x + TILE_SPACE*j;
			tile[position].y = leftTop.y - TILE_SPACE*i;

			if (position == center - 1)
				tilePos[position] = TL_LEFT;
			if (position == center + 1)
				tilePos[position] = TL_RIGHT;
			if (position == center - tileSize - 1)
				tilePos[position] = TL_LEFTUP;
			if (position == center - tileSize)
				tilePos[position] = TL_UP;
			if (position == center - tileSize + 1)
				tilePos[position] = TL_RIGHTUP;
			if (position == center + tileSize - 1)
				tilePos[position] = TL_LEFTDOWN;
			if (position == center + tileSize)
				tilePos[position] = TL_DOWN;
			if (position == center + tileSize + 1)
				tilePos[position] = TL_RIGHTDOWN;

		}

	}
}

/*******************************************************************************
Function: KnockBack

Description: knock back for character or enemy.


Inputs: player - position of player.
normVec - direction for knock back.
distance - knockback distance.

Outputs: None.
*******************************************************************************/
void JE_COLLISIONMGR::KnockBack(JEPoint2D& player, const JEPoint2D normVec, const double distance)
{
	player.x += normVec.x*distance;
	player.y += normVec.y*distance;
}
