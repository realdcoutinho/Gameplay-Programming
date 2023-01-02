#pragma once
#include <vector>
#include <iostream>
#include "framework/EliteMath/EMath.h"
#include "framework\EliteAI\EliteGraphs\ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

namespace Elite
{
	class NavMeshPathfinding
	{
	public:
		static std::vector<Vector2> FindPath(Vector2 startPos, Vector2 endPos, NavGraph* pNavGraph, std::vector<Vector2>& debugNodePositions, std::vector<Portal>& debugPortals)
		{
			//Create the path to return
			std::vector<Vector2> finalPath{};

			auto navMesh = pNavGraph->GetNavMeshPolygon();
			auto triangles = navMesh->GetTriangles();

			auto startTri = navMesh->GetTriangleFromPosition(startPos);
			auto endTri = navMesh->GetTriangleFromPosition(endPos);;

			if (startTri == nullptr || endTri == nullptr)
			{
				return {};
			}
			if (startTri == endTri)
			{
				finalPath.push_back(endPos);
				return finalPath;
			}

			//We have valid start/end triangles and they are not the same
			//=> Start looking for a path
			//Copy the graph
			//shared_ptr
			auto graphClone = pNavGraph->Clone();
		
			//Create extra node for the Start Node (Agent's position)
			NavGraphNode *startNode = new NavGraphNode(graphClone.get()->GetNextFreeNodeIndex(), -1, startPos);

			graphClone->AddNode(startNode);

			auto linesTriangleIndices = startTri->metaData.IndexLines;
				
			for (const auto& lineIndex : linesTriangleIndices)
			{
				int nodeIndex = pNavGraph->GetNodeIdxFromLineIdx(lineIndex);
				if (nodeIndex != invalid_node_index)
				{
					const float distance{ Distance(startPos, graphClone->GetNodePos(graphClone->GetNode(nodeIndex))) };
					graphClone->AddConnection(new GraphConnection2D(startNode->GetIndex(), nodeIndex, distance));
				}
			}

			//Create extra node for the endNode
			NavGraphNode *endNode = new NavGraphNode(graphClone.get()->GetNextFreeNodeIndex(), -1, endPos);
			graphClone->AddNode(endNode);

			for (const auto& lineIndex :endTri->metaData.IndexLines)
			{
				int nodeIndex = pNavGraph->GetNodeIdxFromLineIdx(lineIndex);
				if (nodeIndex != invalid_node_index)
				{
					const float distance{ Distance(startPos, graphClone->GetNodePos(graphClone->GetNode(nodeIndex))) };
					graphClone->AddConnection(new GraphConnection2D(endNode->GetIndex(), nodeIndex, distance));
				}
			}


			//Run A star on new graph
			auto pathfinderAStar{ AStar<NavGraphNode, GraphConnection2D>(graphClone.get(), Elite::HeuristicFunctions::Chebyshev) };

			auto path = pathfinderAStar.FindPath(startNode, endNode);

			//OPTIONAL BUT ADVICED: Debug Visualisation
			debugNodePositions.clear();
			for (const auto& indexInPath : path)
			{
				debugNodePositions.push_back(indexInPath->GetPosition());
				finalPath.push_back(indexInPath->GetPosition());
				std::cout << "X: " << indexInPath->GetPosition().x << "     Y: " << indexInPath->GetPosition().y << '\n';
			}


			//Run optimiser on new graph, MAKE SURE the A star path is working properly before starting this section and uncommenting this!!!
			debugPortals = SSFA::FindPortals(path, pNavGraph->GetNavMeshPolygon());
			finalPath = SSFA::OptimizePortals(debugPortals);

			return finalPath;
		}
	};
}
