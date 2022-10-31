#pragma once

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


		while (openList.size() != 0)
		{
			currentRecord = *std::min_element(openList.begin(), openList.end());

			if (currentRecord.pNode == pGoalNode)
			{
				break;
			}
			if (currentRecord.pNode != pGoalNode)
			{
				float totalCost{};
				auto connections = m_pGraph->GetNodeConnections(currentRecord.pNode);
				for (auto& cone : connections)
				{
					totalCost += cone->GetCost();
					for (auto& closed : closedList)
					{
						auto node = m_pGraph->GetNode(cone->GetTo());
						if (closed.pNode == node)
						{
							float cost = GetHeuristicCost(currentRecord.pNode, closed.pNode);
						}
					}
					for (auto& open : openList)
					{
						auto node = m_pGraph->GetNode(cone->GetTo());
						if (open.pNode == node)
						{
							std::cout << "YO" << '\n';
						}
					}




				}

			}
		}


		//one thing I am not understanding in the slides is that, in the early ones, you start by adding your starting Node to the closed list.you get all the neighboring nodes from the startingNodeand then add those to the open list



		return std::vector<T_NodeType*>();
	}

	template <class T_NodeType, class T_ConnectionType>
	float Elite::AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		Vector2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}