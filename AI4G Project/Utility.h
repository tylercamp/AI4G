
#pragma once

#include <cmath>
#include <sstream>
#include <SFML/System/Vector2.hpp>
#include "GameEntity.h"



#ifndef sqr
# define sqr(x) ((x)*(x))
#endif

#ifdef min
# undef min
#endif

#ifdef max
# undef max
#endif

#define M_PI 3.1415926535F

inline bool StringContainsText( const std::string & string )
{
	std::istringstream stream (string);
	double realValue;
	stream >> realValue;
	return stream.fail();
}

template <typename T>
T StringToReal( const std::string & string )
{
	std::istringstream stream (string);
	T value;
	stream >> value;
	return value;
}

template <typename T>
std::string RealToString( const T & value )
{
	std::ostringstream stream;
	stream << value;
	return stream.str();
}


template <typename T>
inline T min(const T & a, const T & b)
{
	return a < b ? a : b;
}

template <typename T>
inline T max(const T & a, const T & b)
{
	return a > b ? a : b;
}



template <typename T>
inline int sign(T val)
{
	return (T(0) < val) - (val < T(0));
}


std::string				TranslatePathToAbsolute(const std::string & path);
std::string				GetFileName(const std::string & path);


float					DegreesToRadians(float degrees);
float					RadiansToDegrees(float radians);


float					DistanceBetweenEntities (const GameEntity & c1, const GameEntity & c2);
float					RandomNormal ();
float					RandomRange (float min, float max);

sf::Vector2f			NearestPointOnLine (sf::Vector2f lineStartPoint, sf::Vector2f lineDirectionNormal, sf::Vector2f referencePoint);
sf::Vector2f			LinesIntersectionPoint (sf::Vector2f line1StartPoint, sf::Vector2f line1DirectionNormal, sf::Vector2f line2StartPoint, sf::Vector2f line2DirectionNormal);
bool					LinesIntersect (sf::Vector2f line1Start, sf::Vector2f line1End, sf::Vector2f line2Start, sf::Vector2f line2End);

/*
 * SFML vector utilities
 */
namespace Vector
{
	float				Magnitude (const sf::Vector2f & vector);
	float				Magnitude (const sf::Vector3f & vector);
	float				MagnitudeSquared (const sf::Vector2f & vector);
	float				MagnitudeSquared (const sf::Vector3f & vector);
	float				Angle (const sf::Vector2f & vector);
	float				Dot (const sf::Vector2f & a, const sf::Vector2f & b);

	void				Normalize (sf::Vector2f & vector);
	sf::Vector2f		Normal (const sf::Vector2f & vector);

	sf::Vector2f		Reverse (const sf::Vector2f & vector);

	sf::Vector2f		Min(const sf::Vector2f & a, const sf::Vector2f & b);
	sf::Vector2f		Max(const sf::Vector2f & a, const sf::Vector2f & b);

	sf::Vector2f		SafeDivide(const sf::Vector2f & a, const sf::Vector2f & b);
}