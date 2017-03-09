
#pragma once

#include <SFML\Graphics.hpp>
#include <list>
#include <vector>

#include "ResourceManager.h"
#include "Collision.h"

/*
 * GameEntity provides basic utilities for game objects. It comes with shared mechanisms for drawing
 *	and updating, and also maintains its own grouping of GameEntity objects so that you can add and
 *	remove components on the fly.
 */

struct BoundingCircle
{
	float	X;
	float	Y;
	float	Radius;
};

typedef sf::Rect <float> BoundingBox;



class GameEntity;
class GameWorld;

typedef std::list <GameEntity *>	ComponentList;
typedef std::vector <GameEntity *>	AtomicComponentList;

typedef ComponentList::iterator ComponentReference;



class GameEntity
{
public:
	sf::Sprite					Sprite;
	sf::Vector2f				Position;
	int							Depth;

								GameEntity ();
	virtual						~GameEntity ();

	/* Whether or not two entities are the same exact object. */
	bool						operator!=(const GameEntity & ) const;
	bool						operator==(const GameEntity & ) const;

	/* Depth-based comparisons. */
	bool						operator<(const GameEntity & ) const;
	bool						operator>(const GameEntity & ) const;

	virtual void				Destroy ();
	virtual void				Update (float timeDelta);
	virtual void				Draw (sf::RenderWindow & targetWindow);

	void						DrawSelf (sf::RenderWindow & targetWindow);

	void						SetSpriteFromFile (const std::string & fileName);
	void						SetSpriteFromFileWithMask (const std::string & fileName);
	void						SetSpriteFromFileWithCircularMask (const std::string & fileName, float radius = -1.0F);

	bool						IsSolid () const;
	void						SetSolid (bool isSolid) const;

	virtual CollisionMask *		GetCollisionMask () const;

	GameWorld *					GetOwnerWorld() const;

protected:

	mutable bool				m_Solid;

	ComponentReference			m_ComponentLocation;
	GameWorld *					m_OwnerWorld;
	
	CollisionMask *				m_CollisionMask;

	int							m_SubDepth;

	friend class GameWorld;
};