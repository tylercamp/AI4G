
#pragma once

#include "GameEntity.h"

class KinematicEntity : public GameEntity
{
public:

								KinematicEntity();

	sf::Vector2f				Velocity;

	void						Update (float timeDelta);

	void						ApplyForce (const sf::Vector2f & force);
	void						ApplyForce (float x, float y);

	bool						IsRunning;
	float						FrictionFactor;

	float						MaxWalkSpeed;
	float						MaxRunSpeed;

protected:
	bool						m_NoClip;

private:
	typedef GameEntity			Base;

	sf::Vector2f				m_AccumulatedForce;
};