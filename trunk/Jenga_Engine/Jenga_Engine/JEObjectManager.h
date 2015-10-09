/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEObjectManager.h
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final project milestone
due date    12/6/2015

Brief Description:
  This file is interface file for JEObjectManager.cpp file.

*******************************************************************************/
#ifndef JENGA_ENGINE_OBJECTMANAGER
#define JENGA_ENGINE_OBJECTMANAGER

#include <map>
#include <list>
#include "JESTL.h"//map,list
#include "JEObjectTypes.h" // OBJECTS
#include "JEObjects.h"
#include "JEDayAndNight.h"
#ifdef NEW_CHARACTER_SYSTEM
#include "JECharacterNew.h"
#endif

#define MAXPLAYER 2

namespace JE{
namespace ObjectManager{

typedef std::list<BaseObject*> ObjList;
typedef std::map<OBJECTS,ObjList> ObjMap;

	
typedef std::list<BaseObject*>::iterator ListItor;
typedef std::map<OBJECTS,ObjList>::iterator MapItor;

void Load(void);
void Init(void);
void Update(double, DAY_NIGHT dayNight = DAY, Vector3f cameraPos = Vector3f(0.f, 0.f, 0.f));
void Draw(double);
void Draw();
void ShutDown(void);
void Unload(void);

BaseObject* LastAddObject(void);
ObjMap& GetObjMap(void);
void ClearAllList(void);
void ClearMap(void);
void FindObjectsBy(void);
void AddObject(OBJECTS, BaseObject*);
void AddPlayer(int _ID);
int* GetObjID(void);
int* GetBoneID(OBJECTS type);

//Character& GetPlayerByID(int _ID);

}//Object Manager
}//Jenga Engine
#endif