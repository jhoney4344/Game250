/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEPipeLine.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header makes pipe line for various space

*******************************************************************************/

#ifndef  JEPIPELINE_H
#define  JEPIPELINE_H

#include "JEOGLMathHelper.h"
#include "JECamera.h"

namespace JE
{
	class Pipeline
	{
	public:
/*******************************************************************************
   Function: Pipeline

Description: This function is default constructor for pipe line

     Inputs: None.

    Outputs: None.
*******************************************************************************/
		Pipeline()
		{
			m_scale      = Vector3f(1.0f, 1.0f, 1.0f);
			m_worldPos   = Vector3f(0.0f, 0.0f, 0.0f);
			m_rotate = Vector3f(0.0f, 0.0f, 0.0f);
		}
/*******************************************************************************
   Function: Scale

Description: This function is settor about scale variable

     Inputs: scaleX - scale X
			 scaleY - scale Y
			 scaleZ - scale Z

    Outputs: None.
*******************************************************************************/
		void Scale(float scaleX, float scaleY, float scaleZ)
		{
			m_scale.m_x = scaleX;
			m_scale.m_y = scaleY;
			m_scale.m_z = scaleZ;
		}
/*******************************************************************************
   Function: WorldPos

Description: This function is settor about position variable

     Inputs: x - world position X
			 y - world position Y
			 z - world position Z

    Outputs: None.
*******************************************************************************/
		void WorldPos(float x, float y, float z)
		{
			m_worldPos.m_x = x;
			m_worldPos.m_y = y;
			m_worldPos.m_z = z;
		}

		inline Vector3f getWorldPos(void)
		{
			return m_worldPos;
		}
/*******************************************************************************
   Function: Rotate

Description: This function is settor about rotate variable

     Inputs: rotateX - rotate X
			 rotateY - rotate Y
			 rotateZ - rotate Z

    Outputs: None.
*******************************************************************************/
		void Rotate(float rotateX, float rotateY, float rotateZ)
		{
			m_rotate.m_x = rotateX;
			m_rotate.m_y = rotateY;
			m_rotate.m_z = rotateZ;
		}
/*******************************************************************************
   Function: SetPerspective

Description: This function is settor about perspective projection information 
             variable 

     Inputs: FOV    - Field of view
			 width  - Width of window screen
			 height - Height of window screen
			 zNear  - Near plane in the z
			 zFar   - Far plane in the z
    Outputs: None.
*******************************************************************************/
		void SetPerspective(float FOV,
							float width,
							float height,
							float zNear,
							float zFar)
		{
			m_persProjInfo.m_FOV    = FOV;
			m_persProjInfo.m_height = height;
			m_persProjInfo.m_width  = width;
			m_persProjInfo.m_zFar   = zFar;
			m_persProjInfo.m_zNear  = zNear;
		}

		void SetOthogonal(float left,
						  float right,
						  float top,
						  float bottom,
						  float farVal,
						  float nearVal)
		{
			m_othoProjInfo.m_left    = left;
			m_othoProjInfo.m_right   = right;
			m_othoProjInfo.m_top     = top;
			m_othoProjInfo.m_bottom  = bottom;
			m_othoProjInfo.m_farVal  = farVal;
			m_othoProjInfo.m_nearVal = nearVal;
		}

/*******************************************************************************
   Function: SetPerspective

Description: This function is settor about perspective projection information 
             variable with PersProjInfo type

     Inputs: p  - Perspective projection information

    Outputs: None.
*******************************************************************************/
		void SetPerspective(const PersProjInfo& p)
		{
			m_persProjInfo = p;
		}

		void SetOthogonal(const OthoProjInfo& o)
		{
			m_othoProjInfo = o;
		}
/*******************************************************************************
   Function: SetCamera

Description: This function is settor about camera variable

     Inputs: Pos    - Camera position
			 Target - Target vector(z-axis)
			 Up     - Up vector

    Outputs: None.
*******************************************************************************/
		void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
		{
			m_camera.pipePos    = Pos;
			m_camera.pipeTarget = Target;
			m_camera.pipeUp     = Up;
		}
/*******************************************************************************
   Function: SetCamera

Description: This function is settor about camera variable with Camera type

     Inputs: camera - Camera class

    Outputs: None.
*******************************************************************************/
		void SetCamera(const Camera& camera)
		{
		   SetCamera(camera.getPos(), camera.getTarget(), camera.getUp());
		}
/*******************************************************************************
   Function: GetTrans

Description: This function gets transformation matrix(world to view)

     Inputs: None.

    Outputs: return transformation matrix(world to view).
*******************************************************************************/
		const Matrix4f* GetTrans()
		{
			Matrix4f ScaleTrans, 
					 RotateTrans, 
					 TranslationTrans, 
					 PersProjection,
					 CameraTranslationTrans,
					 CameraRotateTrans;

			ScaleTrans.InitScaleTransform(m_scale.m_x, m_scale.m_y, m_scale.m_z);
			RotateTrans.InitRotateTransform(m_rotate.m_x, m_rotate.m_y, m_rotate.m_z);
			TranslationTrans.InitTranslationTransform(m_worldPos.m_x, m_worldPos.m_y, m_worldPos.m_z);
		
			CameraTranslationTrans.InitTranslationTransform(-m_camera.pipePos.m_x, -m_camera.pipePos.m_y, -m_camera.pipePos.m_z);
			CameraRotateTrans.InitCameraTransform(m_camera.pipeTarget, m_camera.pipeUp);
		
			PersProjection.InitPerspectiveProj(m_persProjInfo.m_FOV, m_persProjInfo.m_width, m_persProjInfo.m_height, m_persProjInfo.m_zNear, m_persProjInfo.m_zFar);

			m_transformation = PersProjection * 
							   CameraRotateTrans * 
							   CameraTranslationTrans * 
							   TranslationTrans * 
							   RotateTrans * 
							   ScaleTrans;
			return &m_transformation;
		}

		const Matrix4f& GetWorldTrans();
		const Matrix4f& GetViewPerspectTrans();
		const Matrix4f& GetWorldViewPerspectTrans();
		const Matrix4f& GetWorldViewTrans();
		const Matrix4f& GetWorldPerspectTrans();

		//othogonal
		const Matrix4f& GetWorldOthogonalTrans();
		const Matrix4f& GetViewOthogonalTrans();
		const Matrix4f& GetWorldViewOthogonalTrans();

		
	private:
		Vector3f m_scale;
		Vector3f m_worldPos;
		Vector3f m_rotate;
		Matrix4f m_transformation;

		PersProjInfo m_persProjInfo;
		OthoProjInfo m_othoProjInfo;

		struct
		{
			Vector3f pipePos;
			Vector3f pipeTarget;
			Vector3f pipeUp;
		}m_camera;

		Matrix4f m_worldTransformation;
		Matrix4f m_worldPerspectTrans;
		Matrix4f m_viewPerspectTrans;
		Matrix4f m_worldViewTrans;
		Matrix4f m_worldPerpectTrans;
		Matrix4f m_worldOthogTrans;
		Matrix4f m_viewOthogTrans;
		
	};
}

#endif