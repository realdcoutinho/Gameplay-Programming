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




//struct Inventory
//{
//	Inventory(UINT _maxCapacity, vector<EntityInfo>& _itemVector) :
//		maxCapacity{ _maxCapacity },
//		nextavAilableSlot{0},
//		full{false},
//		itemVector{_itemVector}
//	{
//		std::cout << "Inventory Created" << '\n';
//	}
//
//	void AddItem(EntityInfo& item)
//	{
//		if (itemVector.size() < static_cast<size_t>(maxCapacity))
//		{
//			itemVector.push_back(item);
//			++nextavAilableSlot;
//		}
//		else
//		{
//			std::cout << "No more available slots" << '\n';
//		}
//	}
//	//void RemoveItemByType(EntityInfo& item)
//	//{
//	//	auto itemExists = [&item](const EntityInfo& itemStored) { return item.Type == itemStored.Type; };
//	//	auto itemFound = std::find_if(itemVector.begin(), itemVector.end(), itemExists);
//	//	if (itemFound == std::end(itemVector))
//	//	{
//	//		
//	//	}
//	//	else
//	//	{
//	//		RemoveItemLast();
//	//	}
//	//}
//	//
//	//void RemoveItemBySlot(UINT slotPosition)
//	//{
//	//	
//	//}
//
//	void RemoveItemLast()
//	{
//		itemVector.pop_back();
//	}
//
//	UINT maxCapacity;
//	UINT nextavAilableSlot;
//	bool full;
//	vector<EntityInfo> itemVector;
//};

struct VisitedHouse
{
	Vector2 Center;
	float timeSinceVisited;
	bool needRevisiting;

	const float needVisitingTime{ 300.0f };

	explicit VisitedHouse(HouseInfo& house)
	{
		Center = house.Center;
		timeSinceVisited = 0;
		needRevisiting = true;

	}

	void Update(float dt)
	{
		timeSinceVisited += dt;
		
		if (timeSinceVisited > needVisitingTime)
		{
			needRevisiting = true;
		}
	}
};


class Agent
{
public:
	Agent(IExamInterface* pInterface);
	~Agent();

	void Update(float dt, IExamInterface* pInterface);

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

	////Flee
	void SetFlee(SteeringPlugin_Output& steering);
	
	//GetFunctions
	//vector<HouseInfo> GetRegisteredHouses() const;
	//void RegisterHouse(HouseInfo& rHouse);

	vector<VisitedHouse>& GetRegisteredHouses() ;
	void RegisterHouse(HouseInfo& rHouse);

	//vector<HouseInfo> GetSearchedHouses() const;
	//void SearchedHouse(HouseInfo& sHouse);

	Inventory* GetInventory() const;


	vector<Vector2>& GetExplorationPoints();

	//Stats
	float GetMinimumHealth();
	float GetMinimumEnergy();

private:

	void InitializeSteeringBehaviors();
	void CreateExplorePoints();


	IExamInterface* m_pInterface = nullptr;
	BehaviorTree* m_pDecisionMaking;
	BehaviorTree* m_pNewBehavior;

	//STATS
	float m_MinimumHealth;
	float m_MinimumEnergy;

	//BEHAVIOUORS
	ISteeringBehavior* m_pWander = nullptr;
	ISteeringBehavior* m_pSeek = nullptr;
	ISteeringBehavior* m_pFace = nullptr;
	ISteeringBehavior* m_pFlee = nullptr;
	Vector2 m_SeekPoint;
	Vector2 m_EnemyPosition;

	//INVENTORY
	Inventory* m_Invetory;

	//Houses
	vector<VisitedHouse> m_RegisteredHouses;

	vector<Vector2> m_PointsToExplore;


	//vector<HouseInfo> m_RegisteredHouses;
	vector<HouseInfo> m_SearchedHouses;
	int nrKnownHouses;
	AgentInfo m_Agent;
};

