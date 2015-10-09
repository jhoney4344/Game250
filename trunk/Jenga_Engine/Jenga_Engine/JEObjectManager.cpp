/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEObjectManager.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project submission
due date    12/6/2015

Brief Description:
  Managing objects by list and map. Update their state and draw.

*******************************************************************************/
#include "JEObjectManager.h"
#include "JESound.h"
#include "JEGraphicsNew.h"
#include "JETileMap.h"
#include "JECharacter.h"
#include "JECharacterManager.h"
#include "JELoadTextureNew.h"
#include "JECollisionManager.h"
#include "JEOldParticle.h"
#include "JEGameData.h"
#include "JEParts.h"// PARTS NAME
#include <vector>

extern JE::GameData gd;

static const int MAP_BOTTOM = -40;
static const int MAX_OBJ_NUM = 20;

namespace{

	struct OMData
	{
		int numOfObj;
		int numOfChar;
	};

	JE::ObjectManager::ObjMap objMap;
	JE::ObjectManager::ObjList* pCurrList;
	particleSystem* particleSys = &getParticleSystem();
	OMData omData;
	int objID[MAX_OBJ_NUM];
	int boneID[MAX_OBJ_NUM][20];

}// empty namespace

namespace JE{
namespace ObjectManager{
	/*******************************************************************************
	Function: Load

	Description: Load texture.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void Load(void)
	{
		ResourceID& resourceID = ResourceSystem::GetResourceID();
		objID[OBJ_MEAT] = resourceID.meat;
		objID[OBJ_BREAD] = resourceID.bread;
		objID[OBJ_MINERAL] = ResourceSystem::GetCrystalID();
		objID[OBJ_SHOP] = GraphicLoadTexture("Texture//Capsule.tga");
		objID[OBJ_BUG] = GraphicLoadTexture("Texture//bug.tga");
		objID[OBJ_SHORT_TREE] = GraphicLoadTexture("Texture//Object_ShortTree.tga");
		objID[OBJ_LONG_TREE] = GraphicLoadTexture("Texture//Object_LongTree.tga");
		objID[OBJ_TOMB] = GraphicLoadTexture("Texture//Object_Tomb1.tga");
		objID[OBJ_EMPTY] = GraphicLoadTexture("Texture//emptypart.tga");

		

		boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_HEAD] = GraphicLoadTexture("Texture//Zombie_Head.tga");
		boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_BODY] = GraphicLoadTexture("Texture//Zombie_Body.tga");
		boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_LEFTARM1] = GraphicLoadTexture("Texture//Zombie_LeftArm.tga");
		boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_LEFTARM2] = GraphicLoadTexture("Texture//Zombie_LeftHand.tga");
		boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_RIGHTARM1] = GraphicLoadTexture("Texture//Zombie_RightArm.tga");
		boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_RIGHTARM2] = GraphicLoadTexture("Texture//Zombie_RightHand.tga");
		boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_RIGHTLEG1] = boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_LEFTLEG1] 
			= GraphicLoadTexture("Texture//Zombie_RightLeg.tga");
		boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_LEFTLEG2] = boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_RIGHTLEG2] 
			= GraphicLoadTexture("Texture//Zombie_RightFoot.tga");
		boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_HEAD] = GraphicLoadTexture("Texture//Deer_Head.tga");
		boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_BODY] = GraphicLoadTexture("Texture//Deer_Body.tga");

		boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_LEFTARM1] = boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_LEFTLEG1] 
			= boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_RIGHTARM1] = boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_RIGHTLEG1] 
			= GraphicLoadTexture("Texture//Deer_LegA.tga");

		boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_LEFTARM2] = boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_LEFTLEG2]
			= boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_RIGHTARM2] = boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_RIGHTLEG2] 
			= GraphicLoadTexture("Texture//Deer_LegB.tga");

		boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_TAIL1] = boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_TAIL2] 
			= boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_TAIL3] = GraphicLoadTexture("Texture//Deer_Tail.tga");
	}
	/*******************************************************************************
	Function: Unload

	Description: Unload texture.

	Inputs: None.

	Outputs: None.
	*******************************************************************************/
	void Unload(void)
	{
		GraphicUnloadTexture(objID[OBJ_MEAT]);
		GraphicUnloadTexture(objID[OBJ_BREAD]);
		GraphicUnloadTexture(objID[OBJ_MINERAL]);
		GraphicUnloadTexture(objID[OBJ_SHOP]);
		GraphicUnloadTexture(objID[OBJ_BUG]);
		GraphicUnloadTexture(objID[OBJ_SHORT_TREE]);
		GraphicUnloadTexture(objID[OBJ_LONG_TREE]);
		GraphicUnloadTexture(objID[OBJ_TOMB]);


		GraphicUnloadTexture(boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_HEAD]);
		GraphicUnloadTexture(boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_BODY]);
		GraphicUnloadTexture(boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_LEFTARM1]);
		GraphicUnloadTexture(boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_LEFTARM2]);
		GraphicUnloadTexture(boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_RIGHTARM1]);
		GraphicUnloadTexture(boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_RIGHTARM2]);
		GraphicUnloadTexture(boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_LEFTLEG1]);
		GraphicUnloadTexture(boneID[OBJ_ZOMBIE - TEXT_OBJ_INDEX][PN_LEFTLEG2]);

		GraphicUnloadTexture(boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_HEAD]);
		GraphicUnloadTexture(boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_BODY]);
		GraphicUnloadTexture(boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_LEFTARM2]);
		GraphicUnloadTexture(boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_LEFTARM1]);
		GraphicUnloadTexture(boneID[OBJ_DEER - TEXT_OBJ_INDEX][PN_TAIL1]);

	}
	/*******************************************************************************
	Function: GetObjID

	Description: Return object ID.

	Inputs: None.

	Outputs: ID.
	*******************************************************************************/
	int* GetObjID(void)
	{
		return objID;
	}
	/*******************************************************************************
	Function: GetBoneID

	Description: Return object bone ID.

	Inputs: type - type of objects.

	Outputs: ID.
	*******************************************************************************/
	int* GetBoneID(OBJECTS type)
	{
		return boneID[type - TEXT_OBJ_INDEX];	
	}
	
/*******************************************************************************
   Function: Init

Description: Initialize the object manager data.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void Init(void)
{
	omData.numOfChar = 0;
	omData.numOfObj  = 0;
}
/*******************************************************************************
   Function: Update

Description: Check collision and react from that and draw.

     Inputs: dt - Get from the state manager.

    Outputs: None.
*******************************************************************************/
void Update(double dt, DAY_NIGHT dayNight, Vector3f cameraPos)
{
	JE_UNUSED_PARAM(dt);

	TileMapManager* tileMapManager = &GetTileMapManager();

	//collision check
	for (int id = 0; id < 2; id++)
	{
		Character *pPlayer = &CharacterManager::GetPlayerByID((CHARACTER_ID)id);
		//Check player exist
		//if(!pPlayer->playerOn)
		//	continue;

		// Get character properties
		JEPoint2D charPos = pPlayer->GetProperty().position;
		//double charR = pPlayer->GetProperty().collisionDistance;

		// loop for object map
		MapItor mapItor = objMap.begin();
		
			while(mapItor != objMap.end())
			{
				
				ListItor listItor = mapItor->second.begin();
				while(listItor != mapItor->second.end())
				{
					bool itorAdd = false;

					if ((*listItor)->applyGravity)
					{
						(*listItor)->time += dt;
						
						(*listItor)->CheckCollision();

						if ((*listItor)->isOnGround)
							(*listItor)->gravity = 0;
						else
							(*listItor)->gravity = GRAVITY;

						(*listItor)->position.y -= ((*listItor)->gravity*(*listItor)->time*(*listItor)->time) / 2;
					}

					if ((*listItor)->type == OBJ_TOMB)
					{
						bool& tempBool = dynamic_cast<Tomb*>(*listItor)->active;
						JEPoint2D pos = (*listItor)->position;


						if (dayNight == NIGHT && tempBool)
						{
							ObjectManager::AddObject(OBJ_ZOMBIE, new Zombie(pos));
							particleSys->addParticleEmit(SMOKE, JE::Vector3f(float(pos.x), float(pos.y), 0), 30);
							tempBool = false;
						}
						else if (dayNight == DAY && !tempBool)
							tempBool = true;

						
						if (dayNight == DAY)
						{
							int maxWidth = 330;
							int maxHeight = 220;
							
							if (pos.x <= cameraPos.m_x + maxWidth && pos.x >= cameraPos.m_x - maxWidth
								&& pos.y <= cameraPos.m_y + maxHeight && pos.y >= cameraPos.m_y - maxHeight)
							{
								double gapX = pos.x - charPos.x;
								double gapY = pos.y - charPos.y;

								const double distance = gapX * gapX + gapY * gapY;


								if (distance <= 10000 && (dynamic_cast<Tomb*>(*listItor)->m_flyEmit == 0))
								{
									JE::Vector3f particlePosition = JE::Vector3f(float(dynamic_cast<Tomb*>(*listItor)->position.x),
																				 float(dynamic_cast<Tomb*>(*listItor)->position.y),
																				 0.f);
									dynamic_cast<Tomb*>(*listItor)->m_flyEmit = particleSys->addParticleEmit(PFLY, particlePosition, 100);
								}
								
							}

							if (dynamic_cast<Tomb*>(*listItor)->m_flyEmit != 0)
							{
								dynamic_cast<Tomb*>(*listItor)->m_flyEmit->changeEmitterPos(JE::Vector3f(float(dynamic_cast<Tomb*>(*listItor)->position.x),
																										 float(dynamic_cast<Tomb*>(*listItor)->position.y),
																										 0.f));
								if (dynamic_cast<Tomb*>(*listItor)->m_flyEmit->getNumberOfParticle() <= 0)
								{
									dynamic_cast<Tomb*>(*listItor)->m_flyEmit = 0;
								}
							}
						}
					} 
					else if ((*listItor)->type == OBJ_ZOMBIE) //OBJ_ENEMY
					{
						Zombie* obj = dynamic_cast<Zombie*>((*listItor));

						//dynamic_cast<Zombie*>((*listItor))->checkTime+=dt;
					
						if (obj->jump)
							obj->Jump(dt);

						if (obj->isOnGround)
							obj->jump = false;

						if (obj->knockback_distance <= 0)
							obj->knockBack = false;

						if (obj->knockBack)
						{
							obj->knockback_distance -= 0.2;
							g_Collision->KnockBack((*listItor)->position, obj->knockVec, obj->knockback_distance);
						}
						else
							obj->DefaultMoving((*listItor), dt);
						

						if (dayNight == DAY)
						{
							JEPoint2D pos = (*listItor)->position;
							particleSys->addParticleEmit(SMOKE, JE::Vector3f(float(pos.x), float(pos.y), 0), 10);
							(*listItor)->remove = true;
						}
					} 
					else if ((*listItor)->type == OBJ_DEER) //OBJ_ENEMY
					{
						Deer* obj = dynamic_cast<Deer*>((*listItor));

						//collision position
						if (obj->flip) //left
							obj->collisionPos = JEPoint2D(obj->position.x - obj->collisionDistance / 2, obj->position.y);
						else //right
							obj->collisionPos = JEPoint2D(obj->position.x + obj->collisionDistance / 2, obj->position.y);


						if (obj->jump)
							obj->Jump(dt);

						if (obj->isOnGround)
							obj->jump = false;
						
						if (obj->knockback_distance <= 0)
							obj->knockBack = false;

						if (obj->knockBack)
						{
							obj->knockback_distance -= 0.2;
							g_Collision->KnockBack((*listItor)->position, obj->knockVec, obj->knockback_distance);
						}
						else
							obj->DefaultMoving((*listItor), dt);

						if (dayNight == NIGHT)
							(*listItor)->remove = true;
					} if ((*listItor)->type == OBJ_MEAT || (*listItor)->type == OBJ_BREAD || (*listItor)->type == OBJ_MINERAL
						|| (*listItor)->type == OBJ_BUG)
					{
						dynamic_cast<Eatable*>(*listItor)->protectTime -= dt;


						int p_id = dynamic_cast<Eatable*>(*listItor)->whoEat;


						//if (dynamic_cast<Eatable*>(*listItor)->protectTime < 0)
							
						if (dynamic_cast<Eatable*>(*listItor)->eaten)
						{
							Vector3f camera = GraphicGetCameraPos();
							
							JEPoint2D barHUD[2] = { JEPoint2D(camera.m_x - 30, camera.m_y + 50)
							                         , JEPoint2D(camera.m_x + 30, camera.m_y + 50) };
							
							JEVector2D vec = JEVector2D(barHUD[p_id].x - (*listItor)->position.x, barHUD[p_id].y - (*listItor)->position.y);
							
							int whoEat = dynamic_cast<Eatable*>(*listItor)->whoEat;
							JEVector2D follow;
							if (id == whoEat)
							{
								bool *playerTouch = &(dynamic_cast<Eatable*>(*listItor)->playerTouch);
								
								follow.x = pPlayer->GetProperty().position.x - (*listItor)->position.x;
								follow.y = pPlayer->GetProperty().position.y - (*listItor)->position.y;

								(*listItor)->position.x += follow.x*dt * 10;
								(*listItor)->position.y += follow.y*dt * 10;
								(*listItor)->scale.x -= dt * 15;
								(*listItor)->scale.y -= dt * 15;


								if (*playerTouch == false)
								{
									//if (follow.x*follow.x + follow.y*follow.y < 5)
									{
										*playerTouch = true;
										SOUNDMANAGER()->Play("acq");
									}
								}
							}													
						}
						if ((*listItor)->scale.y + (*listItor)->scale.x < 1)
							(*listItor)->remove = true;
					}

					// Update the tile's contents
					TilePosition currTilePosition = ConvertWorldToTilePosition((*listItor)->position);
					if ((*listItor)->prevTilePosition != currTilePosition)
					{
						std::vector<ObjectData>* objectData = &tileMapManager->tiles.find((*listItor)->prevTilePosition)->second.objectData;
						for (std::vector<ObjectData>::iterator it = objectData->begin(); it != objectData->end();)
						{
							if (it->object == (*listItor))
							{
								tileMapManager->tiles.find(currTilePosition)->second.objectData.push_back(ObjectData(it->objectType, it->object));
								it = objectData->erase(it);
							}
							else
								++it;
						}

						// update the preTilePosition
						(*listItor)->prevTilePosition = currTilePosition;
					}

					
						if ((*listItor)->remove == true)
						{
							// Erase the object from the tile's data
							std::vector<ObjectData>* objectData = &tileMapManager->tiles.find((*listItor)->prevTilePosition)->second.objectData;
							for (std::vector<ObjectData>::iterator it = objectData->begin(); it != objectData->end();)
							{
								if (it->object == (*listItor))
									it = objectData->erase(it);
								else
									++it;
							}
							//BoneFreeTree(player_bone[i]);
							
							delete (*listItor);
							listItor = mapItor->second.erase(listItor);
							itorAdd = true;
						}
					//}
					// check end point because nested loop remove node.
					if(listItor == mapItor->second.end())
						break;
					if (itorAdd == false)
						listItor++;
				}//while(listItor)
				mapItor++;
			}//while(mapItor)
		//}

	}//for


}
/*******************************************************************************
Function: Draw

Description: Draw.

Inputs: dt - for animation.

Outputs: None.
*******************************************************************************/
void Draw(double dt)
{

	//Object gravity update
	ObjectManager::ObjMap::iterator mapIt = ObjectManager::GetObjMap().begin();
	while (mapIt != ObjectManager::GetObjMap().end())
	{
		ObjectManager::ObjList::iterator listIt = mapIt->second.begin();
		while (listIt != mapIt->second.end())
		{
			//JEGraphicsDraw(&transform);
			if ((*listIt)->drawBase ==DRAW_BONE)
			{
				// this part should be change when release mode or make Macro
				BoneBase *pBone = dynamic_cast<BoneBase*>(*listIt);
				if (pBone)
				{
					//BoneDraw(player_bone[id], properties[id]->position.x,
					//	properties[id]->position.y + CHARACTER_Y_ADJUST, 0);
					BoneDraw(pBone->bone,
						pBone->position.x + pBone->imageGap.x,
						pBone->position.y + pBone->imageGap.y,
						0);
				}
			}
			else if ((*listIt)->drawBase == DRAW_TEXT) // texture base object
			{
				TextBase *pTextBase = dynamic_cast<TextBase*>(*listIt);
		
				GraphicSetTexture(pTextBase->textureID);

				pTextBase->aniData.aniTimeStack += (float)dt;
				if (pTextBase->aniData.aniTimeStack >= pTextBase->aniData.aniTime)
				{
					pTextBase->aniData.aniRateOfChange += pTextBase->aniData.aniSecPerFrame;
					if (pTextBase->aniData.aniRateOfChange >= 1)
						pTextBase->aniData.aniRateOfChange = 0;
					pTextBase->aniData.aniTimeStack = 0;
				}
				GraphicSetFilpTex(FLIP_IDLE, pTextBase->aniData.animationNum);
				GraphicSetAnimaTex(pTextBase->aniData.animationNum, pTextBase->aniData.aniRateOfChange);
				GraphicSetPersTransform(pTextBase->position.x + pTextBase->imageGap.x, pTextBase->position.y + pTextBase->imageGap.y, 0,//x,y coordinate z order
					pTextBase->scale.x, pTextBase->scale.y, //scale x,y
					0);//rotation
			}
			listIt++;
		}
		mapIt++;
	}//while
}
/*******************************************************************************
   Function: ShutDown

Description: Release the dynamic memory and resources.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void ShutDown(void)
{
	ClearMap();
}
/*******************************************************************************
   Function: ClearAllList

Description: clear the list.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void ClearAllList(void)
{
	MapItor mapItor = objMap.begin();
	for(;mapItor != objMap.end(); mapItor++)
	{
		mapItor->second.clear();
	}
}
/*******************************************************************************
   Function: ClearMap

Description: Release the dynamic memory in the map.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void ClearMap(void)
{
	ClearAllList();
	objMap.clear();
	omData.numOfObj = 0;
}
/*******************************************************************************
   Function: AddObject

Description: Add the object by give some data.

     Inputs: type - The object type that will be added.
	         pNewObj - the new object pointer will be added.
	        

    Outputs: None.
*******************************************************************************/
void AddObject(OBJECTS type, BaseObject* pNewObj)
{
	// Find the objects list 
	MapItor mapItor = objMap.find(type);


	//pNewObj->bone = BoneInit(pNewObj->bone, "IniFile//Player.ini");
	//BoneSetTextureID(pNewObj->bone, BODY_NAME::word[PN_HEAD], 0);

	// if there is no object like this key type.
	if( objMap.end() == mapItor )
	{
		TileMapManager* tileMapManager = &GetTileMapManager();

		pNewObj->prevTilePosition = ConvertWorldToTilePosition(pNewObj->position);

		// Make new list.
		pCurrList = new ObjList;
		
		// Add new object address to the new list.
		pCurrList->push_back(pNewObj);

		// Add new key value for map.
		objMap.insert(std::make_pair(type,*pCurrList));

		// Insert the object data to the exact tile data
		tileMapManager->tiles.find(ConvertWorldToTilePosition(pNewObj->position))->second.objectData.push_back(ObjectData(type, pNewObj));
	}
	else if( objMap.end() != objMap.find(type) )
	{
		TileMapManager* tileMapManager = &GetTileMapManager();

		pNewObj->prevTilePosition = ConvertWorldToTilePosition(pNewObj->position);

		// Set pointer for useing list
		pCurrList = &mapItor->second;
		// Add new object address to the current list.
		pCurrList->push_back(pNewObj);

		// Insert the object data to the exact tile data
		tileMapManager->tiles.find(ConvertWorldToTilePosition(pNewObj->position))->second.objectData.push_back(ObjectData(type, pNewObj));
	}
	else
	{
		return;
	}
	omData.numOfObj++;
}
/*******************************************************************************
   Function: GetObjMap

Description: Get the objects map.

     Inputs: None.

    Outputs: the reference of ObjMap class.
*******************************************************************************/
ObjMap& GetObjMap(void)
{
	return objMap;
}



}//Object Manager
}//Jenga Engine