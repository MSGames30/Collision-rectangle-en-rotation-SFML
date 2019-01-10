#include "stdafx.h"
#include "UtilsCpp.h"



#ifndef PI
#define PI 3.14159265358979323846264338327
#endif





//////////////////////////////////////
/// Check if mouse is over a rectangle
/// \_window : The render window
/// \_rectangle : The rectangle to check if we are over
/// return EXIT_SUCCESS if we are over the sprite, else EXIT_FAILURE
//////////////////////////////////////
int IsOverRect(RenderWindow *_window, RectangleShape *_rectangle)
{
	FloatRect boundingBox = _rectangle->getGlobalBounds();

	Vector2i mousePos = Mouse::getPosition(*_window);

	if (mousePos.x > boundingBox.left && mousePos.x < boundingBox.left + boundingBox.width
		&& mousePos.y > boundingBox.top && mousePos.y < boundingBox.top + boundingBox.height)
	{
		return 1;
	}

	return 0;
}

//////////////////////////////////////
/// Check if 2 float rect are colliding as AABB (Axis Aligned Bounding Box)
/// \_rect1 : The first AABB
/// \_rect2 : The second AABB
/// return 1 if collision detected, else 0
//////////////////////////////////////
int CollisionAABBVsAABB(FloatRect _rect1, FloatRect _rect2)
{
	/// Check if we are out of the box
	if ((_rect2.left >= _rect1.left + _rect1.width)      /// AABB 2 is on the right
		|| (_rect2.left + _rect2.width <= _rect1.left) /// AABB 2 is on the left
		|| (_rect2.top >= _rect1.top + _rect1.height) /// AABB 2 is at the bottom
		|| (_rect2.top + _rect2.height <= _rect1.top))  /// AABB 2 is at the top
	{
		return 0;
	}
	else
		return 1;
}

//////////////////////////////////////
/// Check if a circle is colliding with a float rect are colliding
/// \_circlePosition : The circle position
/// \_circleBoundingBox : The circle bounding box
/// \_AABB : The rect bounding box
/// return 1 if collision detected, else 0
//////////////////////////////////////
int CollisionCircleVsAABB(Vector2f _circlePosition, FloatRect _circleBoundingBox, FloatRect _AABB)
{
	/// Check if the two AABB (Circle and AABB) are colliding
	if (CollisionAABBVsAABB(_AABB, _circleBoundingBox) == 0)
	{
		return 0;
	}

	//int projvertical = ProjectionSurSegment(_circlePosition.x, _circlePosition.y, _AABB.left, _AABB.top, _AABB.left, _AABB.top + _AABB.height);

	//if (projvertical == 1)
	//{
	//	return 1;   // cas E
	//}

	return 1;  // cas B   
}

int ProjectionSurSegment(float Cx, float Cy, float Ax, float Ay, float Bx, float By)
{
	float ACx = Cx - Ax;
	float ACy = Cy - Ay;
	float ABx = Bx - Ax;
	float ABy = By - Ay;
	float BCx = Cx - Bx;
	float BCy = Cy - By;

	float s1 = (ACx*ABx) + (ACy*ABy);
	float s2 = (BCx*ABx) + (BCy*ABy);

	printf("Product between %.2f & %.2f = %.2f\n", s1, s2, s1 * s2);

	if (s1*s2 > 0)
	{
		return 0;
	}

	return 1;
}

Vector2f GetDirectionFromAngleDegrees(float _angleDegrees)
{
	return GetDirectionFromAngleRadians(DegToRad(_angleDegrees));
}

Vector2f GetDirectionFromAngleRadians(double _angleRadians)
{
	//printf("Angle = %.2f - Radians = %.2f\n", _angleDegree, angleRadians);
	Vector2f direction;

	direction.x = cos(_angleRadians);
	direction.y = sin(_angleRadians);

	//printf("Direction : %.2f, %.2f\n", direction.x, direction.y);
	return direction;
}

float GetAngleRadiansBetweenPoints(Vector2f _pointA, Vector2f _pointB)
{
	return atan2(_pointB.y - _pointA.y, _pointB.x - _pointA.x);
}

float GetAngleRadiansBetweenPointAndMouse(Vector2f _pointA, Vector2i _pointB)
{
	return atan2(_pointB.y - _pointA.y, _pointB.x - _pointA.x);
}

float GetAngleRadiansFromVector(Vector2f _vector)
{
	return atan2(_vector.y, _vector.x);
}

float GetAngleDegreeFromVector(Vector2f _vector)
{
	float angleRadians = atan2(_vector.y, _vector.x);

	return RadToDeg(angleRadians);
}

Vector2f getPosOnCircleFromAngleDegrees(float _angle, float _circleRadius, Vector2f _circleCenterPos)
{
	Vector2f destPos = { 0,0 };
	destPos.x = _circleRadius * cos(DegToRad(_angle)) + _circleCenterPos.x;
	destPos.y = _circleRadius * sin(DegToRad(_angle)) + _circleCenterPos.y;

	return destPos;
}

double DegToRad(float _degrees)
{
	return (_degrees * PI) / 180;
}

float RadToDeg(double _radians)
{
	return _radians * (180 / PI);
}

//////////////////////////////////////
/// Normalize a vector 2 floats
/// \_vector : The vector to normalize
/// return a normalized vector
/////////////////////////////////////
Vector2f Normalize(Vector2f _vector)
{
	double length = Magnitude(_vector);
	//printf("Magnitude = %f\n", length);

	Vector2f newVector;
	newVector.x = _vector.x / length;
	newVector.y = _vector.y / length;
	//printf("Vector normalized, x = %f, y = %f\n", newVector.x, newVector.y);

	return newVector;
}

//////////////////////////////////////
/// Get the length of a vector
/// \_vector : The vector to get the length from
/// return the length
/////////////////////////////////////
float Magnitude(Vector2f _vector)
{
	return sqrt(_vector.x*_vector.x + _vector.y*_vector.y);
}

//////////////////////////////////////
/// Clamp a vector with a max length
/// \_vector : The vector to clamp
/// \_max : the max length
/// return the truncated vector
/////////////////////////////////////
Vector2f Truncate(Vector2f _vector, float _max)
{
	if (Magnitude(_vector) > _max)
	{
		_vector = Normalize(_vector);

		_vector.x *= _max;
		_vector.y *= _max;
	}
	return _vector;
}

