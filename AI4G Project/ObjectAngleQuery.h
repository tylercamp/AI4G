
#pragma  once

#include "WorldQuery.h"
#include "GameEntity.h"
#include "Utility.h"

namespace AI
{
	class ObjectAngleQuery : public WorldQuery
	{
	public:
		float MaxAngleRadians;

		//	The angle being compared against is the Rotation of the Source's sprite.
		GameEntity * Source;
		GameEntity * Target;

	protected:

		bool Evaluate () const
		{
			sf::Vector2f vectorToTarget = Vector::Normal(Target->Position - Source->Position);

			float sourceAngleRadians = DegreesToRadians(Source->Sprite.GetRotation());
			sf::Vector2f sourceOrientation = sf::Vector2f(cosf(sourceAngleRadians), -sinf(sourceAngleRadians));

			float angle = acosf(Vector::Dot(vectorToTarget, sourceOrientation));
			bool result = angle <= MaxAngleRadians;

			Debug::Printf("ObjectAngleQuery: (%f) | %i", RadiansToDegrees(angle), (int)result);

			return result;
		}
	};
}