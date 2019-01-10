#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

using namespace std;
using namespace sf;


int IsOverRect(RenderWindow *_window, RectangleShape *_rectangle);

int CollisionAABBVsAABB(FloatRect _intRect1, FloatRect _intRect2);

int CollisionCircleVsAABB(Vector2f _circlePosition, FloatRect _circleBoundingBox, FloatRect _AABB);

Vector2f GetDirectionFromAngleDegrees(float _angleDegrees);
Vector2f GetDirectionFromAngleRadians(double _angleRadians);

float GetAngleRadiansBetweenPoints(Vector2f _pointA, Vector2f _pointB);
float GetAngleRadiansBetweenPointAndMouse(Vector2f _pointA, Vector2i _pointB);
float GetAngleRadiansFromVector(Vector2f _vector);
float GetAngleDegreeFromVector(Vector2f _vector);

Vector2f getPosOnCircleFromAngleDegrees(float _angle, float _circleRadius , Vector2f _circleCenterPos);

double DegToRad(float _degrees);
float RadToDeg(double _radians);
Vector2f Normalize(Vector2f _vector);
float Magnitude(Vector2f _vector);
Vector2f Truncate(Vector2f _vector, float _max);

#pragma once
