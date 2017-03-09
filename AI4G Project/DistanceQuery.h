
#pragma once

#include "WorldQuery.h"
#include "GameEntity.h"
#include "Utility.h"

namespace AI
{
	class ExceedDistanceQuery : public WorldQuery
	{
	public:
		float ReferenceDistance;
		GameEntity * First, * Second;

	protected:

		bool Evaluate () const
		{
			return Vector::Magnitude(First->Position - Second->Position) > ReferenceDistance;
		}
	};

	class WithinDistanceQuery : public WorldQuery
	{
	public:
		float ReferenceDistance;
		GameEntity * First, * Second;

	protected:
		bool Evaluate () const
		{
			return Vector::Magnitude(First->Position - Second->Position) < ReferenceDistance;
		}
	};
}