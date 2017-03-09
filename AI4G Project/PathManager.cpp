

#include "PathManager.h"
#include "Utility.h"

PathManager::PathManager()
{
	m_CurrentPosition = 0;
}

/*
=================
PathManager::GenerateRandomPath
=================
 */
void PathManager::GenerateRandomPath( PathManager & target, uint nodeCount, const sf::Vector2f & minPosition, const sf::Vector2f & maxPosition, float minDistance )
{
	//TODO: Make the minDistance parameter actually do something

	target.Path.clear();

	for (uint i = 0; i < nodeCount; i++)
	{
		sf::Vector2f currentPosition;

		currentPosition.x = RandomRange(minPosition.x, maxPosition.x);
		currentPosition.y = RandomRange(minPosition.y, maxPosition.y);

		target.Path.push_back(currentPosition);
	}
}

/*
=================
PathManager::advanceActiveNode

Moves the "current" node one step forward, wrapping as necessary.
=================
 */
void PathManager::AdvanceActiveNode()
{
	if (++m_CurrentPosition >= Path.size())
		m_CurrentPosition = 0;
}

/*
================
PathManager::currentNode

Returns the current node in the path list
=================
 */
sf::Vector2f PathManager::CurrentNode()
{
	//return current node
	return Path [m_CurrentPosition];
}

sf::Vector2f PathManager::ClosestNodeToPoint( const sf::Vector2f & point )
{
	if (Path.size() == 0)
		return sf::Vector2f();

	sf::Vector2f currentClosestPoint = sf::Vector2f(1000000.0F, 1000000.0F);
	for (sf::Vector2f node : Path)
	{
		if (Vector::MagnitudeSquared(point - node) < Vector::MagnitudeSquared(point - currentClosestPoint))
			currentClosestPoint = node;
	}
	return currentClosestPoint;
}

void PathManager::DebugDraw()
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0F, 0.0F, 0.0F);
	glBegin (GL_LINE_LOOP);
	for (auto node : Path)
		glVertex2f(node.x, node.y);
	glEnd();
}

void PathManager::AdvanceActiveNodeToNode( const sf::Vector2f & node )
{
	for (uint i = 0; i < Path.size(); i++)
	{
		sf::Vector2f currentNode = this->CurrentNode();
		if (currentNode == Path[i])
			break;

		AdvanceActiveNode();
	}
}
