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
	m_MinimumEnergy{1.0f},
	m_MinimumHealth{7.9f}
{
	std::cout << "agent class created" << '\n';
	
	m_Agent = m_pInterface->Agent_GetInfo();

	//Create Steering Behaviours
	InitializeSteeringBehaviors();
	CreateExplorePoints();
	m_Invetory = new Inventory();
}

Agent::~Agent()
{
	delete m_pInterface;
	delete m_Invetory;
	delete m_pWander;
	delete m_pSeek;
	delete m_pFace;
	delete m_pFlee;
	delete m_pTurn;
}

void Agent::CreateExplorePoints()
{
	float nrOfPoints{ 8 };
	float intervals{ static_cast<float>(M_PI) * 2 / nrOfPoints };
	float intervals1{ static_cast<float>(M_PI) * 2 / 5 };




	float radius5{ 230.0f };
	float radius4{ 200.0f };
	float radius3{ 180.0f };
	float radius2{ 120.0f };
	float radius1{ 23.0f };


	for (int nr{ 0 }; nr < nrOfPoints; ++nr)
	{
		Vector2 angle{ cosf(intervals * nr), sinf(intervals * nr) };
		Vector2 point3 = angle * radius3;
		Vector2 point2 = angle * radius2;
		m_PointsToExplore.push_back(point3);
		m_PointsToExplore.push_back(point2);
	}
	for (int nr{ 0 }; nr < 5; ++nr)
	{
		Vector2 angle{ cosf(intervals1 * nr), sinf(intervals1 * nr) };
		Vector2 point1 = angle * radius1;
		m_PointsToExplore.push_back(point1);
	}

}

#pragma region Steering
void Agent::InitializeSteeringBehaviors()
{
	m_pWander = new Wander();
	m_pSeek = new Seek();
	m_pFace = new Face();
	m_pFlee = new Flee();
	m_pTurn = new Turn();
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

void Agent::SetFleePoint(Vector2& seekPoint)
{
	m_FleePoint = seekPoint;
}

Vector2 Agent::GetFleePoint() const
{
	return m_FleePoint;
}

//TURN
void Agent::SetTurn(SteeringPlugin_Output& steering)
{
	m_pTurn->CalculateSteering(steering, this, m_pInterface);
}

#pragma endregion

IExamInterface* Agent::GetInterface() const
{
	return m_pInterface;
}

vector<Vector2>& Agent::GetExplorationPoints()
{
	return m_PointsToExplore;
}


Inventory* Agent::GetInventory() const
{
	return m_Invetory;
}

vector<VisitedHouse>& Agent::GetRegisteredHouses() 
{
	return m_VisitedHouses;
}

void Agent::RegisterHouse(HouseInfo& rHouse)
{
	m_VisitedHouses.push_back(VisitedHouse(rHouse));
}


void Agent::Update(float dt, IExamInterface* pInterface)
{
	m_Agent = pInterface->Agent_GetInfo();
	UpdateHouses(dt);
	CheckExplorePointsDistances();



}

void Agent::UpdateHouses(float dt)
{
	for (auto& house : m_VisitedHouses)
	{
		house.Update(dt, m_Agent.Position);
	}
}

void Agent::CheckExplorePointsDistances()
{
	for (size_t pos{ 0 }; pos < m_PointsToExplore.size(); ++pos)
	{
		//if agent if within a certain distance of the point, remove from the vector
		//this makes sure he wont go back to go somewhere he has already been
		auto distance = DistanceSquared(m_Agent.Position, m_PointsToExplore[pos]);
		if (distance < 80.0f)
		{
			m_PointsToExplore.erase(m_PointsToExplore.begin() + pos);
		}
	}
}

float Agent::GetMinimumHealth() const
{
	return m_MinimumHealth;
}

float Agent::GetMinimumEnergy() const
{
	return m_MinimumEnergy;
}
