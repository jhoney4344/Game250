/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEOGLMathHelper.cpp
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This file helps graphic engine about math such as matrix operator, vector
type

*******************************************************************************/

#include "JEOGLMathHelper.h"

namespace JE
{
/*******************************************************************************
   Function: InitScaleTransform

Description: This function makes scale transformation matrix

	 Inputs: m_scaleX - scale X
	         m_scaleY - scale Y
			 m_scaleZ - scale Z
			
    Outputs: None.
*******************************************************************************/
	void Matrix4f::InitScaleTransform(float m_scaleX, float m_scaleY, float m_scaleZ)
	{
		m[0][0] = m_scaleX; m[0][1] = 0       ; m[0][2] = 0       ; m[0][3] = 0;
		m[1][0] = 0       ; m[1][1] = m_scaleY; m[1][2] = 0       ; m[1][3] = 0;
		m[2][0] = 0       ; m[2][1] = 0       ; m[2][2] = m_scaleZ; m[2][3] = 0;
		m[3][0] = 0       ; m[3][1] = 0       ; m[3][2] = 0       ; m[3][3] = 1;  
	}
/*******************************************************************************
   Function: InitRotateTransform

Description: This function makes rotation transformation matrix

	 Inputs: m_rotateX - rotate X
	         m_rotateY - rotate Y
			 m_rotateZ - rotate Z
			
    Outputs: None.
*******************************************************************************/
	void Matrix4f::InitRotateTransform(float m_rotateX, float m_rotateY, float m_rotateZ)
	{
		Matrix4f rotateX, rotateY, rotateZ;

		//Change angle to radian for putting in cosf, sinf
		const float x = ToRadian(m_rotateX);
		const float y = ToRadian(m_rotateY);
		const float z = ToRadian(m_rotateZ);

		rotateX.m[0][0] = 1; rotateX.m[0][1] = 0      ; rotateX.m[0][2] = 0       ; rotateX.m[0][3] = 0;
        rotateX.m[1][0] = 0; rotateX.m[1][1] = cosf(x); rotateX.m[1][2] = -sinf(x); rotateX.m[1][3] = 0;
        rotateX.m[2][0] = 0; rotateX.m[2][1] = sinf(x); rotateX.m[2][2] = cosf(x) ; rotateX.m[2][3] = 0;
        rotateX.m[3][0] = 0; rotateX.m[3][1] = 0      ; rotateX.m[3][2] = 0       ; rotateX.m[3][3] = 1;  

		rotateY.m[0][0] = cosf(y); rotateY.m[0][1] = 0; rotateY.m[0][2] = -sinf(y); rotateY.m[0][3] = 0;
        rotateY.m[1][0] = 0      ; rotateY.m[1][1] = 1; rotateY.m[1][2] = 0       ; rotateY.m[1][3] = 0;
        rotateY.m[2][0] = sinf(y); rotateY.m[2][1] = 0; rotateY.m[2][2] = cosf(y) ; rotateY.m[2][3] = 0;
        rotateY.m[3][0] = 0      ; rotateY.m[3][1] = 0; rotateY.m[3][2] = 0       ; rotateY.m[3][3] = 1;  

		rotateZ.m[0][0] = cosf(z); rotateZ.m[0][1] = -sinf(z); rotateZ.m[0][2] = 0; rotateZ.m[0][3] = 0;
        rotateZ.m[1][0] = sinf(z); rotateZ.m[1][1] = cosf(z) ; rotateZ.m[1][2] = 0; rotateZ.m[1][3] = 0;
        rotateZ.m[2][0] = 0      ; rotateZ.m[2][1] = 0       ; rotateZ.m[2][2] = 1; rotateZ.m[2][3] = 0;
        rotateZ.m[3][0] = 0      ; rotateZ.m[3][1] = 0       ; rotateZ.m[3][2] = 0; rotateZ.m[3][3] = 1;  

		*this = rotateZ * rotateY * rotateX;
	}

/*******************************************************************************
   Function: InitTranslationTransform

Description: This function makes translation transformation matrix

	 Inputs: m_worldPosX - World position X
	         m_worldPosY - World position Y
			 m_worldPosZ - World position Z
			
    Outputs: None.
*******************************************************************************/
	void Matrix4f::InitTranslationTransform(float m_worldPosX, float m_worldPosY, float m_worldPosZ)
	{
		m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = m_worldPosX;
		m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = m_worldPosY;
		m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = m_worldPosZ;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;  
	}
/*******************************************************************************
   Function: InitPerspectiveProj

Description: This function makes perspective projection matrix

	 Inputs: m_FOV    - Field of view
	         m_width  - Width of window screen
			 m_height - Height of window screen
			 m_zNear  - Near plane in the z
			 m_zFar   - Far plane in the z
			
    Outputs: None.
*******************************************************************************/
	void Matrix4f::InitPerspectiveProj(float m_FOV,
									   float m_width,
									   float m_height,
									   float m_zNear,
									   float m_zFar)
	{
			const float aspectRatio = m_width / m_height;
			const float zNear       = m_zNear;
			const float zFar        = m_zFar;
			//const float zRange      = zNear - zFar;
			const float zRange = zFar - zNear;
			const float tanHalfFOV  = tanf(ToRadian(m_FOV / 2.0f));

			m[0][0] = 1.0f / (tanHalfFOV * aspectRatio); 
			m[0][1] = 0.0f; 
			m[0][2] = 0.0f; 
			m[0][3] = 0.0f;

			m[1][0] = 0.0f; 
			m[1][1] = 1.0f / tanHalfFOV; 
			m[1][2] = 0.0f; 
			m[1][3] = 0.0f;

			m[2][0] = 0.0f; 
			m[2][1] = 0.0f; 
			m[2][2] = 1 / zRange;//(-zNear - zFar) / zRange; 
			m[2][3] = -zNear / zRange;//2.0f * zFar * zNear / zRange;

			m[3][0] = 0.0f; 
			m[3][1] = 0.0f; 
			m[3][2] = 1.0f;
			m[3][3] = 0.0f;
	}

	void Matrix4f::InitOthogonalProj(float m_left,
									 float m_right,
									 float m_top,
									 float m_bottom,
									 float m_farVal,
									 float m_nearVal)
	{
		m[0][0] = 2.0f / (m_right - m_left);
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;
		m[0][3] = -(m_right+m_left) / (m_right - m_left);

		m[1][0] = 0.0f;
		m[1][1] = 2.0f / (m_top - m_bottom);
		m[1][2] = 0.0f;
		m[1][3] = -(m_top + m_bottom) / (m_top - m_bottom);

		m[2][0] = 0.0f;
		m[2][1] = 0.0f;
		m[2][2] = -2.0f / (m_farVal - m_nearVal);
		m[2][3] = -(m_farVal+m_nearVal)/(m_farVal-m_nearVal);

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}
/*******************************************************************************
   Function: InitCameraTransform

Description: This function makes camera transformation matrix

	 Inputs: target - Target vector
	         up     - Up vector
			
    Outputs: None.
*******************************************************************************/
	void Matrix4f::InitCameraTransform(const Vector3f& target, const Vector3f& up)
	{
		Vector3f N = target;
		N.Normalize();
		Vector3f U = up;
		U.Normalize();
		U = U.Cross(target);
		Vector3f V = N.Cross(U);

		m[0][0] = U.m_x; m[0][1] = U.m_y; m[0][2] = U.m_z; m[0][3] = 0.0f;
		m[1][0] = V.m_x; m[1][1] = V.m_y; m[1][2] = V.m_z; m[1][3] = 0.0f;
		m[2][0] = N.m_x; m[2][1] = N.m_y; m[2][2] = N.m_z; m[2][3] = 0.0f;
		m[3][0] = 0.0f ; m[3][1] = 0.0f ; m[3][2] = 0.0f ; m[3][3] = 1.0f;
	}



	void Matrix4f::InitScreen(int width, int height, int screenX, int screenY)
	{
		m[0][0] = float(width >> 1)          ; m[0][1] = 0.0f                        ; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f                       ; m[1][1] = float(-height >> 1)         ; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f                       ; m[2][1] = 0.0f                        ; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = float(width >> 1) + screenX; m[3][1] = float(height >> 1) + screenY; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}
/*******************************************************************************
   Function: Rotate

Description: This function rotate vector3f variable

	 Inputs: angle - angle
	         axe   - axis
			
    Outputs: None.
*******************************************************************************/
	void Vector3f::Rotate(float angle, const Vector3f& axe)
	{
		const float SinHalfAngle = sinf(ToRadian(angle/2));
		const float CosHalfAngle = cosf(ToRadian(angle/2));

		const float Rx = axe.m_x * SinHalfAngle;
		const float Ry = axe.m_y * SinHalfAngle;
		const float Rz = axe.m_z * SinHalfAngle;
		const float Rw = CosHalfAngle;
		
		Quaternion RotationQ(Rx, Ry, Rz, Rw);

		Quaternion ConjugateQ = RotationQ.Conjugate();
	
		Quaternion W = RotationQ * (*this) * ConjugateQ;

		m_x = W.m_x;
		m_y = W.m_y;
		m_z = W.m_z;
	}
/*******************************************************************************
   Function: Normalize

Description: This function normalize member variable

	 Inputs: None.
			
    Outputs: return *this.
*******************************************************************************/
	Vector3f& Vector3f::Normalize()
	{
		const float length = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);

		m_x /= length;
		m_y /= length;
		m_z /= length;

		return *this;
	}
/*******************************************************************************
   Function: Cross

Description: This function calculate cross product between two Vector3f

	 Inputs: rhs - right hand side(Vector3f)
			
    Outputs: return result value that calculate cross product.
*******************************************************************************/
	Vector3f Vector3f::Cross(const Vector3f &rhs)const
	{
		const float x = m_y * rhs.m_z - m_z * rhs.m_y;
		const float y = m_z * rhs.m_x - m_x * rhs.m_z;
		const float z = m_x * rhs.m_y - m_y * rhs.m_x;

		return Vector3f(x,y,z);
	}
/*******************************************************************************
   Function: Quaternion

Description: This function is conversion constructor about Quaternion

	 Inputs: x - x
	         y - y
			 z - z
			 w - w
			
    Outputs: None.
*******************************************************************************/
	Quaternion::Quaternion(float x, float y, float z, float w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}
/*******************************************************************************
   Function: Normalize

Description: This function nomalize member variable in Quaternion

	 Inputs: None
			
    Outputs: None.
*******************************************************************************/
	void Quaternion::Normalize()
	{
		float Length = sqrtf(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);

		m_x /= Length;
		m_y /= Length;
		m_z /= Length;
		m_w /= Length;
	}

/*******************************************************************************
   Function: Conjugate

Description: This function calculate conjugate. 

	 Inputs: None.
			
    Outputs: return result that calculates conjugate.
*******************************************************************************/
	Quaternion Quaternion::Conjugate()
	{
		Quaternion result(-m_x, -m_y, -m_z, m_w);
		return result;
	}
/*******************************************************************************
   Function: operator*

Description: This function multiplies two Quaternion.

	 Inputs: l - left hand side(Quaternion)
	         r - right hand side(Quaternion)
			
    Outputs: return result that multiplies two Quaternion
*******************************************************************************/
	Quaternion operator*(const Quaternion& l, const Quaternion& r)
	{
		const float w = (l.m_w * r.m_w) - (l.m_x * r.m_x) - (l.m_y * r.m_y) - (l.m_z * r.m_z);
		const float x = (l.m_x * r.m_w) + (l.m_w * r.m_x) + (l.m_y * r.m_z) - (l.m_z * r.m_y);
		const float y = (l.m_y * r.m_w) + (l.m_w * r.m_y) + (l.m_z * r.m_x) - (l.m_x * r.m_z);
		const float z = (l.m_z * r.m_w) + (l.m_w * r.m_z) + (l.m_x * r.m_y) - (l.m_y * r.m_x);

		Quaternion result(x, y, z, w);

		return result;
	}
/*******************************************************************************
   Function: operator*

Description: This function multiplies Quaternion and Vector3f.

	 Inputs: q - Quaternion
	         v - Vector3f
			
    Outputs: return result that multiplies Quaternion and Vector3f.
*******************************************************************************/
	Quaternion operator*(const Quaternion& q, const Vector3f& v)
	{
		const float w = - (q.m_x * v.m_x) - (q.m_y * v.m_y) - (q.m_z * v.m_z);
		const float x =   (q.m_w * v.m_x) + (q.m_y * v.m_z) - (q.m_z * v.m_y);
		const float y =   (q.m_w * v.m_y) + (q.m_z * v.m_x) - (q.m_x * v.m_z);
		const float z =   (q.m_w * v.m_z) + (q.m_x * v.m_y) - (q.m_y * v.m_x);

		Quaternion result(x, y, z, w);

		return result;
	}

	float RandomFloat()
	{
		float Max = RAND_MAX;
		return ((float)RANDOM() / Max);
	}

}