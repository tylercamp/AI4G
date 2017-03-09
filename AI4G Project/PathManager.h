
#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Type.h"

class PathManager
{
public:

	static void GenerateRandomPath( PathManager & target, uint nodeCount, const sf::Vector2f & minPosition, const sf::Vector2f & maxPosition, float minDistance );

	//Vector2 to store the nodes
	std::vector<sf::Vector2f> Path;

	PathManager();

	//Return current node
	sf::Vector2f CurrentNode();

	// Change the active node
	void AdvanceActiveNode();

	void AdvanceActiveNodeToNode( const sf::Vector2f & node );

	sf::Vector2f ClosestNodeToPoint( const sf::Vector2f & point );

	void DebugDraw();

private:

	uint m_CurrentPosition;
};