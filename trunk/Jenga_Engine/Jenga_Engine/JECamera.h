/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JECamera.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header provides the controller of camera position and zoom.

*******************************************************************************/
#ifndef JECAMERA_H
#define	JECAMERA_H

#include "JEOGLMathHelper.h"

namespace JE
{
	class Camera
	{
	public:

		//Conversion constructor
		Camera(int windowWidth, int windowHeight);

		Camera(int windowWidth, int windowHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);

/*******************************************************************************
   Function: getPos

Description: This function is getter function about camera position

     Inputs: None.

    Outputs: return camera position
*******************************************************************************/
		const Vector3f& getPos() const
		{
			return m_pos;
		}
/*******************************************************************************
Function: setPos

Description: This function is setter function about camera position

Inputs: camera position that wants to change.

Outputs: None.
*******************************************************************************/
		void setPos(const Vector3f pos)
		{
			m_pos = pos;
		}

/*******************************************************************************
   Function: getTarget

Description: This function is getter function about camera target(z-axis)

     Inputs: None.

    Outputs: return camera target(z-axis)
*******************************************************************************/
		const Vector3f& getTarget() const
		{
			return m_target;
		}
/*******************************************************************************
   Function: getUp

Description: This function is getter function about camera up vector

     Inputs: None.

    Outputs: return camera up vector
*******************************************************************************/
		const Vector3f& getUp() const
		{
			return m_up;
		}
		//Control the camera position
		void SetCameraPosition(float x, float y);
		//Control the camera zoom
		void SetCameraZoom(bool zoomIn, float magnification);

	private:
		//Camera position
		Vector3f m_pos;
		//Target z-axis
		Vector3f m_target;
		//Up vertor for up vector
		Vector3f m_up;

		//Window screen width and height
		int m_windowWidth;
		int m_windowHeight;
	};
}

#endif	
