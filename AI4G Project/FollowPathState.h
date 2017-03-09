
#pragma once

#include "State.h"
#include "PathManager.h"
#include "PropertyStore.h"
#include "Utility.h"
#include "KinematicEntity.h"

namespace AI
{
	class FollowPathState : public State
	{
	public:
		void Initialize( KinematicEntity * target, PropertyStore * dataSource )
		{
			PathManager * wanderPath = dataSource->GetProperty<PathManager *>("Path");
			sf::Vector2f closestNode = wanderPath->ClosestNodeToPoint(target->Position);
			wanderPath->AdvanceActiveNodeToNode(closestNode);
		}

		State * Apply( KinematicEntity * target, PropertyStore * dataSource )
		{
			PathManager * wanderPath = dataSource->GetProperty<PathManager *>("Path");
			float moveSpeed = dataSource->GetProperty<float>("MoveSpeed");

			sf::Vector2f targetNode = wanderPath->CurrentNode();

			target->ApplyForce(Vector::Normal(targetNode - target->Position) * moveSpeed);

			if (Vector::Magnitude(targetNode - target->Position) < 20.0F)
				wanderPath->AdvanceActiveNode();

			return nullptr;
		}
	};
}