/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.

filename    JEMath.cpp
author      Nahye Park
email       skqla12@gmail.com
course      GAM250
due date    12/6/2015

Brief Description:
  This file contains the function definition for JEMath.

*******************************************************************************/

#include "JEMath.h"

JE_MATH *g_Math = NULL;

//constructor
/*******************************************************************************
   Function: JE_MATH

Description: Constructor.
             

     Inputs: None.
             

    Outputs: None.
*******************************************************************************/
JE_MATH::JE_MATH(void):JEPoint2D()
{
	g_Math = this;
}

//Add Point or Vector
/*******************************************************************************
   Function: AddPoint

Description: Add point or vector.
             

     Inputs: point1 - point1 to add.
             point2 - point2 to add.

    Outputs: JEPoint2D - the result point.
*******************************************************************************/
JEPoint2D JE_MATH::AddPoint(const JEPoint2D& point1, const JEPoint2D& point2){
	
	return (point1+point2);
}

//Subtract Point or Vector
/*******************************************************************************
   Function: SubPoint

Description: Subtract point or vector.
             

     Inputs: point1 - point1 to subtract.
             point2 - point2 to subtract.

    Outputs: JEPoint2D - the result point..
*******************************************************************************/
JEPoint2D JE_MATH::SubPoint(const JEPoint2D& point1, const JEPoint2D& point2){
	return (point1-point2);
}

//Normalize the vector
/*******************************************************************************
   Function: Normalize

Description: Normalize the vector
             

     Inputs: point - vector to normalize.
             

    Outputs: JEPoint2D - the result point (normalized point).
*******************************************************************************/
JEPoint2D JE_MATH::Normalize(const JEPoint2D& point){
	JEPoint2D result;
	double length;

	PointSet(result,0,0);
	length = Length(point);
	if(length)    
		PointSet(result,point.x/length,point.y/length);

	return result;
}

//Scale the vector or point
/*******************************************************************************
   Function: Scale

Description: Scale the vector or point.
             

     Inputs: point - vector to scale.
             scale - double number to multiply to vector

    Outputs: JEPoint2D - result point.
*******************************************************************************/
JEPoint2D JE_MATH::Scale(const JEPoint2D& point, double scale){
	JEPoint2D result;
	PointSet(result,0,0);
	if(scale)
		PointSet(result,point.x*scale,point.y*scale);

	return result;
}

//Normal vector
/*******************************************************************************
   Function: NormalVec

Description: Fine normal vector
             

     Inputs: vector - vector to fine normal vector.
             

    Outputs: JEPoint2D - normal vector of input vector.
*******************************************************************************/
JEPoint2D JE_MATH::NormalVec(const JEPoint2D& vector)
{
	return JEPoint2D(-vector.y , vector.x);
}

//Return the length of vector
/*******************************************************************************
   Function: Length

Description: Calculate the length of vector or magnitude of point.
             

     Inputs: vector - to calculate the length.
             

    Outputs: double - length of vector.
*******************************************************************************/
double JE_MATH::Length(const JEPoint2D& vector){
	return (std::sqrt((vector.x)*(vector.x) + (vector.y)*(vector.y)) );
}

//Dot product
/*******************************************************************************
   Function: DotProduct

Description: Caculate the dot product.
             

     Inputs: vector1 - vector1 for dot product.
             vector2 - vector2 for dot product.

    Outputs: double - the result value of dot product.
*******************************************************************************/
double JE_MATH::DotProduct(const JEPoint2D& vector1, const JEPoint2D& vector2){
	return ( (vector1.x)*(vector2.x) + (vector1.y)*(vector2.y) );
}

//Set the point or vector x,y
/*******************************************************************************
   Function: PointSet

Description: Set the point.
             

     Inputs: result - point to set.
	         x - x value of point.
             y - y value of point.

    Outputs: None.
*******************************************************************************/
void JE_MATH::PointSet(JEPoint2D& result, double x, double y){
	result.x = x;
	result.y = y;

}

//Return distance of point to point
/*******************************************************************************
   Function: DistancePointPoint

Description: Calculate distance of point to point.
             

     Inputs: start - start point to calculate.
             end - end point to calculate.

    Outputs: double - distance between point and point.
*******************************************************************************/
double JE_MATH::DistancePointPoint(const JEPoint2D& start, const JEPoint2D& end){
	JEPoint2D subVec = SubPoint(end,start);

	return Length(subVec);
}

//Return distance of point to line
/*******************************************************************************
   Function: DistancePointLine

Description: Calculate the distance of point to line.
             

     Inputs: point - point to calculate distance with line.
             lineStart - start point of the line.
			 lineEnd - end point of the line.

    Outputs: double - distance between point and line.
*******************************************************************************/
double JE_MATH::DistancePointLine(const JEPoint2D& point, const JEPoint2D& lineStart, const JEPoint2D& lineEnd){
	JEPoint2D vec = lineEnd - lineStart;
	JEPoint2D pointToStart = point - lineStart;
	double dot1 = DotProduct(vec,pointToStart);
	double dot2 = DotProduct(vec,vec);

	if((vec.x == 0) && (vec.y == 0))
	{
		PointSet(vec,point.x - lineStart.x , point.y - lineStart.y);
		return Length(vec);
	}

	if(dot1 <= 0)
		PointSet(vec, point.x - lineStart.x , point.y - lineStart.y);
	else if(dot2 <= dot1)
		PointSet(vec, point.x - lineEnd.x , point.y - lineEnd.y);
	else
		PointSet(vec,point.x - (lineStart.x + (dot1/dot2)*vec.x), point.y - (lineStart.y + (dot1/dot2)*vec.y));


	return Length(vec);

}

/*******************************************************************************
   Function: Max

Description: template function to return the max value between two value.
             

     Inputs: x - value to compare.
             y - value to compare.

    Outputs: T - max value between two input.
*******************************************************************************/
//Return larger value
template <typename T>
T JE_MATH::Max(const T& x, const T& y){
	return (x>y)?x:y;
}

//Return smaller value
/*******************************************************************************
   Function: Min

Description: Return the min value.
             

     Inputs: x - value to compare.
             y - value to compare.

    Outputs: T - smaller value between two input.
*******************************************************************************/
template <typename T>
T JE_MATH::Min(const T& x, const T& y){
	return (x<y)?x:y;
}

//Check if two parameter is same
/*******************************************************************************
   Function: IsEqual

Description: Check if two parameter is same.
             

     Inputs: x - value to compare.
             y - value to compare.

    Outputs: true - if two inputs are same.
	         false - if two inputs are not same.
*******************************************************************************/
template <typename T>
bool JE_MATH::IsEqual(const T& x, const T& y){
	return (x == y);
}

//Check if x is exist between start and end
/*******************************************************************************
   Function: InRange

Description: Check if x is exist between start and end.
             

     Inputs: x - value to compare.
             start - the start value of range.
			 end - the end value of range.

    Outputs: true - if x is exist between start and end.
	         false - if x is not exist between start and end.
*******************************************************************************/
template <typename T>
bool JE_MATH::InRange(const T& x, const T& start, const T& end){
	return (start <= x && x <= end);
}

//Convert degree to radian
/*******************************************************************************
   Function: DegreeToRadian

Description: Convert degree to radian.
             

     Inputs: degree - degree to convert.
             

    Outputs: doube - radian value.
*******************************************************************************/
double JE_MATH::DegreeToRadian(double degree){
	return degree * pi/180.0;
}

//overloading functions for point(vector)
/*******************************************************************************
   Function: operator-

Description: operator overloading function to subtract two points.
             point1 - point2.

     Inputs: point1 - point1 to calculate.
             point2 - point2 to calculate.

    Outputs: JEPoint2D - the result of subtraction.
*******************************************************************************/
JEPoint2D operator-(const JEPoint2D& point1, const JEPoint2D& point2){
	JEPoint2D result;
	result.x = point1.x - point2.x;
	result.y = point1.y - point2.y;
	return result;
}

/*******************************************************************************
   Function: operator+

Description: operator overloading function to add two points.
             point1 + point2.

     Inputs: point1 - point1 to calculate.
             point2 - point2 to calculate.

    Outputs: JEPoint2D - the result of addition.
*******************************************************************************/
JEPoint2D operator+(const JEPoint2D& point1, const JEPoint2D& point2){
	JEPoint2D result;
	result.x = point1.x + point2.x;
	result.y = point1.y + point2.y;
	return result;
}

/*******************************************************************************
   Function: operator==

Description: operator overloading function to compare two points.
             

     Inputs: point1 - point1 to compare.
             point2 - point2 to compare.

    Outputs: true - if point1 and point2 are equal.
	         false - if point1 and point2 are not equal.
*******************************************************************************/
bool operator==(const JEPoint2D& point1, const JEPoint2D& point2){
	return (point1.x == point2.x && point1.y == point2.y);
}

/*******************************************************************************
   Function: operator>=

Description: operator overloading function to compare two points.
             

     Inputs: point1 - point1 to compare.
             point2 - point2 to compare.
             

    Outputs: true - if point1 is bigger than point2 or same with point2.
	         false - if point1 is less than point2.
*******************************************************************************/
bool operator>=(const JEPoint2D& point1, const JEPoint2D& point2){
	return (point1.x >= point2.x && point1.y >= point2.y);
}

/*******************************************************************************
   Function: operator<=

Description: operator overloading function to compare two points.
             

     Inputs: point1 - point1 to compare.
             point2 - point2 to compare.
             

    Outputs: true - if point1 is less than point2 or same with point2.
	         false - if point1 is bigger than point2.
*******************************************************************************/
bool operator<=(const JEPoint2D& point1, const JEPoint2D& point2){
	return (point1.x <= point1.x && point1.y <= point2.y);
}

/*******************************************************************************
   Function: operator>

Description: operator overloading function to compare two points.
             

     Inputs: point1 - point1 to compare.
             point2 - point2 to compare.
             

    Outputs: true - if point1 is bigeer than point2.
	         false - if point1 is less than point2  or same with point2.
*******************************************************************************/
bool operator>(const JEPoint2D& point1, const JEPoint2D& point2){
	return (point1.x > point2.x && point1.y > point2.y);
}

/*******************************************************************************
   Function: operator<

Description: operator overloading function to compare two points.
             

     Inputs: point1 - point1 to compare.
             point2 - point2 to compare.
             

    Outputs: true - if point1 is less than point2.
	         false - if point1 is bigger than point2  or same with point2.
*******************************************************************************/
bool operator<(const JEPoint2D& point1, const JEPoint2D& point2){
	return (point1.x < point2.x && point1.y < point2.y);
}
