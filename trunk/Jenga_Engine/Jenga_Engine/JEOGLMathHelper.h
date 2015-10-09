/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
filename    JEOGLMathHelper.h
author      Seho.Joo
email       jsh2958@gmail.com
course      GAM250
assignment  Final milestone
due date    6/12/2015

Brief Description:
This header helps graphic engine about math such as matrix operator, vector
type

*******************************************************************************/

#ifndef  JEOGL_MATH_HELPER_H
#define  JEOGL_MATH_HELPER_H

#include <cmath>
#include <stdlib.h>

#define M_PI       3.14159265358979323846f
#define M_PI_2     1.57079632679489661923f
#define M_PI_4     0.785398163397448309616f

//micro function for calculating rotate transformation
#define ToRadian(x) ((x) * M_PI/180.0f)
#define ToDegree(x) ((x) * 180.0f/M_PI)
#define RANDOM rand

namespace JE
{
	float RandomFloat();
	//perspective projection information
	class PersProjInfo
	{
	public:
		float m_FOV;
		float m_width; 
		float m_height;
		float m_zNear;
		float m_zFar;
	};

	class OthoProjInfo
	{
	public:
		float m_left;
		float m_right;
		float m_top;
		float m_bottom;
		float m_farVal;
		float m_nearVal;
	};


	class Vector2f
	{
	public:
		float m_x;
		float m_y;
/*******************************************************************************
   Function: Vector2f

Description: This function is default constructor for Vector2f

    Outputs: None.
*******************************************************************************/
		Vector2f():m_x(0),m_y(0){}

/*******************************************************************************
   Function: Vector2f

Description: This function is conversion constructor for Vector2f

     Inputs: x - vector x
			 y - vector y 

    Outputs: None.
*******************************************************************************/
		Vector2f(float x, float y)
		{
			m_x = x;
			m_y = y;
		}
	};

	class Vector3f
	{
	public:
		float m_x;
		float m_y;
		float m_z;

/*******************************************************************************
   Function: Vector3f

Description: This function is default constructor for Vector3f

    Outputs: None.
*******************************************************************************/
		Vector3f():m_x(0),m_y(0),m_z(0){}

/*******************************************************************************
   Function: Vector3f

Description: This function is conversion constructor for Vector3f

     Inputs: x - vector x
			 y - vector y 
			 z - vector z

    Outputs: None.
*******************************************************************************/
		Vector3f(float x, float y, float z)
		{
			m_x = x;
			m_y = y;
			m_z = z;
		}
/*******************************************************************************
   Function: operator=

Description: This function calculate operator '=' between vector3f and vector3f

     Inputs: r - right hand side(vector3f)

    Outputs: return *this variable.
*******************************************************************************/
		Vector3f& operator=(const Vector3f& r)
		{
			m_x = r.m_x;
			m_y = r.m_y;
			m_z = r.m_z;

			return *this;
		}
/*******************************************************************************
   Function: operator+=

Description: This function calculate operator '+=' between vector3f and vector3f

     Inputs: r - right hand side(vector3f)

    Outputs: return *this variable.
*******************************************************************************/
		Vector3f& operator+=(const Vector3f& r)
		{
			m_x += r.m_x;
			m_y += r.m_y;
			m_z += r.m_z;

			return *this;
		}
/*******************************************************************************
   Function: operator-=

Description: This function calculate operator '-=' between vector3f and vector3f

     Inputs: r - right hand side(vector3f)

    Outputs: return *this variable.
*******************************************************************************/
		Vector3f& operator-=(const Vector3f& r)
		{
			m_x -= r.m_x;
			m_y -= r.m_y;
			m_z -= r.m_z;

			return *this;
		}
/*******************************************************************************
   Function: operator*=

Description: This function calculate operator '*=' between vector3f and float

     Inputs: f - right hand side(float)

    Outputs: return *this variable.
*******************************************************************************/
		Vector3f& operator*=(float f)
		{
			m_x *= f;
			m_y *= f;
			m_z *= f;

			return *this;
		}

		Vector3f& operator*(const Vector3f& r)
		{
			m_x *= r.m_x;
			m_y *= r.m_y;
			m_z *= r.m_z;

			return *this;
		}

		Vector3f &Normalize();
		Vector3f Cross(const Vector3f &rhs)const;
		void Rotate(float angle, const Vector3f& axe);
	};

	class Vector4f
	{
	public:
		float m_x;
		float m_y;
		float m_z;
		float m_w;

/*******************************************************************************
   Function: Vector4f

Description: This function is default constructor for Vector4f

    Outputs: None.
*******************************************************************************/
		Vector4f():m_x(0),m_y(0),m_z(0),m_w(0){}

/*******************************************************************************
   Function: Vector4f

Description: This function is conversion constructor for Vector4f

	 Inputs: x - vector x
	         y - vector y
			 z - vector z
			 w - vector w

    Outputs: None.
*******************************************************************************/
		Vector4f(float x, float y, float z, float w)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_w = w;
		}
	};

/*******************************************************************************
   Function: operator+

Description: This function calculate addition between two vector3f

	 Inputs: l - left hand side(vector3f)
	         r - right hand side(vector3f)

    Outputs: return result value that multiplies between two vector3f
*******************************************************************************/
	inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
	{
		Vector3f Ret(l.m_x + r.m_x,
			         l.m_y + r.m_y,
				     l.m_z + r.m_z);

		return Ret;
	}

/*******************************************************************************
   Function: operator-

Description: This function calculate substraction between two vector3f

	 Inputs: l - left hand side(vector3f)
	         r - right hand side(vector3f)

    Outputs: return result value that substracts between two vector3f
*******************************************************************************/
	inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
	{
		Vector3f Ret(l.m_x - r.m_x,
			         l.m_y - r.m_y,
				     l.m_z - r.m_z);

		return Ret;
	}

/*******************************************************************************
   Function: operator*

Description: This function calculate multiplication between vector3f and float

	 Inputs: l - left hand side(vector3f)
	         f - right hand side(float)

    Outputs: return result value that multiplies between vector3f and float
*******************************************************************************/
	inline Vector3f operator*(const Vector3f& l, float f)
	{
		Vector3f Ret(l.m_x * f,
			         l.m_y * f,
				     l.m_z * f);

		return Ret;
	}

	class Matrix4f
	{
	public:
		float m[4][4];
/*******************************************************************************
   Function: Matrix4f

Description: This function is default constructor for Matrix4f

    Outputs: None.
*******************************************************************************/
		Matrix4f()
		{
			m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
			m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
			m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;  
		}
/*******************************************************************************
   Function: Matrix4f

Description: This function is conversion constructor for Matrix4f

	 Inputs:  m00 - 00  m01 - 01  m02 - 02  m03 - 03 (Matrix)
		      m10 - 10  m11 - 11  m12 - 12  m13 - 13 (Matrix)
			  m20 - 20  m21 - 21  m22 - 22  m23 - 23 (Matrix)
			  m30 - 30  m31 - 31  m32 - 32  m33 - 33 (Matrix)
			
    Outputs: None.
*******************************************************************************/

		Matrix4f(float m00, float m01, float m02, float m03,
				 float m10, float m11, float m12, float m13,
				 float m20, float m21, float m22, float m23,
				 float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}
/*******************************************************************************
   Function: operator*

Description: This function calculate multiplication between two Marix4f

	 Inputs: rhs - right hand side(Matrix4f)
			
    Outputs: return result value that multiplies between two Matrix4f
*******************************************************************************/
		inline Matrix4f operator*(const Matrix4f &rhs) const
		{
			Matrix4f result;
			for(unsigned int i=0; i<4; ++i)
			{
				for(unsigned int j=0; j<4;++j)
				{
					result.m[i][j] = m[i][0] * rhs.m[0][j] +
									 m[i][1] * rhs.m[1][j] + 
									 m[i][2] * rhs.m[2][j] +
									 m[i][3] * rhs.m[3][j];
				}
			}
			return result;
		}

		inline Matrix4f operator+(const Matrix4f &rhs) const
		{
			Matrix4f result;
			for (unsigned int i = 0; i<4; ++i)
			{
				for (unsigned int j = 0; j<4; ++j)
				{
					result.m[i][j] = m[i][j] + rhs.m[i][j];
				}
			}
			return result;
		}
/*******************************************************************************
   Function: operator*

Description: This function calculate multiplication between two Vector4f

	 Inputs: rhs - right hand side(Vector4f)
			
    Outputs: return result value that multiplies between two Vector4f
*******************************************************************************/
		Vector4f operator*(const Vector4f& rhs) const
		{
			Vector4f result;
		
			result.m_x = m[0][0]* rhs.m_x + m[0][1]* rhs.m_y + m[0][2]* rhs.m_z + m[0][3]* rhs.m_w;
			result.m_y = m[1][0]* rhs.m_x + m[1][1]* rhs.m_y + m[1][2]* rhs.m_z + m[1][3]* rhs.m_w;
			result.m_z = m[2][0]* rhs.m_x + m[2][1]* rhs.m_y + m[2][2]* rhs.m_z + m[2][3]* rhs.m_w;
			result.m_w = m[3][0]* rhs.m_x + m[3][1]* rhs.m_y + m[3][2]* rhs.m_z + m[3][3]* rhs.m_w;
        
			return result;
		}
/*******************************************************************************
   Function: const float*(operator)

Description: This function return the address of first element in matrix4f

	 Inputs: None.
			
    Outputs: return address value about first element in matrix4f 
*******************************************************************************/

		operator const float*() const
		{
			return &(m[0][0]);
		}

		void InitScaleTransform(float m_scaleX, float m_scaleY, float m_scaleZ);
		void InitRotateTransform(float m_rotateX, float m_rotateY, float m_rotateZ);
		void InitTranslationTransform(float m_worldPosX, float m_worldPosY, float m_worldPosZ);
		void InitPerspectiveProj(float m_FOV,
								 float m_width,
								 float m_height,
								 float m_zNear,
								 float m_zFar); 
		void InitOthogonalProj(float m_left,
							   float m_right,
							   float m_top,
							   float m_bottom,
							   float m_farVal,
							   float m_nearVal);
		
		void InitCameraTransform(const Vector3f& target, const Vector3f& up);
		void InitScreen(int width, int height, int screenSizeX, int screenSizeY);
	};

	
	struct Quaternion
	{
		float m_x, m_y, m_z, m_w;

/*******************************************************************************
   Function: Quaternion

Description: This function is conversion constructor about Quaternion

	 Inputs: x - Quaternion X 
	         y - Quaternion Y
			 z - Quaternion Z 
			 w - Quaternion W 
			
    Outputs: None.
*******************************************************************************/
		Quaternion(float x, float y, float z, float w);

		void Normalize();

		Quaternion Conjugate();  
	 };

	Quaternion operator*(const Quaternion& l, const Quaternion& r);

	Quaternion operator*(const Quaternion& q, const Vector3f& v);
}
#endif