
#pragma once

#include "GameEntity.h"
#include "Type.h"
#include "PropertyStore.h"

struct SceneNode;
struct NodeLink;
class NodePath;
class SceneGraph;
class GraphTranslator;

struct SceneNode
{
					SceneNode( uint x, uint y );

	uint			GetID( );
	bool			HasLinkToNode( SceneNode * other );



	std::vector <NodeLink *> Links;

	NodeLink *		GetLinkToNode;
	uint			X, Y;

	bool			IsActive;

private:
	uint			m_ID;
};



struct NodeLink
{
	SceneNode *		Nodes [2];
	float			Cost;

					NodeLink( );

	SceneNode *		GetJoint( const NodeLink & otherLink );

	bool			IsActive( ) const;
	SceneNode *		GetOtherNode( SceneNode * thisSide );
};



class NodePath
{
public:

	void						SetPathLimits( const sf::Vector2f & startPosition, const sf::Vector2f & targetPosition );

	sf::Vector2f				TargetPosition( );
	sf::Vector2f				StartPosition( );

	sf::Vector2f				GetNextPositionToTarget( const sf::Vector2f & currentPosition );

	void						Debug_Draw( sf::RenderWindow & target );

	void						GetNodes( std::vector<sf::Vector2f> & target );

protected:
	std::vector <NodeLink *>	m_Path;
	GraphTranslator *			m_SourceGraph;

	friend class				SceneGraph;
	friend class				GameWorld;

private:
	sf::Vector2f				m_StartPosition;
	sf::Vector2f				m_TargetPosition;
};



class SceneGraph
{
public:

								SceneGraph( );
								~SceneGraph( );

	void						FillNodePath( NodePath & target, const sf::Vector2i & startPosition, const sf::Vector2i & targetPosition );

	void						ClearGraphToSize( uint width, uint height );
	void						AutoGenerateLinks( );

	void						AddBlockedPosition( uint x, uint y );

	unsigned int				GetWidth( );
	unsigned int				GetHeight( );

	SceneNode *					GetNode( uint x, uint y );

	void						Debug_Draw( sf::RenderWindow & target, sf::Vector2f basePosition, sf::Vector2f cellSize );

private:
	void						_GenerateLinkForNodes( SceneNode * node1, SceneNode * node2 );

	std::vector <SceneNode *>	m_Nodes;

	uint						m_Width;
	uint						m_Height;
};



class GraphTranslator
{
public:
	SceneGraph *		ManagedGraph;

	sf::Vector2i		TranslatePositionToGraph( float x, float y) const;
	sf::Vector2i		TranslatePositionToGraph( const sf::Vector2f & position ) const;
	sf::Vector2f		TranslatePositionToWorld( uint x, uint y) const;
	sf::Vector2f		TranslatePositionToWorld( const sf::Vector2i & graphPosition )
 const;

	void				SyncGraphToEntities( const ComponentList & entities );

	void				Debug_Draw( sf::RenderWindow & target ) const;

	sf::Vector2f		BasePosition;
	float				PixelsPerCell;
};




class GameWorld
{
public:

							GameWorld( sf::RenderWindow & targetWindow );

	void					SetTargetWindow( sf::RenderWindow & targetWindow );

	const sf::Input &		GetInput( );

	GameEntity *			AddEntity( GameEntity * target );
	void					DetachEntity( GameEntity * target );
	void					DestroyEntity( GameEntity * target );

	void					Update( float timeDelta );
	void					Draw( );
	void					DrawToTarget( sf::RenderWindow & target );

	GameEntity *			EntityPlaceFree( GameEntity & entity, const sf::Vector2f & position );
	sf::Vector2f			EntityMoveSafe( GameEntity & entity, const sf::Vector2f & velocity );
	bool					RaycastIntersects( const sf::Vector2f & rayPosition, const sf::Vector2f & rayDirectionNormal, float maxDist );

	NodePath *				GetPathToTarget( const sf::Vector2f & currentPosition, const sf::Vector2f & targetPosition );

	SceneGraph &			GetSceneGraph( );
	const GraphTranslator & GetGraphTranslator( );

	void					BuildWorldGraph( );

	PropertyStore &			GetProperties( );

private:
	void					_ApplyQueuedOperations( );

	void					_ImmediateDestroyEntity( GameEntity * entity );
	void					_ImmediateDetachEntity( GameEntity * entity );
	void					_ImmediateAddEntity( GameEntity * entity );

	SceneGraph				m_SceneGraph;
	GraphTranslator			m_Translator;

	ComponentList			m_GameEntities;

	AtomicComponentList		m_Add_Queue;
	AtomicComponentList		m_Destroy_Queue;
	AtomicComponentList		m_Detach_Queue;

	bool					m_IsProcessing;

	sf::RenderWindow *		m_TargetWindow;

	PropertyStore 			m_Properties;
};