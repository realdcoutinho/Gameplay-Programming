#include "IExamInterface.h"
#include "stdafx.h"
#include "Agent.h"
#include "BlackBoard.h"
#include "IExamInterface.h"
#include "SteeringBehaviors.h"
#include "Inventory.h"

#include <iostream>

using namespace std;
using namespace Elite;

Agent::Agent(IExamInterface* pInterface)
	: m_pInterface{pInterface},
	m_Agent{ pInterface->Agent_GetInfo() },
	m_MinimumEnergy{4.0f},
	m_MinimumHealth{7.0f}
{
	std::cout << "agent class created" << '\n';
	
	m_Agent = m_pInterface->Agent_GetInfo();

	//Create Steering Behaviours
	InitializeSteeringBehaviors();
	CreateExplorePoints();
	m_Invetory = new Inventory();
}

void Agent::CreateExplorePoints()
{
	float nrOfPoints{ 10 };
	float radius{ 225.0f };
	float intervals{ 3.14f * 2 / nrOfPoints };

	for (int nr{ 0 }; nr < nrOfPoints; ++nr)
	{
		//Vector2 angle{ cosf(intervals + i * intervals/2)}
		Vector2 angle{ cosf(intervals * nr), sinf(intervals * nr) };
		Vector2 point = angle * radius;
		m_PointsToExplore.push_back(point);
	}

}

vector<Vector2>& Agent::GetExplorationPoints()
{
	return m_PointsToExplore;
}

Agent::~Agent()
{
	delete m_pInterface;
	delete m_Invetory;
	delete m_pWander;
	delete m_pSeek;
	delete m_pFace;
	delete m_pFlee;
}

#pragma region Steering
void Agent::InitializeSteeringBehaviors()
{
	m_pWander = new Wander();
	m_pSeek = new Seek();
	m_pFace = new Face();
	m_pFlee = new Flee();
}

//WANDER
void Agent::SetWander(SteeringPlugin_Output& steering)
{
	m_pWander->CalculateSteering(steering, this, m_pInterface);
}

//SEEK
Vector2 Agent::GetSeekPoint() const
{
	return m_SeekPoint;
}
//SEEK
void Agent::SetSeek(SteeringPlugin_Output& steering)
{
	m_pSeek->CalculateSteering(steering, this, m_pInterface);
}
//SEEK
void Agent::SetSeekPoint(Vector2& seekPoint)
{
	m_SeekPoint = seekPoint;
}

//FACE
Vector2 Agent::GetEnemyPosition() const
{
	return m_EnemyPosition;
}
//FACE
void Agent::SetEnemyPosition(Vector2& position)
{
	m_EnemyPosition = position;
}
//FACE
void Agent::SetFace(SteeringPlugin_Output& steering)
{
	m_pFace->CalculateSteering(steering, this, m_pInterface);
}
//FLEE
void Agent::SetFlee(SteeringPlugin_Output& steering)
{
	m_pFlee->CalculateSteering(steering, this, m_pInterface);
}

#pragma endregion

Inventory* Agent::GetInventory() const
{
	return m_Invetory;
}

vector<VisitedHouse>& Agent::GetRegisteredHouses() 
{
	return m_RegisteredHouses;
}

void Agent::RegisterHouse(HouseInfo& rHouse)
{
	m_RegisteredHouses.push_back(VisitedHouse(rHouse));
}

void Agent::Update(float dt, IExamInterface* pInterface)
{
	m_Agent = pInterface->Agent_GetInfo();
	m_Agent.Position;

	for (auto& house : m_RegisteredHouses)
	{
		house.Update(dt);
	}
}

float Agent::GetMinimumHealth()
{
	return m_MinimumHealth;
}

float Agent::GetMinimumEnergy()
{
	return m_MinimumEnergy;
}
