


#include <typeinfo.h>

#include "Collision.h"
#include "Utility.h"
#include "Debug.h"



bool CircleIntersectsAABB(const MaskCircle & circle, const MaskAABB & aabb)
{
	sf::Vector2f closestPointOnAABB = *circle.Position;

	if (closestPointOnAABB.x < aabb.GetLeft())
		closestPointOnAABB.x = aabb.GetLeft();
	if (closestPointOnAABB.x > aabb.GetRight())
		closestPointOnAABB.x = aabb.GetRight();
	if (closestPointOnAABB.y < aabb.GetTop())
		closestPointOnAABB.y = aabb.GetTop();
	if (closestPointOnAABB.y > aabb.GetBottom())
		closestPointOnAABB.y = aabb.GetBottom();

	return Vector::MagnitudeSquared(closestPointOnAABB - *circle.Position) <= sqr(circle.Radius);
}

/*
=================
CollisionMask::CollisionMask
=================
 */
CollisionMask::CollisionMask( )
{
	m_ID = rand () * rand ();
}

/*
=================
CollisionMask::Intersects( CollisionMask )
=================
*/
bool CollisionMask::Intersects( CollisionMask * mask )
{
	const type_info & maskType = typeid( *mask );

	if (maskType == typeid(MaskAABB))
		return Intersects((MaskAABB *)mask);

	if (maskType == typeid(MaskCircle))
		return Intersects((MaskCircle *)mask);

	//	Should never reach this point
	throw;
}

/*
=================
CollisionMask::Project( CollisionMask )
=================
*/
sf::Vector2f CollisionMask::Project( CollisionMask * mask, const sf::Vector2f & velocity )
{
	const type_info & maskType = typeid( *mask );

	if (maskType == typeid(MaskAABB))
		return Project((MaskAABB *)mask, velocity);

	if (maskType == typeid(MaskCircle))
		return Project((MaskCircle *)mask, velocity);

	//	Should never reach this point
	throw;
}

/*
=================
CollisionMask::GetID
=================
 */
unsigned int CollisionMask::GetID( )
{
	return m_ID;
}

/*
=================
MaskAABB::GetLeft
=================
*/
float MaskAABB::GetLeft() const
{
	return Position->x - Origin.x;
}

/*
=================
MaskAABB::GetRight
=================
*/
float MaskAABB::GetRight() const
{
	return Position->x - Origin.x + Size.x;
}

/*
=================
MaskAABB::GetTop
=================
*/
float MaskAABB::GetTop() const
{
	return Position->y - Origin.y;
}

/*
=================
MaskAABB::GetBottom
=================
*/
float MaskAABB::GetBottom() const
{
	return Position->y - Origin.y + Size.y;
}

/*
=================
MaskAABB::Intersects( Vector2f )
=================
 */
bool MaskAABB::Intersects( const sf::Vector2f & position )
{
	return	position.x >= GetLeft()		&&
			position.x <= GetRight()	&&
			position.y <= GetBottom()	&&
			position.y >= GetTop();
}

/*
=================
MaskAABB::Intersects( MaskAABB )
=================
*/
bool MaskAABB::Intersects( MaskAABB * mask )
{
	if (mask == NULL)
		return false;

	return
		GetLeft()   <= mask->GetRight()  &&
		GetRight()  >= mask->GetLeft()   &&
		GetTop()    <= mask->GetBottom() &&
		GetBottom() >= mask->GetTop();
}

/*
=================
MaskAABB::Intersects( MaskCircle )
=================
*/
bool MaskAABB::Intersects( MaskCircle * mask )
{
	return CircleIntersectsAABB( *mask, *this );
}

/*
=================
MaskAABB::Project( MaskAABB )
=================
*/
sf::Vector2f MaskAABB::Project( MaskAABB * mask, const sf::Vector2f & velocity )
{
	//TODO: Implement
	return *Position + velocity;
}

/*
=================
MaskAABB::Project( MaskCircle )
=================
*/
sf::Vector2f MaskAABB::Project( MaskCircle * mask, const sf::Vector2f & velocity )
{
	//TODO: Implement
	return *Position + velocity;
}

/*
=================
MaskAABB::RaycastIntersects
=================
 */
bool MaskAABB::RaycastIntersects( const sf::Vector2f & position, const sf::Vector2f & directionNormal ) const
{
	/* Credit where credit is due:
	 * 
	 *	http://www.gamedev.net/page/resources/_/technical/math-and-physics/intersection-math-algorithms-learn-to-derive-r3033
	 *
	 */

	sf::Vector2f tmin = Vector::SafeDivide((sf::Vector2f(GetLeft(), GetTop()) - position), directionNormal);
	sf::Vector2f tmax = Vector::SafeDivide((sf::Vector2f(GetRight(), GetBottom()) - position), directionNormal);

	sf::Vector2f tnear = Vector::Min(tmin, tmax);
	sf::Vector2f tfar = Vector::Max(tmin, tmax);

	float enter, exit;
	enter = max(max(tnear.x, 0.0f), tnear.y);
	exit = min(tfar.x, tfar.y);

	return (exit > 0.0f && enter < exit);
}

/*
=================
MaskCircle::Intersects( Vector2f )
=================
 */
bool MaskCircle::Intersects( const sf::Vector2f & position )
{
	return Vector::MagnitudeSquared(position - *Position) <= sqr(Radius);
}

/*
=================
MaskCircle::Intersects( MaskAABB )
=================
*/
bool MaskCircle::Intersects( MaskAABB * mask )
{
	return CircleIntersectsAABB(*this, *mask );
}

/*
=================
MaskCircle::Intersects( MaskCircle )
=================
*/
bool MaskCircle::Intersects( MaskCircle * mask )
{
	return Vector::MagnitudeSquared(*Position - *mask->Position) <= sqr (Radius + mask->Radius);
}

/*
=================
MaskCircle::Project( MaskAABB )
=================
*/
sf::Vector2f MaskCircle::Project( MaskAABB * mask, const sf::Vector2f & velocity )
{
	//TODO: Implement
	return *Position + velocity;
}

/*
=================
MaskCircle::Project( MaskCircle )
=================
*/
sf::Vector2f MaskCircle::Project( MaskCircle * mask, const sf::Vector2f & velocity )
{
	//TODO: Implement
	return *Position + velocity;
}

/*
=================
MaskCircle::RaycastIntersects
=================
*/
bool MaskCircle::RaycastIntersects( const sf::Vector2f & position, const sf::Vector2f & directionNormal ) const
{
	sf::Vector2f nearestPoint = NearestPointOnLine(position, directionNormal, *Position);
	float mag = Vector::MagnitudeSquared(nearestPoint - *Position);
	return Vector::MagnitudeSquared(nearestPoint - *Position) < sqr (Radius);
}