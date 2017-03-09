
#include "TestAI.h"
#include "Utility.h"
#include "PathManager.h"

#include "Player.h"

#include "ChaseEntityState.h"
#include "FollowPathState.h"

#include "DistanceQuery.h"
#include "RayCastQuery.h"
#include "CompositeQuery.h"
#include "ObjectAngleQuery.h"

#include <gl/GL.h>

TextureAssetResponse(BuildAI);
void BuildAI (GameWorld & world, const PropertyStore & properties, const LevelLoader::LevelAssetCollection & assets)
{
	std::string assetName = GetFileName(properties.GetProperty<std::string>("AssetName"));
	if (assetName == "policeguard")
	{
		TestAI * newAi = new TestAI();
		newAi->Position = properties.GetProperty<sf::Vector2f>("Position");
		world.AddEntity(newAi);

		std::string pathName = properties.GetProperty<std::string>("custompath");
		std::shared_ptr<PropertyStore> pathProperties;
		pathProperties = assets.find(pathName)->second;

		newAi->WanderPath = pathProperties->GetProperty<PathManager>("GlobalPath");
		newAi->WanderPath.AdvanceActiveNodeToNode(newAi->WanderPath.ClosestNodeToPoint(newAi->Position));

		AI::FollowPathState * followState = new AI::FollowPathState();
		AI::ChaseEntityState * chaseState = new AI::ChaseEntityState();

		AI::ExceedDistanceQuery * chaseExceedQuery = new AI::ExceedDistanceQuery();
		AI::RayCastQuery * chaseCastQuery = new AI::RayCastQuery();
		AI::CompositeQuery * chaseCompositeQuery = new AI::CompositeQuery(AI::CompositeQuery::AnyTrue);

		AI::WithinDistanceQuery * followWithinQuery = new AI::WithinDistanceQuery();
		AI::RayCastQuery * followRaycastQuery = new AI::RayCastQuery();
		AI::ObjectAngleQuery * followAngleQuery = new AI::ObjectAngleQuery();
		AI::CompositeQuery * followCompositeQuery = new AI::CompositeQuery(AI::CompositeQuery::AllTrue);


		/////////////////////////////////////////
		chaseExceedQuery->ReferenceDistance = 1000.0F;
		chaseExceedQuery->First = newAi;
		chaseExceedQuery->Second = Player::Instance();

		chaseCastQuery->Source = newAi;
		chaseCastQuery->Target = Player::Instance();

		chaseCompositeQuery->Queries.push_back(chaseExceedQuery);
		chaseCompositeQuery->Queries.push_back(chaseCastQuery);


		/////////////////////////////////////////
		followWithinQuery->ReferenceDistance = 500.0F;
		followWithinQuery->First = newAi;
		followWithinQuery->Second = Player::Instance();

		followRaycastQuery->Source = newAi;
		followRaycastQuery->Target = Player::Instance();
		followRaycastQuery->Negate = true;
		
		followAngleQuery->MaxAngleRadians = DegreesToRadians(80.0F);
		followAngleQuery->Source = newAi;
		followAngleQuery->Target = Player::Instance();

		followCompositeQuery->Queries.push_back(followWithinQuery);
		followCompositeQuery->Queries.push_back(followRaycastQuery);
		followCompositeQuery->Queries.push_back(followAngleQuery);

		followState->AvailableLogic.push_back(State::LogicBlock(followCompositeQuery, chaseState));

		chaseState->AvailableLogic.push_back(State::LogicBlock(chaseCompositeQuery, followState));
		chaseState->Goal = Player::Instance();

		newAi->StateMachine.SetNewActiveState(followState);
	}
}

TestAI::TestAI( )
{
	this->SetSpriteFromFileWithMask("resources/policeguard.png");

	m_MoveSpeed = 400.0F;

	MaxWalkSpeed = 100.0F;
	MaxRunSpeed = 600.0F;

	SetSolid(false);

	StateMachine.Properties.SetProperty<PathManager *>("Path", &WanderPath);
	StateMachine.Properties.SetProperty<float>("MoveSpeed", m_MoveSpeed);
}

TestAI::~TestAI( )
{
}

void TestAI::Draw(sf::RenderWindow & targetWindow)
{
	//WanderPath.DebugDraw();
	KinematicEntity::Draw(targetWindow);

	//glDisable(GL_TEXTURE_2D);
	//glBegin (GL_LINES);

	//if (m_OwnerWorld->RaycastIntersects(
	//						Position,
	//						Vector::Normal(Player::Instance()->Position - Position),
	//						Vector::Magnitude(Player::Instance()->Position - Position)
	//						))
	//	glColor4f (1.0F, 0.0F, 0.0F, 0.5F);
	//else
	//	glColor4f (0.2F, 0.2F, 0.2F, 0.5F);

	//glVertex2f(Position.x, Position.y);
	//glVertex2f(Player::Instance()->Position.x, Player::Instance()->Position.y);

	//glEnd();
}

void TestAI::Update( float timeDelta )
{
	StateMachine.ApplyToObject(this);
	Base::Update(timeDelta);

	if (Vector::Magnitude(Velocity) > 0.0F)
		Sprite.SetRotation(-RadiansToDegrees(Vector::Angle(Velocity)));
}
