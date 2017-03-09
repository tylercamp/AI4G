
#pragma once

#include <SFML/Graphics.hpp>

class MaskAABB;
class MaskCircle;

class CollisionMask
{
public:

	sf::Vector2f * Position;

								CollisionMask( );

	virtual bool				Intersects( const sf::Vector2f & point ) = 0;
	virtual bool				Intersects( MaskAABB * mask ) = 0;
	virtual bool				Intersects( MaskCircle * mask ) = 0;
	bool						Intersects( CollisionMask * mask );


	virtual sf::Vector2f		Project( MaskAABB * mask, const sf::Vector2f & velocity ) = 0;
	virtual sf::Vector2f		Project( MaskCircle * mask, const sf::Vector2f & velocity ) = 0;
	sf::Vector2f				Project( CollisionMask * mask, const sf::Vector2f & velocity );

	virtual bool				RaycastIntersects( const sf::Vector2f & position, const sf::Vector2f & directionNormal ) const = 0;

	unsigned int				GetID ();

private:
	unsigned int				m_ID;
};


class MaskAABB : public CollisionMask
{
public:

	sf::Vector2f				Size;
	sf::Vector2f				Origin;

	float						GetLeft( ) const;
	float						GetRight( ) const;
	float						GetTop( ) const;
	float						GetBottom( ) const;

	bool						Intersects( const sf::Vector2f & point );
	bool						Intersects( MaskAABB * mask );
	bool						Intersects( MaskCircle * mask );

	sf::Vector2f				Project( MaskAABB * mask, const sf::Vector2f & velocity );
	sf::Vector2f				Project( MaskCircle * mask, const sf::Vector2f & velocity );

	bool						RaycastIntersects( const sf::Vector2f & position, const sf::Vector2f & directionNormal ) const;
};


class MaskCircle : public CollisionMask
{
public:

	float						Radius;

	bool						Intersects( const sf::Vector2f & point );
	bool						Intersects( MaskAABB * mask );
	bool						Intersects( MaskCircle * mask );

	sf::Vector2f				Project( MaskAABB * mask, const sf::Vector2f & velocity );
	sf::Vector2f				Project( MaskCircle * mask, const sf::Vector2f & velocity );

	bool						RaycastIntersects( const sf::Vector2f & position, const sf::Vector2f & directionNormal ) const;
};