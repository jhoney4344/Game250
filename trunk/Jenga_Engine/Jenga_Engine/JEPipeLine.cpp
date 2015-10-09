/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEPipeLine.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file makes pipe line for various space

*******************************************************************************/

#include "JEPipeLine.h"

namespace JE
{
/*******************************************************************************
   Function: GetWorldTrans

Description: This function gets transformation matrix(world trans)

     Inputs: None.

    Outputs: return transformation matrix(world trans).
*******************************************************************************/
	const Matrix4f& Pipeline::GetWorldTrans()
	{
		Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

		ScaleTrans.InitScaleTransform(m_scale.m_x, m_scale.m_y, m_scale.m_z);
		RotateTrans.InitRotateTransform(m_rotate.m_x, m_rotate.m_y, m_rotate.m_z);
		TranslationTrans.InitTranslationTransform(m_worldPos.m_x, m_worldPos.m_y, m_worldPos.m_z);

		m_worldTransformation = TranslationTrans * RotateTrans * ScaleTrans;
		return m_worldTransformation;
	}
/*******************************************************************************
   Function: GetViewPerspectTrans

Description: This function gets transformation matrix(view perspective)

     Inputs: None.

    Outputs: return transformation matrix(view perspective).
*******************************************************************************/
	const Matrix4f& Pipeline::GetViewPerspectTrans()
	{
		Matrix4f CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

		CameraTranslationTrans.InitTranslationTransform(-m_camera.pipePos.m_x, -m_camera.pipePos.m_y, -m_camera.pipePos.m_z);
		CameraRotateTrans.InitCameraTransform(m_camera.pipeTarget, m_camera.pipeUp);
		PersProjTrans.InitPerspectiveProj(m_persProjInfo.m_FOV, m_persProjInfo.m_width, m_persProjInfo.m_height, m_persProjInfo.m_zNear, m_persProjInfo.m_zFar);
    
		m_viewPerspectTrans = PersProjTrans * CameraRotateTrans * CameraTranslationTrans;
		return m_viewPerspectTrans;
	}
/*******************************************************************************
   Function: GetWorldViewPerspectTrans

Description: This function gets transformation matrix(world view perspective)

     Inputs: None.

    Outputs: return transformation matrix(world view perspective).
*******************************************************************************/
	const Matrix4f& Pipeline::GetWorldViewPerspectTrans()
	{
		GetWorldTrans();
		GetViewPerspectTrans();

		m_worldPerspectTrans = m_viewPerspectTrans * m_worldTransformation;
		return m_worldPerspectTrans;
	}

	const Matrix4f& Pipeline::GetWorldViewOthogonalTrans()
	{
		GetWorldTrans();
		GetViewOthogonalTrans();

		m_worldOthogTrans = m_viewOthogTrans * m_worldTransformation;
		return m_worldOthogTrans;
	}
/*******************************************************************************
   Function: GetWorldViewTrans

Description: This function gets transformation matrix(world view)

     Inputs: None.

    Outputs: return transformation matrix(world view).
*******************************************************************************/
	const Matrix4f& Pipeline::GetWorldViewTrans()
	{
		GetWorldTrans();
	
		Matrix4f CameraTranslationTrans, CameraRotateTrans;

		CameraTranslationTrans.InitTranslationTransform(-m_camera.pipePos.m_x, -m_camera.pipePos.m_y, -m_camera.pipePos.m_z);
		CameraRotateTrans.InitCameraTransform(m_camera.pipeTarget, m_camera.pipeUp);
	
		m_worldViewTrans = CameraRotateTrans * CameraTranslationTrans * m_worldTransformation;
		return m_worldViewTrans;
	}

/*******************************************************************************
   Function: GetWorldPerspectTrans

Description: This function gets transformation matrix(perspective)

     Inputs: None.

    Outputs: return transformation matrix(perspective).
*******************************************************************************/
	const Matrix4f& Pipeline::GetWorldPerspectTrans()
	{
		Matrix4f PersProjTrans;

		GetWorldTrans();
		PersProjTrans.InitPerspectiveProj(m_persProjInfo.m_FOV, m_persProjInfo.m_width, m_persProjInfo.m_height, m_persProjInfo.m_zNear, m_persProjInfo.m_zFar);

		m_worldPerpectTrans = PersProjTrans * m_worldTransformation;
		return m_worldPerpectTrans;
	}

	const Matrix4f& Pipeline::GetWorldOthogonalTrans()
	{
		Matrix4f OthogProjTrans;

		GetWorldTrans();
		OthogProjTrans.InitOthogonalProj(m_othoProjInfo.m_left, m_othoProjInfo.m_right, m_othoProjInfo.m_top, m_othoProjInfo.m_bottom, m_othoProjInfo.m_farVal, m_othoProjInfo.m_nearVal);

		m_worldOthogTrans = OthogProjTrans * m_worldTransformation;
		return m_worldOthogTrans;
	}

	const Matrix4f& Pipeline::GetViewOthogonalTrans()
	{
		Matrix4f CameraTranslationTrans, CameraRotateTrans, OthogProjTrans;

		CameraTranslationTrans.InitTranslationTransform(-m_camera.pipePos.m_x, -m_camera.pipePos.m_y, -m_camera.pipePos.m_z);
		CameraRotateTrans.InitCameraTransform(m_camera.pipeTarget, m_camera.pipeUp);
		OthogProjTrans.InitOthogonalProj(m_othoProjInfo.m_left, m_othoProjInfo.m_right, m_othoProjInfo.m_top, m_othoProjInfo.m_bottom, m_othoProjInfo.m_farVal, m_othoProjInfo.m_nearVal);

		m_viewPerspectTrans = OthogProjTrans * CameraRotateTrans * CameraTranslationTrans;
		return m_viewOthogTrans;
	}
}