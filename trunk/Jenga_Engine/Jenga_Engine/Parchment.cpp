#include "Parchment.h"

#define MAX_TUTO_COUNT 4

namespace
{
	JE::parchment parchmentObj;
	int parchmentID;
	int tutoArrowID;
	int tutoHpId;
	int tutoCapsuleID;
	int tutoShopID;

	const double ANIMATION_TIME = 0.5f;
	static double survieAniTime;
	static double aniRateInc;
	static double currAniRate;

	double entireTime;
	double halfTime;
	static double time;


	static bool questFlag[MAX_TUTO_COUNT];
	static bool stopFlag[MAX_TUTO_COUNT];

	static int count;

	static int texture[MAX_TUTO_COUNT];

	JE::Vector2f tutoSize[MAX_TUTO_COUNT];
}

namespace JE
{
	void loadParchment(void)
	{
		parchmentID = JE::GraphicLoadTexture("Texture//parchment.tga");
		tutoArrowID = JE::GraphicLoadTexture("Texture//tuto_Arrow.tga");
		tutoHpId = JE::GraphicLoadTexture("Texture//tuto_hp.tga");
		tutoCapsuleID = JE::GraphicLoadTexture("Texture//tuto_capsule.tga");
		tutoShopID = JE::GraphicLoadTexture("Texture//tuto_shop.tga");
	}

	void initParchment(void)
	{
		survieAniTime = 0.f;
		aniRateInc = .25f;
		currAniRate = 0;

		entireTime = 5;
		halfTime = entireTime / 2;
		time = 0;

		for (unsigned i = 0; i < MAX_TUTO_COUNT; ++i)
		{
			questFlag[i] = false;
			stopFlag[i] = false;
		}
		texture[0] = tutoArrowID;
		texture[1] = tutoHpId;
		texture[2] = tutoCapsuleID;
		texture[3] = tutoShopID;

		tutoSize[0] = JE::Vector2f(.4f, .4f);
		tutoSize[1] = JE::Vector2f(.22f, .22f);
		tutoSize[2] = JE::Vector2f(.22f, .22f);
		tutoSize[3] = JE::Vector2f(.22f, .22f);

		count = 0;
	}


	void drawParchment(double dt, bool woodDie, bool deerDie, bool shopTuto)
	{
		Vector3f parchPos = parchmentObj.getParchmentPos();
		Vector2f parchSize = parchmentObj.getParchmentSize();
		float parchAmbient = parchmentObj.getParchAmbi();

		if (questFlag[count] == false && stopFlag[count] == false)
		{
			if (parchPos.m_y >= .85f)
			{
				parchPos.m_y -= (float)dt * 0.2f;
				parchmentObj.setParchPos(parchPos);
			}

			if (parchAmbient <= 1.f)
			{
				parchAmbient += (float)dt;
				parchmentObj.setParchAmbi(parchAmbient);
			}
			else if ((g_Input->IsPressed(JE_ARROW_LEFT) ||
				     g_Input->IsPressed(JE_ARROW_RIGHT) ||
				     MyGamepad->GetButtonPressed(XButtons.DPad_Left) ||
				     MyGamepad->GetButtonPressed(XButtons.DPad_Right)
					 ) && stopFlag[0] == false
					 && count == 0)
			{
				questFlag[count] = true;
			}
			else if (woodDie == true && stopFlag[1] == false && count == 1)
			{
				questFlag[count] = true;
			}
			else if (deerDie == true && stopFlag[2] == false && count == 2)
			{
				questFlag[count] = true;
			}
			else if (shopTuto == true && stopFlag[3] == false && count == 3)
			{
				questFlag[count] = true;
			}
		}
		else if (questFlag[count] == true)
		{
			if (time <= entireTime)
			{
				time += dt;
			}
			else
			{
				if (parchPos.m_y <= 1.5f)
				{
					parchPos.m_y += (float)dt * 0.2f;
					parchmentObj.setParchPos(parchPos);
				}

				if (parchAmbient >= 0.f)
				{
					parchAmbient -= (float)dt;
					parchmentObj.setParchAmbi(parchAmbient);
				}
				else
				{
					time = 0;
					stopFlag[count] = true;
					currAniRate = 0;

					if (count < MAX_TUTO_COUNT -1 )
						++count;
				}
			}
		}

		survieAniTime += dt;
		if (survieAniTime > ANIMATION_TIME)
		{
			currAniRate += aniRateInc;
			survieAniTime = 0;

			if (currAniRate >= 0.5f && questFlag[count] == false)
				currAniRate = 0;

			if (questFlag[count] == true && currAniRate > .75f)
				currAniRate = .5f;
		}

		GraphicSetTextureUnit(0);
		GraphicSetTexture(parchmentID);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(1, 0);
		GraphicSetOrthoParticleTransform(parchPos.m_x, parchPos.m_y, parchPos.m_z,
			parchSize.m_x, parchSize.m_y,
			0.f, parchAmbient);

		GraphicSetTextureUnit(0);
		GraphicSetTexture(texture[count]);
		GraphicSetFilpTex(FLIP_IDLE, 1);
		GraphicSetAnimaTex(4, float(currAniRate));
		GraphicSetOrthoParticleTransform(parchPos.m_x, parchPos.m_y, parchPos.m_z,
			tutoSize[count].m_x, tutoSize[count].m_y,
			0.f, parchAmbient);
	}


	void unloadParchment(void)
	{
		JE::GraphicUnloadTexture(parchmentID);
		JE::GraphicUnloadTexture(tutoArrowID);
		JE::GraphicUnloadTexture(tutoHpId);
		JE::GraphicUnloadTexture(tutoCapsuleID);
		JE::GraphicUnloadTexture(tutoShopID);
	}
}