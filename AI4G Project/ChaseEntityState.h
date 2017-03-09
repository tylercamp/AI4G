
#pragma once

#include "State.h"
#include "PropertyStore.h"
#include "KinematicEntity.h"
#include "GameEntity.h"
#include "Utility.h"
#include "WorldQuery.h"

namespace AI
{
	class ChaseEntityState : public State
	{
	public:
		GameEntity * Goal;

		ChaseEntityState()
		{
			Goal = nullptr;
		}

		void Initialize( KinematicEntity * target, PropertyStore * dataSource )
		{
		}

		State * Apply( KinematicEntity * target, PropertyStore * dataSource )
		{
			target->ApplyForce(Vector::Normal(Goal->Position - target->Position) * 200.0F);

			return nullptr;
		}
	};
}