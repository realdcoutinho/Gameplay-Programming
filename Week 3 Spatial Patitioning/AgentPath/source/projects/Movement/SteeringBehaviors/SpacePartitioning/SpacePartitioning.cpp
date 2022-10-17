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
	int indexCurrent = PositionToIndex(agent->GetPosition());
	int indexOld = PositionToIndex(oldPos);

	if (indexCurrent != indexOld)
	{
		m_Cells[indexOld].agents.remove(agent);
		m_Cells[indexCurrent].agents.push_back(agent);
	}
}

void CellSpace::RegisterNeighbors(SteeringAgent* agent, float queryRadius)
{
	m_NrOfNeighbors = 0;
	Elite::Vector2 position{ agent->GetPosition() };


	//DEBUGRENDERER2D->DrawCircle(position, queryRadius, { 0, 1, 0 }, 0);

	std::vector<Elite::Vector2> rectPoints =
	{
		{ position.x - queryRadius , position.y - queryRadius  }, //bomttomLeft //0
		{ position.x - queryRadius ,  position.y + queryRadius  }, //topLeft //1
		{ position.x + queryRadius ,  position.y + queryRadius }, //topRight //2
		{ position.x + queryRadius ,  position.y - queryRadius  }, //bottomRight //3
	};

	//DEBUGRENDERER2D->DrawSegment(rectPoints[0], rectPoints[1], { 0.0f, 1.0f, 0.0f });
	//DEBUGRENDERER2D->DrawSegment(rectPoints[1], rectPoints[2], { 0.0f, 1.0f, 0.0f });
	//DEBUGRENDERER2D->DrawSegment(rectPoints[2], rectPoints[3], { 0.0f, 1.0f, 0.0f });
	//DEBUGRENDERER2D->DrawSegment(rectPoints[0], rectPoints[3], { 0.0f, 1.0f, 0.0f });

	int minColumn{ GetColumnIndex(rectPoints[0].x) };
	int maxColumn{ GetColumnIndex(rectPoints[2].x) };

	int minRow{ GetRowIndex(rectPoints[0].y) };
	int maxRow{ GetRowIndex(rectPoints[2].y) };


	for (int indexRow{ minRow }; indexRow <= maxRow; ++indexRow)
	{
		for (int indexColumn{ minColumn }; indexColumn <= maxColumn; ++indexColumn)
		{
			int index{ indexRow * (m_NrOfRows)+indexColumn };
			if (Elite::IsOverlapping(m_Cells[index].GetRectPoints(), position, queryRadius))
			{
				const std::vector<Elite::Vector2> rect = m_Cells[index].GetRectPoints();
				//DEBUGRENDERER2D->DrawSegment(rect[0], rect[1], { 0.0f, 0.0f, 1.0f });
				//DEBUGRENDERER2D->DrawSegment(rect[1], rect[2], { 0.0f, 0.0f, 1.0f });
				//DEBUGRENDERER2D->DrawSegment(rect[2], rect[3], { 0.0f, 0.0f, 1.0f });
				//DEBUGRENDERER2D->DrawSegment(rect[0], rect[3], { 0.0f, 0.0f, 1.0f });

				for (SteeringAgent* pAgent : m_Cells[index].agents)
				{
					if (pAgent == agent)
						continue;
					if (Elite::IsPointInCircle(pAgent->GetPosition(), position, queryRadius))
					{
						m_Neighbors[m_NrOfNeighbors++] = pAgent;
						//++m_NrOfNeighbors;
					}
				}

				//for (SteeringAgent* agent : m_Cells[index].agents)
				//{
				//	if (Elite::IsPointInCircle(agent->GetPosition(), position, queryRadius))
				//	{
				//		m_Neighbors[m_NrOfNeighbors] = agent;
				//		++m_NrOfNeighbors;
				//	}
				//}
			}
		}
	}
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
	int widthIndex{ GetColumnIndex(pos.x) };
	int heightIndex{ GetRowIndex(pos.y) };

	int index{ heightIndex * (m_NrOfCols) + widthIndex};
	assert(index < m_NrOfCells);

	//index = Elite::Clamp(index, 0, m_NrOfCells);
	return index;
}

int CellSpace::GetColumnIndex(const float xPosition) const
{
	int widthIndex{ static_cast<int>(xPosition / m_CellWidth) };
	widthIndex = Elite::Clamp(widthIndex, 0, m_NrOfCols - 1);
	return widthIndex;
}


int CellSpace::GetRowIndex(const float yPosition) const
{
	int heightIndex{ static_cast<int>(yPosition / m_CellHeight) };
	heightIndex = Elite::Clamp(heightIndex, 0, m_NrOfRows - 1);
	return heightIndex;
}

void CellSpace::InitializeCells()
{
	m_CellWidth = m_SpaceWidth / m_NrOfCols; //width
	m_CellHeight = m_SpaceHeight / m_NrOfRows; //height
	m_NrOfCells = m_NrOfCols * m_NrOfRows; //equal to area

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


