
#include "Utility.h"

#include <Windows.h>

std::string TranslatePathToAbsolute(const std::string & path)
{
	char programPath[MAX_PATH];
	GetModuleFileNameA(NULL, programPath, sizeof(programPath));

	std::string programDirectory = programPath;
	programDirectory = programDirectory.substr(0, programDirectory.find_last_of("\\"));

	return programDirectory + "\\" + path;
}

std::string GetFileName(const std::string & path)
{
	int fslashSearch = path.find_last_of('/'), bslashSearch = path.find_last_of('\\');
	return path.substr(max(fslashSearch, bslashSearch) + 1);
}

/*
=================
DegreesToRadians
=================
*/
float DegreesToRadians(float degrees)
{
	return degrees * M_PI / 180.0F;
}

/*
=================
RadiansToDegrees
=================
*/
float RadiansToDegrees(float radians)
{
	return radians * 180.0F / M_PI;
}

/*
=================
DistanceBetweenEntities
=================
*/
float DistanceBetweenEntities (const GameEntity & c1, const GameEntity & c2)
{
	return sqrtf (sqr(c1.Position.x - c2.Position.x) + sqr(c1.Position.y - c2.Position.y));
}

/*
=================
RandomNormal

Returns a random number from 0 through 1
=================
*/
float RandomNormal ()
{
	return abs ((float)(rand ()) / (float)(RAND_MAX));
}

/*
=================
RandomRange
=================
*/
float RandomRange (float min, float max)
{
	return RandomNormal () * (max - min) + min;
}

/*
=================
NearestPointOnLine

TODO: Implement
=================
 */
sf::Vector2f NearestPointOnLine(sf::Vector2f lineStartPoint, sf::Vector2f lineDirectionNormal, sf::Vector2f referencePoint)
{
	throw;
}

/*
=================
LinesIntersectionPoint

TODO: Implement
=================
 */
sf::Vector2f LinesIntersectionPoint(sf::Vector2f line1StartPoint, sf::Vector2f line1DirectionNormal, sf::Vector2f line2StartPoint, sf::Vector2f line2DirectionNormal)
{
	throw;
}

/*
=================
LinesIntersect

TODO: Implement
=================
 */
bool LinesIntersect(sf::Vector2f line1Start, sf::Vector2f line1End, sf::Vector2f line2Start, sf::Vector2f line2End)
{
	throw;
}

/*
=================
Vector::Magnitude
=================
*/
float Vector::Magnitude (const sf::Vector2f & vector)
{
	return sqrtf (sqr(vector.x) + sqr(vector.y));
}

/*
=================
Vector::Magnitude
=================
*/
float Vector::Magnitude (const sf::Vector3f & vector)
{
	return sqrtf (sqr(vector.x) + sqr(vector.y) + sqr(vector.z));
}

/*
=================
Vector::MagnitudeSquared
=================
*/
float Vector::MagnitudeSquared (const sf::Vector2f & vector)
{
	return sqr(vector.x) + sqr(vector.y);
}

float Vector::MagnitudeSquared (const sf::Vector3f & vector)
{
	return sqr(vector.x) + sqr(vector.y) + sqr(vector.z);
}

/*
=================
Vector::Angle
=================
*/
float Vector::Angle (const sf::Vector2f & vector)
{
	return atan2f (vector.y, vector.x);
}

/*
=================
Vector::Dot
=================
*/
float Vector::Dot (const sf::Vector2f & a, const sf::Vector2f & b)
{
	return a.x * b.x + a.y * b.y;
}

/*
=================
Vector::Normalize
=================
*/
void Vector::Normalize (sf::Vector2f & vector)
{
	float mag = Magnitude (vector);
	if (mag == 0.0F) return;
	vector.x /= mag;
	vector.y /= mag;
}

/*
=================
Vector::Normal
=================
*/
sf::Vector2f Vector::Normal(const sf::Vector2f & vector)
{
	float magnitude = Magnitude (vector);
	if (magnitude == 1.0F) return vector;
	if (magnitude < 0.001F) return sf::Vector2f(1.0F, 0.0F);
	return sf::Vector2f (vector.x / magnitude, vector.y / magnitude);
}

/*
=================
Vector::Reverse
=================
 */
sf::Vector2f Vector::Reverse(const sf::Vector2f & vector)
{
	return sf::Vector2f (-vector.x, -vector.y);
}

/*
=================
Vector::Min
=================
*/
sf::Vector2f Vector::Min(const sf::Vector2f & a, const sf::Vector2f & b)
{
	return sf::Vector2f(min(a.x, b.x), min(a.y, b.y));
}

/*
=================
Vector::Max
=================
*/
sf::Vector2f Vector::Max(const sf::Vector2f & a, const sf::Vector2f & b)
{
	return sf::Vector2f(max(a.x, b.x), max(a.y, b.y));
}

/*
=================
Vector::SafeDivide
=================
*/
sf::Vector2f Vector::SafeDivide(const sf::Vector2f & a, const sf::Vector2f & b)
{
	return sf::Vector2f (a.x / b.x, a.y / b.y);
}