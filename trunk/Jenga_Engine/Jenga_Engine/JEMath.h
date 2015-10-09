/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.

filename    JEMath.h
author      Nahye Park
email       skqla12@gmail.com
course      GAM250
due date    12/6/2015

Brief Description:
  This file contains the function declaration for JEMath.
  Basic math functions for project.
*******************************************************************************/

#ifndef JENGA_ENGINE_MATH
#define JENGA_ENGINE_MATH

#include <cmath>
#include "JEDebug.h"

//for DegreeToRadian
const double pi = 3.14159265;

namespace JE{
	const double GRAVITY = 5;
	const double GRAVITY_FLY = 2;
}
class JEPoint2D
{
public:

	double x;
	double y;

	JEPoint2D(void) : x(0), y(0) {  };
	JEPoint2D(double _x , double _y){ x = _x; y = _y; };
	~JEPoint2D(void){};

friend	JEPoint2D operator-(const JEPoint2D& point1, const JEPoint2D& point2);
friend	JEPoint2D operator+(const JEPoint2D& point1, const JEPoint2D& point2);
friend	bool operator==(const JEPoint2D& point1, const JEPoint2D& point2);
friend	bool operator>=(const JEPoint2D& point1, const JEPoint2D& point2);
friend	bool operator<=(const JEPoint2D& point1, const JEPoint2D& point2);
friend	bool operator>(const JEPoint2D& point1, const JEPoint2D& point2);
friend	bool operator<(const JEPoint2D& point1, const JEPoint2D& point2);
};


class JE_MATH : public JEPoint2D
{
public:
	JE_MATH(void);
	~JE_MATH(void){};

	//Add and subtract points
	JEPoint2D AddPoint(const JEPoint2D& point1, const JEPoint2D& point2);
	JEPoint2D SubPoint(const JEPoint2D& point1, const JEPoint2D& point2);

	//Normalize the vector
	JEPoint2D Normalize(const JEPoint2D& point);
	//Scale the vector
	JEPoint2D Scale(const JEPoint2D& point, double scale);
	//Normal vector
	JEPoint2D NormalVec(const JEPoint2D& vector);

	//Return the length of vector
	double Length(const JEPoint2D& vector);
	//Dot prouct
	double DotProduct(const JEPoint2D& vector1, const JEPoint2D& vector2);

	//Set x,y of point(vector)
	void PointSet(JEPoint2D& result, double x, double y);

	//distnace functions
	double DistancePointPoint(const JEPoint2D& start, const JEPoint2D& end);
	double DistancePointLine(const JEPoint2D& point, const JEPoint2D& lineStart, const JEPoint2D& lineEnd);

	template <typename T>
	T Max(const T& x, const T& y);

	template <typename T>
	T Min(const T& x, const T& y);

	template <typename T>
	bool IsEqual(const T& x, const T& y);

	template <typename T>
	bool InRange(const T& x, const T& start, const T& end);

	double DegreeToRadian(double degree);

};


typedef JEPoint2D JEVector2D;

extern JE_MATH *g_Math;

//clock

#endif