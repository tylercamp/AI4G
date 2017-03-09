
#include "World.h"
#include "Utility.h"
#include "Debug.h"

#include <gl/GL.h>
#include <algorithm>

#pragma warning (disable: 4244)
#pragma warning (disable: 4018)

bool GameEntityCompare(const GameEntity * a, const GameEntity * b)
{
	return (*a) > (*b);
}

/*
=================
SceneNode::SceneNode
=================
*/
SceneNode::SceneNode( uint x, uint y )
{
	m_ID = rand() * rand () + rand () - rand (); // Just get some random number
	X = x;
	Y = y;
}

/*
=================
SceneNode::GetID
=================
*/
uint SceneNode::GetID()
{
	return m_ID;
}

/*
=================
SceneNode::HasLinkToNode
=================
*/
bool SceneNode::HasLinkToNode( SceneNode * other )
{
	if (GetID() == other->GetID())
		return true;

	for (uint i = 0; i < Links.size(); i++)
	{
		if (Links[i]->GetOtherNode(this)->GetID() == other->GetID())
			return true;
	}

	return false;
}

/*
=================
NodeLink::NodeLink
=================
*/
NodeLink::NodeLink( )
{
	Nodes [0] = NULL;
	Nodes [1] = NULL;
}

/*
=================
NodeLink::GetJoint

Returns the node that is shared between the two links. Returns NULL if there is no joint.
=================
 */
SceneNode * NodeLink::GetJoint( const NodeLink & otherLink )
{
	if (Nodes[0]->GetID() == otherLink.Nodes[0]->GetID())
		return Nodes[0];
	if (Nodes[1]->GetID() == otherLink.Nodes[1]->GetID())
		return Nodes[1];
	if (Nodes[0]->GetID() == otherLink.Nodes[1]->GetID())
		return Nodes[0];
	if (Nodes[1]->GetID() == otherLink.Nodes[0]->GetID())
		return Nodes[1];

	return NULL;
}

/*
=================
NodeLink::IsActive
=================
 */
bool NodeLink::IsActive( ) const
{
	if (Nodes [0] == NULL || Nodes [1] == NULL)
		return false;

	return Nodes[0]->IsActive && Nodes[1]->IsActive;
}

/*
=================
NodeLink::GetOtherNode
=================
*/
SceneNode * NodeLink::GetOtherNode( SceneNode * thisSide )
{
	if (Nodes [0]->GetID() == thisSide->GetID())
		return Nodes [1];
	else
		return Nodes [0];
}

/*
=================
NodePath::SetPathLimits
=================
*/
void NodePath::SetPathLimits( const sf::Vector2f & startPosition, const sf::Vector2f & targetPosition)
{

}

/*
=================
NodePath::TargetPosition
=================
*/
sf::Vector2f NodePath::TargetPosition( )
{
	return m_TargetPosition;
}

/*
=================
NodePath::StartPosition
=================
*/
sf::Vector2f NodePath::StartPosition( )
{
	return m_StartPosition;
}

/*
=================
NodePath::GetNextPositionToTarget
=================
*/
sf::Vector2f NodePath::GetNextPositionToTarget( const sf::Vector2f & currentPosition )
{
	if (m_Path.size() == 0) return currentPosition;


	/* Need the nodes (not the links) in order to get the correct position */
	std::vector<sf::Vector2f> positions;
	GetNodes(positions);

	
	uint closestNode = -1;
	float closestDistance = 100000.0F;

	for (uint i = 0; i < positions.size(); i++)
	{
		float currentDistance = Vector::Magnitude(positions[i] - currentPosition);
		if (currentDistance < closestDistance)
		{
			closestDistance = currentDistance;
			closestNode = i;
		}
	}

	if (closestNode == positions.size() - 1)
		return positions[closestNode];

	return positions[closestNode + 1];
}

/*
=================
NodePath::Debug_Draw
=================
*/
void NodePath::Debug_Draw( sf::RenderWindow & target )
{
	glDisable (GL_TEXTURE_2D);

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();

	glTranslatef (m_SourceGraph->BasePosition.x, m_SourceGraph->BasePosition.y, 0.0F);
	glScalef (m_SourceGraph->PixelsPerCell, m_SourceGraph->PixelsPerCell, 0.0F);

	glLineWidth(5.0F);

	glColor4f (0.0F, 0.0F, 1.0F, 0.5F);
	glBegin (GL_LINES);
	for (uint i = 0; i < m_Path.size(); i++)
	{
		glVertex2f (m_Path[i]->Nodes[0]->X, m_Path[i]->Nodes[0]->Y);
		glVertex2f (m_Path[i]->Nodes[1]->X, m_Path[i]->Nodes[1]->Y);

	}
	glEnd ();

	glLineWidth(1.0F);

	glPopMatrix ();

	glEnable (GL_TEXTURE_2D);
}

/*
=================
NodePath::GetNodes

The ordering of nodes is undefined since NodeLink objects aren't required
to maintain them in any particular way. This method extracts node objects
and sorts them such that they are in proper order to go from point A
to point B.
=================
 */
void NodePath::GetNodes( std::vector<sf::Vector2f> & target )
{
	target.clear();

	if (m_Path.size() == 0) return;

	if (m_Path.size() == 1)
	{
		target.push_back(m_SourceGraph->TranslatePositionToWorld(m_Path[0]->Nodes[0]->X, m_Path[0]->Nodes[0]->Y));
		target.push_back(m_SourceGraph->TranslatePositionToWorld(m_Path[0]->Nodes[1]->X, m_Path[0]->Nodes[1]->Y));
		return;
	}

	std::vector<SceneNode *> intermediary;
	SceneNode * joint = m_Path[0]->GetJoint(*m_Path[1]);

	intermediary.push_back(m_Path[0]->GetOtherNode(joint));

	//	Add all the joints between links
	for (uint i = 0; i < m_Path.size()-1; i++)
	{
		intermediary.push_back (m_Path[i]->GetJoint(*m_Path[i+1]));
	}

	//	Add the last non-joint
	joint = m_Path.back()->GetJoint(*m_Path[m_Path.size()-2]);
	intermediary.push_back(m_Path.back()->GetOtherNode(joint));

	//	Convert from nodes to positions for output
	for (uint i = 0; i < intermediary.size(); i++)
	{
		target.push_back(m_SourceGraph->TranslatePositionToWorld(intermediary[i]->X, intermediary[i]->Y));
	}
}

/*
=================
SceneGraph::SceneGraph
=================
*/
SceneGraph::SceneGraph( )
{
	m_Width = 0;
	m_Height = 0;
}

/*
=================
SceneGraph::~SceneGraph
=================
*/
SceneGraph::~SceneGraph( )
{
	ClearGraphToSize(0, 0);
}

/*
=================
NodeRecord

Structure containing various pieces of data required for implementing path-finding.
=================
 */
struct NodeRecord
{
	SceneNode * Node;
	NodeRecord * PreviousNode;
	NodeLink * LinkToNode;
	float CostSoFar;

	NodeRecord() {}
	NodeRecord( SceneNode * node, NodeRecord * previous, NodeLink * link, float cost )
		: Node ( node ), PreviousNode( previous ), LinkToNode( link ), CostSoFar( cost )
	{
	}
};

/*
=================
GetCheapestNode

Returns the NodeRecord in the given container with the lowest CostSoFar.
=================
 */
NodeRecord * GetCheapestNode( const std::vector<NodeRecord *> & container )
{
	if (container.size() == 0) return NULL;
	
	NodeRecord * result = container.front();

	for (uint i = 1; i < container.size(); i++)
		if (result->CostSoFar > container[i]->CostSoFar) result = container [i];

	return result;
}

/*
=================
FindRecordForNode

Returns the record associated with the given node.
=================
 */
NodeRecord * FindRecordForNode( const std::vector<NodeRecord *> & container, SceneNode * node )
{
	for ( uint i = 0; i < container.size(); i++ )
		if (container [i]->Node->GetID() == node->GetID())
			return container [i];

	return NULL;
}

/*
=================
ContainsNode
=================
 */
bool ContainsNode( const std::vector<NodeRecord *> & container, SceneNode * node )
{
	for (int i = 0; i < container.size(); i++)
		if (container [i]->Node->GetID() == node->GetID())
			return true;

	return false;
}

/*
=================
RemoveNodeFromContainer
=================
 */
void RemoveNodeFromContainer( std::vector<NodeRecord *> & container, SceneNode * node )
{
	uint nodePosition = -1;
	for (uint i = 0; i < container.size(); i++)
	{
		if (container[i]->Node->GetID() == node->GetID())
		{
			nodePosition = i;
			break;
		}
	}

	if (nodePosition != -1)
		container.erase(container.begin() + nodePosition);
}

/*
=================
SceneGraph::FillNodePath
=================
*/
void SceneGraph::FillNodePath( NodePath & target, const sf::Vector2i & startPosition, const sf::Vector2i & targetPosition )
{
	std::vector<NodeRecord *> open;		open.reserve (m_Nodes.size());
	std::vector<NodeRecord *> closed;	closed.reserve (m_Nodes.size());

	SceneNode * startNode, * targetNode;
	startNode = GetNode(startPosition.x, startPosition.y);
	targetNode = GetNode(targetPosition.x, targetPosition.y);

	open.push_back( new NodeRecord( startNode, NULL, NULL, 0.0F ) );

	NodeRecord * finalNode = NULL;

	while( open.size() > 0 )
	{
		NodeRecord * smallestRecord = GetCheapestNode( open );
		if (smallestRecord->Node->GetID() == targetNode->GetID())
		{
			finalNode = smallestRecord;
			break;
		}

		for (uint i = 0; i < smallestRecord->Node->Links.size() && smallestRecord->Node->IsActive; i++)
		{
			NodeLink * currentLink = smallestRecord->Node->Links [i];
			SceneNode * currentNode = currentLink->GetOtherNode(smallestRecord->Node);

			if (!currentNode->IsActive)
				continue;

			float currentCostSoFar = smallestRecord->CostSoFar + currentLink->Cost;

			NodeRecord * currentRecord = NULL;

			if (ContainsNode( closed, currentNode ))
			{
				continue;
			}
			else if (ContainsNode( open, currentNode ))
			{
				currentRecord = FindRecordForNode( open, currentNode );
				if (currentRecord->CostSoFar < currentCostSoFar)
					continue;
			}
			else
			{
				currentRecord = new NodeRecord( );
				currentRecord->Node = currentNode;
			}

			currentRecord->CostSoFar = currentCostSoFar;
			currentRecord->PreviousNode = smallestRecord;
			currentRecord->LinkToNode = currentLink;

			if (!ContainsNode( open, currentNode ))
				open.push_back( currentRecord );
		}

		closed.push_back( smallestRecord );
		
		RemoveNodeFromContainer( open, smallestRecord->Node );
	}

	std::vector <NodeLink *> & nodes = target.m_Path;
	nodes.clear ();
	if (finalNode == NULL) goto cleanup;

	while( finalNode->Node->GetID() != startNode->GetID() )
	{
		nodes.push_back( finalNode->LinkToNode );
		finalNode = finalNode->PreviousNode;
	}

	std::reverse(nodes.begin(), nodes.end());

cleanup:
	for (uint i = 0; i < open.size(); i++)
	{
		delete open [i];
	}

	for (uint i = 0; i < closed.size(); i++)
	{
		delete closed [i];
	}
}

/*
=================
SceneGraph::ClearGraphToSize
=================
*/
void SceneGraph::ClearGraphToSize( uint width, uint height )
{
	for(uint i = 0; i < m_Nodes.size(); i++)
	{
		delete m_Nodes[i];
	}

	m_Nodes.clear();

	m_Nodes.reserve(width * height);
	for(uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			m_Nodes.push_back(new SceneNode(x, y));
		}
	}
	m_Width = width;
	m_Height = height;
}

/*
=================
SceneGraph::AutoGenerateLinkWeights
=================
*/
void SceneGraph::AutoGenerateLinks()
{
	if (m_Width == 0 || m_Height == 0)
		Debug::Error("Cannot call SceneGraph::AutoGenerateLinks when there are no nodes (size [%ui, %ui])", m_Width, m_Height);

	for (uint y = 0; y < m_Height; y++)
	{
		for (uint x = 0; x < m_Width; x++)
		{
			SceneNode
				* current	= GetNode(x, y),
				* left		= GetNode(x - 1, y),
				* right		= GetNode(x + 1, y),
				* top		= GetNode(x, y - 1),
				* bottom	= GetNode(x, y + 1),
				* topLeft	= GetNode(x - 1, y - 1),
				* topRight	= GetNode(x + 1, y - 1),
				* bottomLeft= GetNode(x - 1, y + 1),
				* bottomRight=GetNode(x + 1, y + 1);

			_GenerateLinkForNodes(current, left);
			_GenerateLinkForNodes(current, right);
			_GenerateLinkForNodes(current, top);
			_GenerateLinkForNodes(current, bottom);
			_GenerateLinkForNodes(current, topLeft);
			_GenerateLinkForNodes(current, topRight);
			_GenerateLinkForNodes(current, bottomLeft);
			_GenerateLinkForNodes(current, bottomRight);
		}
	}
}

/*
=================
SceneGraph::AddBlockedPosition
=================
*/
void SceneGraph::AddBlockedPosition( uint x, uint y )
{
	uint position = y * m_Width + x;

	if (GetNode(x, y) == NULL)
		return;

	delete m_Nodes [position];
	m_Nodes [position] = NULL;
}

/*
=================
SceneGraph::GetWidth
=================
*/
unsigned int SceneGraph::GetWidth()
{
	return m_Width;
}

/*
=================
SceneGraph::GetHeight
=================
*/
unsigned int SceneGraph::GetHeight()
{
	return m_Height;
}

/*
=================
SceneGraph::GetNode
=================
*/
SceneNode * SceneGraph::GetNode( uint x, uint y )
{
	uint position = y * m_Width + x;

	if (position >= m_Width * m_Height || position < 0 || y >= m_Height || y < 0 || x >= m_Width || x < 0)
		return NULL;

	return m_Nodes[position];
}

/*
=================
SceneGraph::Debug_Draw
=================
*/
void SceneGraph::Debug_Draw( sf::RenderWindow & target, sf::Vector2f basePosition, sf::Vector2f cellSize )
{
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(basePosition.x, basePosition.y, 0.0F);
	glScalef(cellSize.x, cellSize.y, 0.0F);

	glColor4f(0.0F, 0.0F, 0.0F, 0.5F);
	glBegin(GL_LINES);
	for (uint y = 0; y < m_Height; y++)
	{
		for (uint x = 0; x < m_Width; x++)
		{
			SceneNode * currentNode = GetNode(x, y);
			for (uint i = 0; i < currentNode->Links.size(); i++)
			{
				NodeLink * currentLink = currentNode->Links [i];
				if (!currentLink->IsActive())
					continue;

				glVertex2f(currentLink->Nodes[0]->X, currentLink->Nodes[0]->Y);
				glVertex2f(currentLink->Nodes[1]->X, currentLink->Nodes[1]->Y);
			}
		}
	}
	glEnd();

	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}

/*
=================
SceneGraph::_GenerateLinkForNodes
=================
*/
void SceneGraph::_GenerateLinkForNodes( SceneNode * node1, SceneNode * node2 )
{
	if (node1 == NULL || node2 == NULL) return;

	if (node1->HasLinkToNode(node2))
		return;

	NodeLink * newLink = new NodeLink ();
	newLink->Nodes[0] = node1;
	newLink->Nodes[1] = node2;

	node1->Links.push_back(newLink);
	node2->Links.push_back(newLink);

	newLink->Cost = sqrtf(sqr(node1->X - node2->X) + sqr(node1->Y - node2->Y));
}

/*
=================
GraphTranslator::TranslaatePositionToGraph
=================
 */
sf::Vector2i GraphTranslator::TranslatePositionToGraph(float x, float y) const
{
	sf::Vector2i result;
	result.x = (int)floorf((x - BasePosition.x) / PixelsPerCell + 0.5F);
	result.y = (int)floorf((y - BasePosition.y) / PixelsPerCell + 0.5F);

	result.x = min(result.x, (int)ManagedGraph->GetWidth() - 1);
	result.y = min(result.y, (int)ManagedGraph->GetHeight() - 1);

	result.x = max(0, result.x);
	result.y = max(0, result.y);

	return result;
}

/*
=================
GraphTranslator::TranslatePositionToGraph
=================
*/
sf::Vector2i GraphTranslator::TranslatePositionToGraph( const sf::Vector2f & position ) const
{
	return TranslatePositionToGraph(position.x, position.y);
}

/*
=================
GraphTranslator::TranslatePositionToWorld
=================
 */
sf::Vector2f GraphTranslator::TranslatePositionToWorld(uint x, uint y) const
{
	sf::Vector2f result;
	result.x = x * PixelsPerCell + BasePosition.x;
	result.y = y * PixelsPerCell + BasePosition.y;

	return result;
}

/*
=================
GraphTranslator::TranslatePositionToWorld
=================
*/
sf::Vector2f GraphTranslator::TranslatePositionToWorld( const sf::Vector2i & graphPosition ) const
{
	return TranslatePositionToWorld(graphPosition.x, graphPosition.y);
}

/*
=================
GraphTranslator::SyncGraphToEntities

Deactivates nodes within the graph that are covered by solid entities. Does
not activate nodes that are not covered. This is an expensive operation; to
be used sparingly.
=================
*/
void GraphTranslator::SyncGraphToEntities( const ComponentList & entities )
{
	for( auto iter = entities.begin(); iter != entities.end(); iter++)
	{
		if (!(*iter)->IsSolid())
			continue;

		CollisionMask * currentMask = (*iter)->GetCollisionMask();
		if (currentMask == NULL)
			continue;

		for( uint y = 0; y < ManagedGraph->GetHeight(); y++)
		{
			for( uint x = 0; x < ManagedGraph->GetWidth(); x++)
			{
				sf::Vector2f translatedPosition = TranslatePositionToWorld(x, y);

				if (currentMask->Intersects(translatedPosition))
					ManagedGraph->GetNode(x, y)->IsActive = false;
			}
		}
	}
}

/*
=================
GraphTranslator::Debug_Draw
=================
*/
void GraphTranslator::Debug_Draw( sf::RenderWindow & target ) const
{
	if (ManagedGraph)
		ManagedGraph->Debug_Draw( target, BasePosition, sf::Vector2f( PixelsPerCell, PixelsPerCell ));
}

/*
=================
GameWorld::GameWorld
=================
*/
GameWorld::GameWorld( sf::RenderWindow & targetWindow )
{
	m_TargetWindow = &targetWindow;

	m_Translator.ManagedGraph = &m_SceneGraph;
	m_Translator.BasePosition = sf::Vector2f(0.0F, 0.0F);
	m_Translator.PixelsPerCell = 10.0F;

	m_IsProcessing = false;
}

/*
=================
GameWorld::SetTargetWindow
=================
*/
void GameWorld::SetTargetWindow( sf::RenderWindow & targetWindow )
{
	m_TargetWindow = &targetWindow;
}

/*
=================
GameWorld::GetInput
=================
*/
const sf::Input & GameWorld::GetInput()
{
	return m_TargetWindow->GetInput();
}

/*
=================
GameWorld::AddEntity
=================
*/
GameEntity * GameWorld::AddEntity( GameEntity * target )
{
	if (m_IsProcessing)
		m_Add_Queue.push_back(target);
	else
		_ImmediateAddEntity(target);
	return target;
}

/*
=================
GameWorld::DetachEntity
=================
*/
void GameWorld::DetachEntity( GameEntity * target )
{
	if (m_IsProcessing)
		m_Detach_Queue.push_back(target);
	else
		_ImmediateDetachEntity(target);
}

/*
=================
GameWorld::DestroyEntity
=================
*/
void GameWorld::DestroyEntity( GameEntity * target )
{
	m_Destroy_Queue.push_back(target);
}

/*
=================
GameWorld::Update
=================
*/
void GameWorld::Update( float timeDelta )
{
	m_IsProcessing = true;
	for( auto entity : m_GameEntities )
	{
		entity->Update( timeDelta );
	}
	m_IsProcessing = false;

	_ApplyQueuedOperations();
}

/*
=================
GameWorld::Draw
=================
*/
void GameWorld::Draw()
{
	DrawToTarget(*m_TargetWindow);
}

/*
=================
GameWorld::DrawToTarget
=================
*/
void GameWorld::DrawToTarget( sf::RenderWindow & target )
{
	m_IsProcessing = true;
	for( auto entity : m_GameEntities )
	{
		entity->Draw( target );
	}
	m_IsProcessing = false;

	_ApplyQueuedOperations();
}

/*
=================
GameWorld::EntityPlaceFree
=================
*/
GameEntity * GameWorld::EntityPlaceFree( GameEntity & entity, const sf::Vector2f & position )
{
	CollisionMask * entityCollisionMask = entity.GetCollisionMask();

	if (entityCollisionMask == NULL)
		return false;

	sf::Vector2f originalPosition = entity.Position;
	entity.Position = position;

	GameEntity * result = NULL;

	for (auto currentEntity : m_GameEntities)
	{
		CollisionMask * currentMask = currentEntity->GetCollisionMask();
		if (currentMask == NULL)
			continue;

		if (currentMask->GetID() == entityCollisionMask->GetID())
			continue;

		if (!currentEntity->IsSolid())
			continue;

		if (entityCollisionMask->Intersects(currentMask))
		{
			result = currentEntity;
			break;
		}
	}

	entity.Position = originalPosition;

	return result;
}

/*
=================
GameWorld::EntityMoveSafe
=================
*/
sf::Vector2f GameWorld::EntityMoveSafe( GameEntity & entity, const sf::Vector2f & velocity )
{
	return sf::Vector2f ();
}

/*
=================
GameWorld::RaycastIntersects
=================
*/
bool GameWorld::RaycastIntersects( const sf::Vector2f & rayPosition, const sf::Vector2f & rayDirectionNormal, float maxDist )
{
	sf::Vector2f directionNormal = Vector::Normal(rayDirectionNormal); // Just to be safe

	for (auto currentEntity : m_GameEntities)
	{
		CollisionMask * currentMask = currentEntity->GetCollisionMask();

		if (currentMask == NULL)
			continue;

		if (!currentEntity->IsSolid())
			continue;

		if (currentMask->RaycastIntersects(rayPosition, rayDirectionNormal) && Vector::Magnitude(rayPosition - *currentMask->Position) <= maxDist)
			return true;
	}

	return false;
}

/*
=================
GameWorld::GetPathToTarget

Uses path-finding from the SceneGraph object
to return the next position that the object should move to in an attempt to
reach the given target position.
=================
*/
NodePath * GameWorld::GetPathToTarget( const sf::Vector2f & currentPosition, const sf::Vector2f & targetPosition )
{
	NodePath * result = new NodePath();

	result->m_SourceGraph = &m_Translator;
	m_SceneGraph.FillNodePath( *result, m_Translator.TranslatePositionToGraph( currentPosition ), m_Translator.TranslatePositionToGraph( targetPosition ) );

	return result;
}

/*
=================
GameWorld::GetSceneGraph
=================
*/
SceneGraph & GameWorld::GetSceneGraph()
{
	return m_SceneGraph;
}

/*
=================
GameWorld::GetGraphTranslator
=================
*/
const GraphTranslator & GameWorld::GetGraphTranslator( )
{
	return m_Translator;
}


/*
=================
GameWorld::BuildWorldGraph
=================
*/
void GameWorld::BuildWorldGraph( )
{
	_ApplyQueuedOperations( );
	m_Translator.SyncGraphToEntities( m_GameEntities );
}

PropertyStore & GameWorld::GetProperties()
{
	return m_Properties;
}


/*
=================
GameWorld::_ApplyQueuedOperations

All requests to add, destroy, and unlink game entities are queued to prevent
these operations from invalidating the loop in Draw() and Update().
=================
*/
void GameWorld::_ApplyQueuedOperations()
{
	for (unsigned int i = 0; i < m_Detach_Queue.size(); i++)
	{
		_ImmediateDetachEntity(m_Detach_Queue[i]);
	}

	for (unsigned int i = 0; i < m_Destroy_Queue.size(); i++)
	{
		_ImmediateDestroyEntity(m_Destroy_Queue[i]);
	}

	for (unsigned int i = 0; i < m_Add_Queue.size(); i++)
	{
		_ImmediateAddEntity(m_Add_Queue[i]);
	}

	m_Add_Queue.clear();
	m_Detach_Queue.clear();
	m_Destroy_Queue.clear();

	m_GameEntities.sort(GameEntityCompare);
}

/*
=================
GameWorld::_ImmediateDestroyEntity
=================
 */
void GameWorld::_ImmediateDestroyEntity( GameEntity * entity )
{
	_ImmediateDetachEntity(entity);
	delete entity;
}

/*
=================
GameWorld::_ImmediateDetachEntity
=================
 */
void GameWorld::_ImmediateDetachEntity( GameEntity * entity )
{
	entity->m_OwnerWorld = NULL;
	m_GameEntities.erase( entity->m_ComponentLocation );
}

/*
=================
GameWorld::_ImmediateAddEntity
=================
 */
void GameWorld::_ImmediateAddEntity( GameEntity * entity )
{
	m_GameEntities.push_back( entity );
	entity->m_OwnerWorld = this;
	entity->m_ComponentLocation = --m_GameEntities.end();
}