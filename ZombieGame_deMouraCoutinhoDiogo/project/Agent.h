#pragma once
#include "Exam_HelperStructs.h"
#include <vector>


class IBaseInterface;
class IExamInterface;
class BehaviorTree;

using namespace std;
using namespace Elite;

class Agent
{
public:
	Agent(IExamInterface* pInterface);
	~Agent();

	void Update(IExamInterface* pInterface);
	void AddHouse(const HouseInfo& houseSeen);
	vector<HouseInfo> GetRegisteredHouses() const;
	void SetDecisionMaking(BehaviorTree* decisionMakingStructure);

	Vector2 GetSeekPoint();
	void SetWander(SteeringPlugin_Output& steering);
	void SetFace(SteeringPlugin_Output& steering, EntityInfo& enemy);
	void SetFace(SteeringPlugin_Output& steering, Vector2& point);


private:
	IExamInterface* m_pInterface = nullptr;
	BehaviorTree* m_pDecisionMaking;
	BehaviorTree* m_pNewBehavior;


	float m_OffsetDistance = 6.0f; // OffSet (Agent Direction)
	float m_Radius = 4.0f; // WanderRadius
	float m_MaxAngleChange; //Max WanderAngle change per frame
	float m_WanderAngle = 0.f; //Interval
	float m_MaxAngularSpeed = 10.f;


	vector<ItemInfo> m_Inventory;
	vector<HouseInfo> m_RegisteredHouses;
	int nrKnownHouses;
	AgentInfo m_Agent;
};

