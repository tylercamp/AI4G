
#pragma once

#include "StateMachine.h"
#include "KinematicEntity.h"
#include "LevelLoader.h"
#include "PathManager.h"

class TestAI : public KinematicEntity
{
public:
	
			TestAI( );
	virtual ~TestAI( );

	void	Update(float timeDelta);
	void	Draw(sf::RenderWindow & targetWindow);

	PathManager WanderPath;

	StateMachine StateMachine;

private:
	typedef KinematicEntity Base;

	float	m_MoveSpeed;
};