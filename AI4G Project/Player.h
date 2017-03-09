
#pragma once

#include "KinematicEntity.h"

#include "LevelLoader.h"

class Player : public KinematicEntity
{

	float					m_MoveSpeed;
	
	typedef KinematicEntity	Base;

public:

	static Player *			Instance();

							Player ();

	void					Update (float timeDelta);
	void					Draw( sf::RenderWindow & target );
};