
#include "KinematicEntity.h"
#include "World.h"
#include "Debug.h"
#include "Utility.h"

/*
=================
KinematicEntity::KinematicEntity
=================
 */
KinematicEntity::KinematicEntity()
{
	m_AccumulatedForce = sf::Vector2f(0.0F, 0.0F);

	Velocity = sf::Vector2f(0.0F, 0.0f);

	MaxWalkSpeed = 0.0F;
	MaxRunSpeed = 0.0F;
	IsRunning = false;

	FrictionFactor = 1200.0F;

	m_NoClip = false;
}

/*
=================
KinematicEntity::Update
=================
*/
void KinematicEntity::Update(float timeDelta)
{
	Base::Update(timeDelta);

	if (m_AccumulatedForce.x == 0.0F)
	{
		float prevVelocity = Velocity.x;
		Velocity.x -= FrictionFactor * timeDelta * sign (Velocity.x);
		if (sign(prevVelocity) != sign(Velocity.x))
			Velocity.x = 0.0F;
	}
	else
	{
		Velocity.x += m_AccumulatedForce.x * timeDelta;

		if (sign(m_AccumulatedForce.x) != sign(Velocity.x))
		{
			Velocity.x += FrictionFactor * timeDelta * sign(m_AccumulatedForce.x);

			if (sign(m_AccumulatedForce.x) == sign(Velocity.x))
				Velocity.x = 0.0F;
		}
	}

	if (m_AccumulatedForce.y == 0.0F)
	{
		float prevVelocity = Velocity.y;
		Velocity.y -= FrictionFactor * timeDelta * sign (Velocity.y);
		if (sign(prevVelocity) != sign(Velocity.y))
			Velocity.y = 0.0F;
	}
	else
	{
		Velocity.y += m_AccumulatedForce.y * timeDelta;

		if (sign(m_AccumulatedForce.y) != sign(Velocity.y))
		{
			Velocity.y += FrictionFactor * timeDelta * sign(m_AccumulatedForce.y);

			if (sign(m_AccumulatedForce.y) == sign(Velocity.y))
				Velocity.y = 0.0F;
		}
	}

	m_AccumulatedForce = sf::Vector2f (0.0F, 0.0F);

	if (fabs(Velocity.x) < 0.01F)
		Velocity.x = 0.0F;
	if (fabs(Velocity.y) < 0.01F)
		Velocity.y = 0.0F;

	sf::Vector2f originalPosition = Position;
	sf::Vector2f localVelocity = Velocity * timeDelta;

	if (NULL == m_OwnerWorld->EntityPlaceFree(*this, sf::Vector2f(originalPosition.x + localVelocity.x, originalPosition.y)) || m_NoClip)
	{
		Position.x += localVelocity.x;
	}
	else
	{
		Velocity.x = 0.0F;
	}

	if (NULL == m_OwnerWorld->EntityPlaceFree(*this, sf::Vector2f(originalPosition.x, originalPosition.y + localVelocity.y)) || m_NoClip)
	{
		Position.y += localVelocity.y;
	}
	else
	{
		Velocity.y = 0.0F;
	}

	if (IsRunning && MaxRunSpeed > 0.0F)
	{
		float speed = Vector::Magnitude(Velocity);
		if (speed > MaxRunSpeed)
		{
			Vector::Normalize(Velocity);
			Velocity *= MaxRunSpeed;
		}
	}

	if (!IsRunning && MaxWalkSpeed > 0.0F)
	{
		float speed = Vector::Magnitude(Velocity);
		if (speed > MaxWalkSpeed)
		{
			Vector::Normalize(Velocity);
			Velocity *= MaxWalkSpeed;
		}
	}

	//Debug::Printf("Velocity: (%f, %f)", Velocity.x, Velocity.y);
}

/*
=================
KinematicEntity::ApplyForce( Vector2f )
=================
 */
void KinematicEntity::ApplyForce(const sf::Vector2f & force)
{
	m_AccumulatedForce += force;
}

/*
=================
KinematicEntity::ApplyForce( float, float )
=================
 */
void KinematicEntity::ApplyForce(float x, float y)
{
	m_AccumulatedForce.x += x;
	m_AccumulatedForce.y += y;
}