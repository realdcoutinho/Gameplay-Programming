#include "stdafx.h"
#include "SpacePartitioning.h"
#include "projects\Movement\SteeringBehaviors\SteeringAgent.h"

#include <iostream>
#include <string> // for string and to_string()
using namespace std;

// --- Cell ---
// ------------
Cell::Cell(float left, float bottom, float width, float height)
{
	boundingBox.bottomLeft = { left, bottom };
	boundingBox.width = width;
	boundingBox.height = height;
}

std::vector<Elite::Vector2> Cell::GetRectPoints() const
{
	auto left = boundingBox.bottomLeft.x;
	auto bottom = boundingBox.bottomLeft.y;
	auto width = boundingBox.width;
	auto height = boundingBox.height;

	std::vector<Elite::Vector2> rectPoints =
	{
		{ left , bottom  },
		{ left , bottom + height  },
		{ left + width , bottom + height },
		{ left + width , bottom  },
	};

	return rectPoints;
}

// --- Partitioned Space ---
// -------------------------
CellSpace::CellSpace(float width, float height, int rows, int cols, int maxEntities)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	, m_Neighbors(maxEntities)
	, m_NrOfNeighbors(0)
{
	InitializeCells();
}

void CellSpace::AddAgent(SteeringAgent* agent)
{
	int index = PositionToIndex(agent->GetPosition());
	m_Cells[index].agents.push_back(agent);
}

void CellSpace::UpdateAgentCell(SteeringAgent* agent, Elite::Vector2 oldPos)
{

		if (m_Cells[0].agents.size() > 0)
		{
			for (SteeringAgent* agents : m_Cells[0].agents)
			{
				//agents->SetBodyColor({ 1, 1, 1 });
				//std::cout << m_Cells[0].agents.size() << '\n';
				//std::cout << agents->GetPosition() << '\n';
				if (Elite::IsPointInRect(agents->GetPosition(), m_Cells[0].GetRectPoints()))
				{

				}
			}
		}



	//for (int index{}; index < m_NrOfCells; ++index)
	//{
	//
	//	if (m_Cells[index].agents.size() > 0)
	//	{
	//		for (SteeringAgent* agents : m_Cells[index].agents)
	//		{
	//			std::cout << m_Cells[index].agents.size() << '\n';
	//			std::cout << agents->GetPosition() << '\n';
	//			if (Elite::IsPointInRect(agents->GetPosition(), m_Cells[index].GetRectPoints()))
	//			{
	//
	//			}
	//		}
	//	}
	//}




	//	//for (SteeringAgent* agents : m_Cells[index].agents) 
	//	//{
	//	//	if (Elite::IsPointInRect(agents->GetPosition(), m_Cells[index].GetRectPoints()))
	//	//	{
	//	//	}
	//	//	else
	//	//	{
	//	//		
	//	//	}
	//	//}
	//}
}

void CellSpace::RegisterNeighbors(SteeringAgent* agent, float queryRadius)
{
	
}

void CellSpace::EmptyCells()
{
	for (Cell& c : m_Cells)
		c.agents.clear();
}

void CellSpace::RenderCells() const
{
	for (int index{}; index < m_NrOfCells; ++index)
	{
		const std::vector<Elite::Vector2> rect = m_Cells[index].GetRectPoints();
		DEBUGRENDERER2D->DrawSegment(rect[0], rect[1], { 1.0f, 0.0f, 0.0f });
		DEBUGRENDERER2D->DrawSegment(rect[1], rect[2], { 1.0f, 0.0f, 0.0f });
		DEBUGRENDERER2D->DrawSegment(rect[2], rect[3], { 1.0f, 0.0f, 0.0f });
		DEBUGRENDERER2D->DrawSegment(rect[0], rect[3], { 1.0f, 0.0f, 0.0f });

		std::string strNrNeighbors{ std::to_string(m_Cells[index].agents.size()) };
		std::string strNrIndex{ std::to_string(index) };

		DEBUGRENDERER2D->DrawString({ rect[1].x, rect[1].y }, strNrNeighbors.c_str());
		//DEBUGRENDERER2D->DrawString({ rect[1].x, rect[1].y }, strNrIndex.c_str());
	}
}

int CellSpace::PositionToIndex(const Elite::Vector2 pos) const
{
	for (int index{}; index < m_NrOfCells; ++index)
	{
		if (Elite::IsPointInRect(pos, m_Cells[index].GetRectPoints()))
		{
			return index;
		}
	}
	return NULL;
}

void CellSpace::InitializeCells()
{
	m_CellWidth = m_SpaceWidth / m_NrOfCols;
	m_CellHeight = m_SpaceHeight / m_NrOfRows;
	m_NrOfCells = m_NrOfCols * m_NrOfRows;
	//m_NrOfNeighbors = m_Cells

	for (int rows{}; rows < m_NrOfRows; ++rows)
	{
		for (int cols{}; cols < m_NrOfCols; ++cols)
		{
			int left{ cols * static_cast<int>(m_CellWidth) };
			int bottom{ rows * static_cast<int>(m_CellHeight) };
			m_Cells.push_back(Cell(static_cast<float>(left), static_cast<float>(bottom),
				static_cast<float>(m_CellWidth), static_cast<float>(m_CellHeight)));
		}
	}
}


