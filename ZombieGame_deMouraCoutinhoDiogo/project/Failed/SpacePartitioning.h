#pragma once
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>

class IExamInterface;

struct Cell
{
	Cell(float left, float bottom, float width, float height);

	std::vector<Elite::Vector2> GetRectPoints() const;

	// all the agents currently in this cell
	//std::list<SteeringAgent*> agents;

	Elite::Rect boundingBox;

	std::vector<Elite::Vector2> oldPositions;
};

class CellSpace
{

public:
	CellSpace(float width, float height, int rows, int cols, int maxEntities);
//
	//void AddAgent(SteeringAgent* agent);
//	void UpdateAgentCell(SteeringAgent* agent, Elite::Vector2 oldPos);
//
//	void RegisterNeighbors(SteeringAgent* agent, float queryRadius);
//	const std::vector<SteeringAgent*>& GetNeighbors() const { return m_Neighbors; }
//	int GetNrOfNeighbors() const { return m_NrOfNeighbors; }
//
//	//empties the cells of entities
//	void EmptyCells();
//
	void RenderCells(IExamInterface* interfaceDebuger)const;
	void RenderCells()const;

	std::vector<Cell> GetCells() const;
//	//my  functions
//	void SetDebugAgent(SteeringAgent* agent);
//	void SetDebugAgentNeighborhoodRadius(float queryradius);
//
//	void IsDebugAgent(std::vector<Elite::Vector2>, Elite::Color color) const;
//	void DebugAgent()const;
private:
	/////////// NEW////////////
	//std::vector<HouseInfo> houses;
	//std::vector<EntityInfo> entities;


//	// Cells and properties
	std::vector<Cell> m_Cells;

 	float m_SpaceWidth;
	float m_SpaceHeight;

	int m_NrOfRows;
	int m_NrOfCols;

	float m_CellWidth;
	float m_CellHeight;

//	// Members to avoid memory allocation on every frame
//	std::vector<SteeringAgent*> m_Neighbors;
//	int m_NrOfNeighbors;
//
//	// Helper functions
//	int PositionToIndex(const Elite::Vector2 pos) const;
//
	int m_NrOfCells;
//	SteeringAgent* m_DegubAgent{};
//	float m_NeighborhoodRadius{};
//
//
//	//my functions
	void InitializeCells();
//	int GetColumnIndex(const float xPosition) const;
//	int GetRowIndex(const float yPosition) const;
};

