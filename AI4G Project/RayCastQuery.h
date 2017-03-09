
#pragma once

#include "WorldQuery.h"
#include "State.h"
#include "GameEntity.h"
#include "Utility.h"

namespace AI
{
	class RayCastQuery : public WorldQuery
	{
	public:
		GameEntity * Source, * Target;

	protected:

		bool Evaluate () const
		{
			if (!Source || !Target)
				return false;

			//	We can only ray-cast if the two are within the same world
			if (Source->GetOwnerWorld() != Target->GetOwnerWorld())
				return false;

			sf::Vector2f normalToTarget = Vector::Normal(Target->Position - Source->Position);
			float distanceToTarget = Vector::Magnitude(Target->Position - Source->Position);

			return Source->GetOwnerWorld()->RaycastIntersects(Source->Position, normalToTarget, distanceToTarget);
		}
	};
}