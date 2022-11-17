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

	std::vector<Elite::Vector2> rectPoints =
	{
		{ position.x - queryRadius , position.y - queryRadius  }, //bomttomLeft //0
		{ position.x - queryRadius ,  position.y + queryRadius  }, //topLeft //1
		{ position.x + queryRadius ,  position.y + queryRadius }, //topRight //2
		{ position.x + queryRadius ,  position.y - queryRadius  }, //bottomRight //3
	};

	//if (m_DegubAgent == agent)
		//IsDebugAgent(rectPoints, {0.0f, 1.0f, 0.0f});


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

				//if (m_DegubAgent == agent)
					//IsDebugAgent(rect, { 0.0f, 0.0f, 1.0f });


				for (SteeringAgent* pAgent : m_Cells[index].agents)
				{
					if (pAgent == agent)
						continue;
					if (Elite::IsPointInCircle(pAgent->GetPosition(), position, queryRadius))
						m_Neighbors[m_NrOfNeighbors++] = pAgent;
				}
			}
		}
	}
}






void CellSpace::EmptyCells()
{
	for (Cell& c : m_Cells)
		c.agents.clear();
}


#pragma region IndexPosition Calculations
int CellSpace::PositionToIndex(const Elite::Vector2 pos) const
{
	int widthIndex{ GetColumnIndex(pos.x) }; //Gets the width-> Column Index
	int heightIndex{ GetRowIndex(pos.y) };

	int index{ heightIndex * (m_NrOfCols) + widthIndex};

	return index;
}

int CellSpace::GetColumnIndex(const float xPosition) const
{
	int widthIndex{ static_cast<int>(xPosition / m_CellWidth) }; //The result will gives us the number of the column to which the xPos belongs to
																 //Because the result is an int, it will the correct cordinate, most of the times
	widthIndex = Elite::Clamp(widthIndex, 0, m_NrOfCols - 1);    //The xPos might go above the m_SpaceWidth. If that is the case the widthIndex might go outside of bounds
																 //That is fixed by clamping the width index between two values: 0 (min index) and m_NrOfCols-1 (max index)  
	return widthIndex;
}


int CellSpace::GetRowIndex(const float yPosition) const
{
	int heightIndex{ static_cast<int>(yPosition / m_CellHeight) }; //The result will gives us the number of the row to which the yPos belongs to
																   //Because the result is an int, it will the correct cordinate, most of the times
	heightIndex = Elite::Clamp(heightIndex, 0, m_NrOfRows - 1);	   //The yPos might go above the m_SpaceHeight. If that is the case the heightIndex might go outside of bounds
																   //That is fixed by clamping the width index between two values: 0 (min index) and m_NrOfRows-1 (max index)  
	return heightIndex;
}
#pragma endregion

#pragma region renders and debugs
void CellSpace::RenderCells() const
{
	for (int index{}; index < m_NrOfCells; ++index)
	{
		const std::vector<Elite::Vector2> rect = m_Cells[index].GetRectPoints();
		float deepth = DEBUGRENDERER2D->NextDepthSlice();

		DEBUGRENDERER2D->DrawSegment(rect[0], rect[1], { 1.0f, 0.0f, 0.0f }, deepth);
		DEBUGRENDERER2D->DrawSegment(rect[1], rect[2], { 1.0f, 0.0f, 0.0f }, deepth);
		DEBUGRENDERER2D->DrawSegment(rect[2], rect[3], { 1.0f, 0.0f, 0.0f }, deepth);
		DEBUGRENDERER2D->DrawSegment(rect[0], rect[3], { 1.0f, 0.0f, 0.0f }, deepth);

		std::string strNrNeighbors{ std::to_string(m_Cells[index].agents.size()) };

		DEBUGRENDERER2D->DrawString({ rect[1].x, rect[1].y }, strNrNeighbors.c_str());
	}
}

void CellSpace::SetDebugAgent(SteeringAgent* agent)
{
	m_DegubAgent = agent;
}

void CellSpace::SetDebugAgentNeighborhoodRadius(float queryradius)
{
	m_NeighborhoodRadius = queryradius;
}

void CellSpace::DebugAgent() const
{
	Elite::Vector2 position{ m_DegubAgent->GetPosition() };

	std::vector<Elite::Vector2> rectPoints =
	{
		{ position.x - m_NeighborhoodRadius , position.y - m_NeighborhoodRadius  }, //bomttomLeft //0
		{ position.x - m_NeighborhoodRadius ,  position.y + m_NeighborhoodRadius  }, //topLeft //1
		{ position.x + m_NeighborhoodRadius ,  position.y + m_NeighborhoodRadius }, //topRight //2
		{ position.x + m_NeighborhoodRadius ,  position.y - m_NeighborhoodRadius  }, //bottomRight //3
	};

	DEBUGRENDERER2D->DrawCircle(position, m_DegubAgent->GetRadius(), { 0.0f, 1.0f, 0.0f }, 0);
	DEBUGRENDERER2D->DrawSegment(rectPoints[0], rectPoints[1], { 0.0f, 1.0f, 0.0f });
	DEBUGRENDERER2D->DrawSegment(rectPoints[1], rectPoints[2], { 0.0f, 1.0f, 0.0f });
	DEBUGRENDERER2D->DrawSegment(rectPoints[2], rectPoints[3], { 0.0f, 1.0f, 0.0f });
	DEBUGRENDERER2D->DrawSegment(rectPoints[0], rectPoints[3], { 0.0f, 1.0f, 0.0f });

	int minColumn{ GetColumnIndex(rectPoints[0].x) };
	int maxColumn{ GetColumnIndex(rectPoints[2].x) };
	int minRow{ GetRowIndex(rectPoints[0].y) };
	int maxRow{ GetRowIndex(rectPoints[2].y) };


	for (int indexRow{ minRow }; indexRow <= maxRow; ++indexRow)
	{
		for (int indexColumn{ minColumn }; indexColumn <= maxColumn; ++indexColumn)
		{
			int index{ indexRow * (m_NrOfRows)+indexColumn };
			if (Elite::IsOverlapping(m_Cells[index].GetRectPoints(), position, m_NeighborhoodRadius))
			{
				const std::vector<Elite::Vector2> rect = m_Cells[index].GetRectPoints();
				DEBUGRENDERER2D->DrawCircle(position, m_NeighborhoodRadius, { 0.0f, 1.0f, 0.0f }, 0);
				DEBUGRENDERER2D->DrawSegment(rect[0], rect[1], { 0.0f, 0.0f, 1.0f });
				DEBUGRENDERER2D->DrawSegment(rect[1], rect[2], { 0.0f, 0.0f, 1.0f });
				DEBUGRENDERER2D->DrawSegment(rect[2], rect[3], { 0.0f, 0.0f, 1.0f });
				DEBUGRENDERER2D->DrawSegment(rect[0], rect[3], { 0.0f, 0.0f, 1.0f });
			}
		}
	}
}


void CellSpace::IsDebugAgent(std::vector<Elite::Vector2> rect, Elite::Color color) const
{
	DEBUGRENDERER2D->DrawCircle(m_DegubAgent->GetPosition(), m_DegubAgent->GetRadius(), color, 0);
	DEBUGRENDERER2D->DrawCircle(m_DegubAgent->GetPosition(), 5, color, 0); // chanfe 15 later
	DEBUGRENDERER2D->DrawSegment(rect[0], rect[1], color);
	DEBUGRENDERER2D->DrawSegment(rect[1], rect[2], color);
	DEBUGRENDERER2D->DrawSegment(rect[2], rect[3], color);
	DEBUGRENDERER2D->DrawSegment(rect[0], rect[3], color);
}
#pragma endregion