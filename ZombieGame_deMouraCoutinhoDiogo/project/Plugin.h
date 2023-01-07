#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
//#include "Blackboard.h"


class IBaseInterface;
class IExamInterface;
class Blackboard;
class Agent;
class BehaviorTree;
class IDecisionMaking;
class Inventory;

using namespace std;

class Plugin :public IExamPlugin
{
public:
	Plugin() {};
	virtual ~Plugin() {};

	void Initialize(IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

private:

	//Interface, used to request data from/perform actions with the AI Framework
	IExamInterface* m_pInterface = nullptr;

	std::vector<HouseInfo> GetHousesInFOV();
	std::vector<EntityInfo> GetEntitiesInFOV();

	Elite::Vector2 m_Target = {};
	bool m_CanRun = false; //Demo purpose
	bool m_GrabItem = false; //Demo purpose
	bool m_UseItem = false; //Demo purpose
	bool m_RemoveItem = false; //Demo purpose
	float m_AngSpeed = 0.f; //Demo purpose

	UINT m_InventorySlot = 0;

	//////////////
	// MINE
	//////////////
	Blackboard* m_pBlackBoard;
	Agent* m_pAgent;
	BehaviorTree* m_pBehaviorTree;

	SteeringPlugin_Output m_pSteeringOutput;

	Blackboard* CreateAgentBlackBoard(Agent* pAgent);
	void CreateBehaviorTree();
	void UpdateBlackBoard();

	vector<HouseInfo> m_HousesFOV;
	vector<EntityInfo> m_EnemiesFOV;
	vector<EntityInfo> m_ItemsFOV;
};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register()
	{
		return new Plugin();
	}
}