#pragma once
#include "Exam_HelperStructs.h"
#include <vector>
#include <iostream>

class IBaseInterface;
class IExamInterface;
class BehaviorTree;
class ISteeringBehavior;
class Wander;
class Inventory;


using namespace std;
using namespace Elite;

struct VisitedHouse
{
	Vector2 center;
	float width;
	float height;
	vector<Vector2> m_PointsInTheHouse;
	vector<Vector2> m_PointsToKeep;
	float offset = 5.0f;
	float maxDistFromPoint = 9.0f;


	float timeSinceVisited;
	bool needRevisiting;
	const float needVisitingTime{ 900.0f };

	VisitedHouse(HouseInfo& house)
	{
		center = house.Center;
		width = house.Size.x;
		height = house.Size.y;
		timeSinceVisited = 0;
		needRevisiting = true;
		CreateHousePoints();
		m_PointsToKeep = m_PointsInTheHouse;
	}


	void Update(float dt, Vector2 agentPos)
	{
		UpdateHouseState(dt);
		CheckHousePointDistances(agentPos);
	}

	void UpdateHouseState(float dt)
	{
		//updates house time since visited
		timeSinceVisited += dt;

		//if time since visited is greater than set need visiting time
		if (timeSinceVisited > needVisitingTime)
		{
			//then agent can return back
			needRevisiting = true;
			//resets time
			timeSinceVisited = 0;
			//recreated the house points
			CreateHousePoints();
		}
	}

	//creates the visiting points inside the house
	void CreateHousePoints()
	{
		Vector2 bottomLeft = { (center.x - width / 2) + offset, (center.y - height / 2) + offset };
		Vector2 middleLeft = { (center.x - width / 2) + offset, center.y };
		
		Vector2 topLeft = { (center.x - width / 2) + offset, (center.y + height / 2) - offset };
		Vector2 middleTop = { center.x,  (center.y + height / 2) - offset };
		
		Vector2 topRight = { (center.x + width / 2) - offset, (center.y + height / 2) - offset };
		Vector2 middleRight = { (center.x + width / 2) - offset, center.y };
		
		Vector2 bottomRight = { (center.x + width / 2) - offset, (center.y - height / 2) + offset };
		Vector2 middleBottom = { center.x,  (center.y - height / 2) + offset };

		m_PointsInTheHouse.push_back(middleTop);
		m_PointsInTheHouse.push_back(topRight);
		m_PointsInTheHouse.push_back(middleRight);
		m_PointsInTheHouse.push_back(bottomRight);
		m_PointsInTheHouse.push_back(middleBottom);
		m_PointsInTheHouse.push_back(bottomLeft);
		m_PointsInTheHouse.push_back(middleLeft);
		m_PointsInTheHouse.push_back(topLeft);
		m_PointsInTheHouse.push_back(center);


	}

	//returns house points
	vector<Vector2>& GetHousePoints()
	{
		return m_PointsInTheHouse;
	}

	void CheckHousePointDistances(Vector2 agentPos)
	{
		if (IsAgentInsideHouse(agentPos))
			return;
		for (size_t pos{ 0 }; pos < m_PointsInTheHouse.size(); ++pos)
		{
			//if agent if within a certain distance of the point, remove from the vector
			//this makes sure he wont go back to go somewhere he has already been
			auto distance = DistanceSquared(agentPos, m_PointsInTheHouse[pos]);
			if (distance <25.0f)
			{
				m_PointsInTheHouse.erase(m_PointsInTheHouse.begin() + pos);
			}
		}
	}

	bool IsAgentInsideHouse(Vector2 agentPos)
	{
		Vector2 bottomLeft = m_PointsToKeep[5];
		Vector2 bottomRight = m_PointsToKeep[1];
		Vector2 topLeft = m_PointsToKeep[7];
		Vector2 topRight = m_PointsToKeep[3];

		if (agentPos.x < (bottomLeft.x - offset))
			return false;
		if (agentPos.x > (topRight.x + offset))
			return false;
		if (agentPos.y < (bottomLeft.y - offset))
			return false;
		if (agentPos.y > (topRight.y + offset))
			return false;

		return true;
	}
};


class Agent
{
public:
	Agent(IExamInterface* pInterface);
	Agent(const Agent& other) = delete; //copy constructor
	Agent& operator=(const Agent& other) = delete; // assignment operator
	Agent(Agent&& other) = delete; // move constructor
	Agent& operator=(Agent&& other) = delete; // move assignment operator
	~Agent();

	void Update(float dt, IExamInterface* pInterface);

#pragma region steeringBehaviours
	//Behaviour Related
	////Wander
	void SetWander(SteeringPlugin_Output& steering);

	////Seek
	Vector2 GetSeekPoint() const;
	void SetSeekPoint(Vector2& seekPoint);
	void SetSeek(SteeringPlugin_Output& steering);

	////Face
	Vector2 GetEnemyPosition() const;
	void SetEnemyPosition(Vector2& position);
	void SetFace(SteeringPlugin_Output& steering);
	////TURN
	void SetTurn(SteeringPlugin_Output& steering);
	//uses the same point as the 

	////Flee
	void SetFlee(SteeringPlugin_Output& steering);
	void SetFleePoint(Vector2& position);
	Vector2 GetFleePoint() const;

#pragma endregion
	//HOUSES
	vector<VisitedHouse>& GetRegisteredHouses(); //returns the reference to the vector
	void RegisterHouse(HouseInfo& rHouse); //registers normal HouseInfo into the Visited house vector
	//void SetTargetHouse(VisitedHouse* targetHouse); //WILL BE USED TO SET THE CLOSEST HOUSE TO FLEE ENEMY
	
	//Inventory
	Inventory* GetInventory() const; //returns pointer to the inventory
	//Interface
	IExamInterface* GetInterface() const; //returns the pointer

	//Exploration
	vector<Vector2>& GetExplorationPoints(); //return a reference to the exploration points in the map

	//Stats
	float GetMinimumHealth() const;
	float GetMinimumEnergy() const;

private:

	// Initialize Steering behaviour pointers
	void InitializeSteeringBehaviors();
	//Create Exploration points
	void CreateExplorePoints(); 


	//UPDATES
	//Update VisitedHouses
	void UpdateHouses(float dt);
	//update explore points distances
	void CheckExplorePointsDistances();

	//STATS
	float m_MinimumHealth;
	float m_MinimumEnergy;

	//Interface pointer
	IExamInterface* m_pInterface = nullptr;

	//BEHAVIOUORS
	ISteeringBehavior* m_pWander = nullptr;
	ISteeringBehavior* m_pSeek = nullptr;
	ISteeringBehavior* m_pFace = nullptr;
	ISteeringBehavior* m_pFlee = nullptr;
	ISteeringBehavior* m_pTurn = nullptr;

	//Behaviours target points
	Vector2 m_SeekPoint;
	Vector2 m_FleePoint;
	Vector2 m_EnemyPosition;

	//INVENTORY
	Inventory* m_Invetory;

	//Houses
	vector<VisitedHouse> m_VisitedHouses;

	//EXPLORE
	vector<Vector2> m_PointsToExplore;

	//AGENT
	AgentInfo m_Agent;
};

