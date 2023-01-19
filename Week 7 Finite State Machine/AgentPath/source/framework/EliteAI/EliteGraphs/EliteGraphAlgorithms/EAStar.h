#pragma once
#include <unordered_set>
#include "framework/EliteAI/EliteNavigation/ENavigation.h"

namespace Elite
{
	template <class T_NodeType, class T_ConnectionType>
	class AStar
	{
	public:
		AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			T_NodeType* pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;

		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction)
		: m_pGraph(pGraph)
		, m_HeuristicFunction(hFunction)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> AStar<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pGoalNode)
	{
		std::vector<T_NodeType*> path;
		std::vector<NodeRecord> openList;
		std::vector<NodeRecord> closedList;
		NodeRecord currentRecord;

		NodeRecord startRecord;
		startRecord.pNode = pStartNode;
		startRecord.pConnection = nullptr;
		startRecord.estimatedTotalCost = GetHeuristicCost(pStartNode, pGoalNode);
		openList.push_back(startRecord);
		float costSoFar{};

		while (openList.size() != 0)
		{
			currentRecord = *std::min_element(openList.begin(), openList.end());

			if (currentRecord.pNode == pGoalNode)
			{
				break;
			}
			
			const auto recordConnections = m_pGraph->GetNodeConnections(currentRecord.pNode);
				
			for (const auto& connection : recordConnections)
			{
				costSoFar = currentRecord.costSoFar + connection->GetCost();
					//solution with for loop ..... much much slower // works perfectly for the A* Path tho.

					//for (const auto& closed : closedList)
					//{
					//	if (connection->GetTo() == closed.pNode->GetIndex())
					//	{
					//		if (closed.costSoFar <= costSoFar)
					//		{
					//			break;
					//		}
					//		else
					//		{
					//			closedList.erase(std::remove(closedList.begin(), closedList.end(), closed));
					//		}
					//	}
					//}
					//////// 2.E
					//for (const auto& open : openList)
					//{
					//	if (connection->GetFrom() == open.pNode->GetIndex())
					//	{
					//		if (open.costSoFar <= costSoFar)
					//		{
					//			break;
					//		}
					//		else
					//		{
					//			openList.erase(std::remove(openList.begin(), openList.end(), open));
					//		}
					//	}
					//}
			
				//solution with lambdas
				auto matchingNodeRecord = [&connection](const NodeRecord& findRecord) { return connection->GetTo() == findRecord.pNode->GetIndex(); };
				auto closedResult = std::find_if(closedList.begin(), closedList.end(), matchingNodeRecord);

				if (closedResult != std::end(closedList)) 
				{
					if (closedResult->costSoFar <= costSoFar)
					{
						continue;
					}
					else
					{
						closedList.erase(std::remove(closedList.begin(), closedList.end(), *closedResult));
					}
				}

				auto openResult = std::find_if(openList.begin(), openList.end(), matchingNodeRecord);
				if (openResult != std::end(openList))
				{
					if (openResult->costSoFar <= costSoFar)
					{
						continue;						
					}
					else
					{
						openList.erase(std::remove(openList.begin(), openList.end(), *openResult));
					}
				}

				NodeRecord newRecord;
				newRecord.pNode = m_pGraph->GetNode(connection->GetTo());
				newRecord.pConnection = connection;
				newRecord.costSoFar = costSoFar;
				newRecord.estimatedTotalCost = costSoFar + GetHeuristicCost(newRecord.pNode, pGoalNode);
				openList.push_back(newRecord);

			}		
			openList.erase(std::remove(openList.begin(), openList.end(), currentRecord));
			closedList.push_back(currentRecord);
		}

		while (currentRecord.pNode != pStartNode)
		{
			path.push_back(currentRecord.pNode);
			for (const auto& closed : closedList)
			{
				if (currentRecord.pConnection->GetFrom() == closed.pNode->GetIndex())
				{
					currentRecord = closed;
					break;
				}
			}
		}


		path.emplace_back(pStartNode);
		std::reverse(path.begin(), path.end());

		return path;
	}

	template <class T_NodeType, class T_ConnectionType>
	float Elite::AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		Vector2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}