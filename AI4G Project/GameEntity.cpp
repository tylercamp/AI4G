
#pragma once

#include "GameEntity.h"
#include "World.h"
#include "Debug.h"


/*
=================
GameEntity::GameEntity
=================
 */
GameEntity::GameEntity ()
{
	m_OwnerWorld = NULL;
	m_CollisionMask = NULL;
	m_Solid = true;

	Depth = 0;
	m_SubDepth = rand() * rand() * rand () * rand ();
}

/*
=================
GameEntity::~GameEntity
=================
 */
GameEntity::~GameEntity ()
{
	if (m_OwnerWorld != NULL)
		Debug::Error(
		"Attempting to delete entity that has not been detached from its GameWorld."
		"Game components that are attached to a GameWorld cannot be destroyed by anything except the GameWorld owning it."
		);
}

bool GameEntity::operator<(const GameEntity & o) const
{
	if (Depth < o.Depth)
		return true;
	if (Depth > o.Depth)
		return false;

	if (m_SubDepth < o.m_SubDepth)
		return true;
	if (m_SubDepth > o.m_SubDepth)
		return false;

	return false;
}

bool GameEntity::operator>(const GameEntity & o) const
{
	return !(*this < o) && (*this) != o;
}

bool GameEntity::operator==(const GameEntity & o) const
{
	return Depth == o.Depth && m_SubDepth == o.m_SubDepth;
}

bool GameEntity::operator!=(const GameEntity & o) const
{
	return !(*this == o);
}

/*
=================
GameEntity::Destroy
=================
 */
void GameEntity::Destroy ()
{
	if (m_OwnerWorld == NULL)
		Debug::Error(
		"Illegal invokation of Destroy() upon an entity that was not attached to a GameWorld."
		);

	m_OwnerWorld->DestroyEntity(this);
}

/*
=================
GameEntity::Update
=================
 */
void GameEntity::Update (float timeDelta)
{
}

/*
=================
GameEntity::Draw
=================
 */
void GameEntity::Draw (sf::RenderWindow & targetWindow)
{
	DrawSelf (targetWindow);
}

/*
=================
GameEntity::DrawSelf
=================
 */
void GameEntity::DrawSelf (sf::RenderWindow & targetWindow)
{
	Sprite.SetPosition (Position);
	targetWindow.Draw (Sprite);
}

/*
=================
GameEntity::SetSpriteFromFile
=================
 */
void GameEntity::SetSpriteFromFile (const std::string & fileName)
{
	sf::Image * image;
	ResourceManager::Context->GetResource (fileName, image);
	Sprite.SetImage (*image);
	Sprite.SetCenter (Sprite.GetSize () / 2.0f);
}

/*
=================
GameEntity::SetSpriteFromFileWithMask
=================
 */
void GameEntity::SetSpriteFromFileWithMask (const std::string & fileName)
{
	SetSpriteFromFile(fileName);

	MaskAABB * newMask = new MaskAABB();
	newMask->Position = &Position;
	newMask->Size = Sprite.GetSize();
	newMask->Origin = Sprite.GetCenter();

	m_CollisionMask = newMask;
}

/*
=================
GameEntity::SetSpriteFromFileWithMask
=================
 */
void GameEntity::SetSpriteFromFileWithCircularMask(const std::string & fileName, float radius)
{
	SetSpriteFromFile(fileName);

	MaskCircle * newMask = new MaskCircle();
	newMask->Position = &Position;
	newMask->Radius = (radius >= 0.0F ? radius : (Sprite.GetSize().x + Sprite.GetSize().y) / 2.0F);

	m_CollisionMask = newMask;
}

/*
=================
GameEntity::IsSolid
=================
 */
bool GameEntity::IsSolid() const
{
	return m_Solid;
}

/*
=================
GameEntity::SetSolid
=================
 */
void GameEntity::SetSolid(bool isSolid) const
{
	m_Solid = isSolid;
}

/*
=================
GameEntity::GetCollisionMask

Gets the collision mask for the current entity; by default, entities do not
have a collision mask and this function returns NULL. Derived classes can
override this method to give a valid CollisionMask.
=================
 */
CollisionMask * GameEntity::GetCollisionMask() const
{
	return m_CollisionMask;
}

GameWorld * GameEntity::GetOwnerWorld() const
{
	return m_OwnerWorld;
}