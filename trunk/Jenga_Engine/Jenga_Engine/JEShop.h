/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEShop.h
author      Jaeheon Lee
            Sunghak Jin
email       jhoney4344@gmail.com
            shjin920@gmail.com
course      GAM250
assignment  Final project submission
due date    6/12/2014

Brief Description:
  Shop data for all state.

*******************************************************************************/
#include "JEInput.h"
#include "JEParts.h"
#include "JEObjectManager.h"
#include "JESound.h"

#include "JEGraphicsNew.h"
#include "JELoadTextureNew.h"
#include "JEIniFileManager.h"

#include "JECharacterManager.h"
#include "JECharacterType.h"
#include "JEParts.h"
#include "JEAniSystem.h"
#include "JEHUD.h"
namespace JE{

void ShopCheck(int ID);
void ShopLoad();
void ShopInit();
bool ShopUpdate(const JEPoint2D& mousePosition, JEPoint2D& padCursorPosition);
void ShopDraw();
void DrawMouse(bool pause, JEPoint2D* mousePosition, JEPoint2D* padCursorPosition, const double dt, const int cursorID);
void ShopShutdown();
void ShopUnload();
void SetShopOn(bool);
bool& GetShopOn();
BODY_PART GetRect(const JEPoint2D& mouse, BODY_PART part);

}