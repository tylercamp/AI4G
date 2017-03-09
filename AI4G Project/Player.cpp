

#include "Player.h"
#include "Utility.h"
#include "StateMachine.h"
#include "World.h"

TextureAssetResponse(BuildPlayer);

void BuildPlayer(
	GameWorld & target,
	const PropertyStore & properties,
	const LevelLoader::LevelAssetCollection & assets
	)
{
	std::string assetName = GetFileName(properties.GetProperty<std::string>("assetname"));
	if (assetName == "ninjer")
	{
		Player * player = Player::Instance();
		player->Position = properties.GetProperty<sf::Vector2f>("Position");
		target.AddEntity(player);
	}
}

Player * Player::Instance()
{
	static Player * instance = nullptr;
	if (instance == nullptr)
		instance = new Player();
	return instance;
}

/*
=================
Player::Player
=================
*/
Player::Player ()
{
	Position = sf::Vector2f( 300.0f, 300.0f );

	SetSpriteFromFileWithMask("resources/ninjer.png" /*, 25.0F*/);

	//Sprite.SetScale(0.2F, 0.2F);

	m_MoveSpeed = 600.0F;

	MaxWalkSpeed = 150.0F;
	MaxRunSpeed = 300.0F;

	SetSolid(false);

	Depth = -500;

	IsRunning = true;
}

/*
=================
Player::Update

Handles input-based movement and collision detection.
=================
*/
void Player::Update (float timeDelta)
{
	const sf::Input & input = m_OwnerWorld->GetInput();

	sf::Vector2f moveVector (0.0f, 0.0f);

	if (input.IsKeyDown (sf::Key::Left) || input.IsKeyDown (sf::Key::A))
	{
		moveVector.x -= m_MoveSpeed * timeDelta;
	}

	if (input.IsKeyDown (sf::Key::Right) || input.IsKeyDown (sf::Key::D))
	{
		moveVector.x += m_MoveSpeed * timeDelta;
	}

	if (input.IsKeyDown (sf::Key::Up) || input.IsKeyDown (sf::Key::W))
	{
		moveVector.y -= m_MoveSpeed * timeDelta;
	}

	if (input.IsKeyDown (sf::Key::Down) || input.IsKeyDown (sf::Key::S))
	{
		moveVector.y += m_MoveSpeed * timeDelta;
	}

	if (moveVector != sf::Vector2f (0.0f, 0.0f))
	{
		Vector::Normalize (moveVector);
		ApplyForce(moveVector * 800.0F);
	}

	if (Vector::MagnitudeSquared(Velocity) > 1.0F)
		Sprite.SetRotation(-RadiansToDegrees(Vector::Angle(Velocity)));

	//Debug::Printf("Player Position: %f, %f", Position.x, Position.y);

	Base::Update(timeDelta);
}

/*
=================
Player::Draw

Renders the player + debug info
=================
 */
void Player::Draw( sf::RenderWindow & target )
{
// 	float x, y;
// 	x = target.GetInput().GetMouseX();
// 	y = target.GetInput().GetMouseY();
// 
// 	NodePath * path = m_OwnerWorld->GetPathToTarget(Position, sf::Vector2f(x, y));
// 	path->Debug_Draw(target);
// 	delete path;

	Base::Draw( target );
}