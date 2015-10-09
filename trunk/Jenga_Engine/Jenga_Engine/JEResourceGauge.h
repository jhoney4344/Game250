/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEResourceGauge.h
author      Minsu Kang
email       minsu9486@gmail.com
course      GAM250

*******************************************************************************/

#ifndef JENGA_RESOURCE_GAUGE_H
#define JENGA_RESOURCE_GAUGE_H

#include "JEDayAndNight.h"
#include "JEResource.h"
#include "JEMath.h"

const double GAUGE_SIZE = 0.025;
const double GAUGE_PLAYERS_SPACE = 1.325;
const double GAUGE_Y_SPACE = 0.05;
const JEPoint2D GAUGE_POSITION(-0.9f, 0.9f);

const double CONDITION_SIZE_X = 0.45;
const double CONDITION_SIZE_Y = CONDITION_SIZE_X / 2.f;
const JEPoint2D CONDITION_POSITION(GAUGE_POSITION.x + 0.25, GAUGE_POSITION.y - 0.05);

const double MAX_LENGTH = 0.5;

struct ResourseGaugeTexture
{
	void TextureLoad(void);
	void TextureUnload(void);
	void DrawResourceGauge(const JE::DAY_NIGHT dayNight);

private:
	int deadBlackID;
	int deadRedID;

	int breadID;
	int breadBackID;

	int mineralID;
	int mineralBackID;

	int meatID;
	int meatBackID;
};

#endif