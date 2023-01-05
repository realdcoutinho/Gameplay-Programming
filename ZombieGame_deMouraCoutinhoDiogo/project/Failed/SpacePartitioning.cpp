#include "stdafx.h"
#include "SpacePartitioning.h"
#include "Exam_HelperStructs.h"
#include "IExamInterface.h"


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


CellSpace::CellSpace(float width, float height, int rows, int cols, int maxEntities)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	//, m_Neighbors(maxEntities)
	//, m_NrOfNeighbors(0)
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
			int left{ cols * static_cast<int>(m_CellWidth) + static_cast<int>(-m_SpaceWidth /2)};
			int bottom{ rows * static_cast<int>(m_CellHeight) + static_cast<int>(-m_SpaceHeight/2)};
			m_Cells.push_back(Cell(static_cast<float>(left), static_cast<float>(bottom),
				static_cast<float>(m_CellWidth), static_cast<float>(m_CellHeight)));
		}
	}
}


void CellSpace::RenderCells(IExamInterface* interfaceDebuger) const
{
	for (int index{}; index < m_NrOfCells; ++index)
	{
		const std::vector<Elite::Vector2> rect = m_Cells[index].GetRectPoints();
		//float deepth = DEBUGRENDERER2D->NextDepthSlice();


		interfaceDebuger->Draw_Segment(rect[0], rect[1], { 1.0f, 0.0f, 0.0f });
		interfaceDebuger->Draw_Segment(rect[1], rect[2], { 1.0f, 0.0f, 0.0f });
		interfaceDebuger->Draw_Segment(rect[2], rect[3], { 1.0f, 0.0f, 0.0f });
		interfaceDebuger->Draw_Segment(rect[0], rect[3], { 1.0f, 0.0f, 0.0f });

		//std::string strNrNeighbors{ std::to_string(m_Cells[index].agents.size()) };

		//DEBUGRENDERER2D->DrawString({ rect[1].x, rect[1].y }, strNrNeighbors.c_str());
	}
}

void CellSpace::RenderCells() const
{
	for (int index{}; index < m_NrOfCells; ++index)
	{
		const std::vector<Elite::Vector2> rect = m_Cells[index].GetRectPoints();
		//float deepth = DEBUGRENDERER2D->NextDepthSlice();


		//interfaceDebuger.Draw_Segment(rect[0], rect[1], { 1.0f, 0.0f, 0.0f });
		//interfaceDebuger.Draw_Segment(rect[1], rect[2], { 1.0f, 0.0f, 0.0f });
		//interfaceDebuger.Draw_Segment(rect[2], rect[3], { 1.0f, 0.0f, 0.0f });
		//interfaceDebuger.Draw_Segment(rect[0], rect[3], { 1.0f, 0.0f, 0.0f });

		//std::string strNrNeighbors{ std::to_string(m_Cells[index].agents.size()) };

		//DEBUGRENDERER2D->DrawString({ rect[1].x, rect[1].y }, strNrNeighbors.c_str());
	}
}


std::vector<Cell> CellSpace::GetCells() const
{
	return m_Cells;
}