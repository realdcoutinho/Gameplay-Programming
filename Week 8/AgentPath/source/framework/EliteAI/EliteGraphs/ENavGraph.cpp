#include "stdafx.h"
#include "ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

using namespace Elite;

Elite::NavGraph::NavGraph(const Polygon& contourMesh, float playerRadius = 1.0f) :
	Graph2D(false),
	m_pNavMeshPolygon(nullptr)
{
	//Create the navigation mesh (polygon of navigatable area= Contour - Static Shapes)
	m_pNavMeshPolygon = new Polygon(contourMesh); // Create copy on heap

	//Get all shapes from all static rigidbodies with NavigationCollider flag
	auto vShapes = PHYSICSWORLD->GetAllStaticShapesInWorld(PhysicsFlags::NavigationCollider);

	//Store all children
	for (auto shape : vShapes)
	{
		shape.ExpandShape(playerRadius);
		m_pNavMeshPolygon->AddChild(shape);
	}

	//Triangulate
	m_pNavMeshPolygon->Triangulate();

	//Create the actual graph (nodes & connections) from the navigation mesh
	CreateNavigationGraph();
}

Elite::NavGraph::~NavGraph()
{
	delete m_pNavMeshPolygon; 
	m_pNavMeshPolygon = nullptr;
}

int Elite::NavGraph::GetNodeIdxFromLineIdx(int lineIdx) const
{
	auto nodeIt = std::find_if(m_Nodes.begin(), m_Nodes.end(), [lineIdx](const NavGraphNode* n) { return n->GetLineIndex() == lineIdx; });
	if (nodeIt != m_Nodes.end())
	{
		return (*nodeIt)->GetIndex();
	}

	return invalid_node_index;
}

Elite::Polygon* Elite::NavGraph::GetNavMeshPolygon() const
{
	return m_pNavMeshPolygon;
}

void Elite::NavGraph::CreateNavigationGraph()
{
	//1. Go over all the edges of the navigationmesh and create nodes
	auto lines = m_pNavMeshPolygon->GetLines();

	for (const auto& line : lines)
	{
		int index = line->index;
		auto triangles = m_pNavMeshPolygon->GetTrianglesFromLineIndex(index);
		if (triangles.size() == 2)
		{
			Vector2 midPoint{ (line->p1 + line->p2) / 2 };
			AddNode(new NavGraphNode( GetNextFreeNodeIndex(), index, midPoint));
		}
	}
	
	//2. Create connections now that every node is created
	auto totalTriangles = m_pNavMeshPolygon->GetTriangles();
	for (const auto& tri : totalTriangles)
	{
		auto lineIndexes = tri->metaData.IndexLines;
		std::vector<int> validNodeIndices;
		for (const auto& line : lineIndexes)
		{
			if (GetNodeIdxFromLineIdx(line) != invalid_node_index)
			{
				validNodeIndices.push_back(GetNodeIdxFromLineIdx(line));
			}
		}
		if (validNodeIndices.size() == 2)
		{
			AddConnection(new GraphConnection2D(validNodeIndices[0], validNodeIndices[1]));
		}
		if (validNodeIndices.size() == 3)
		{
			AddConnection(new GraphConnection2D(validNodeIndices[0], validNodeIndices[1]));
			AddConnection(new GraphConnection2D(validNodeIndices[1], validNodeIndices[2]));
			AddConnection(new GraphConnection2D(validNodeIndices[2], validNodeIndices[0]));
		}
	}

	//3. Set the connections cost to the actual distance
	SetConnectionCostsToDistance();
}

