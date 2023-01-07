#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"
//#include "SteeringBehaviors.h"
//#include "SpacePartitioning.h"
#include "Blackboard.h"
#include "Agent.h"
#include "EBehaviorTree.h"
#include "Behaviors.h"
#include "Inventory.h"

using namespace std;
using namespace Elite;


//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);
	m_pAgent = new Agent(m_pInterface);

	//Bit information about the plugin
	//Please fill this in!!
	info.BotName = "Siuuuu";
	info.Student_FirstName = "Diogo";
	info.Student_LastName = "Coutinho";
	info.Student_Class = "2DAE08";

	m_pBlackBoard = CreateAgentBlackBoard(m_pAgent);
	CreateBehaviorTree();




	//for (int nr{ 0 }; nr < 10; ++nr)
	//{
	//	Vector2 toEplore;
	//	
	//}


}

Blackboard* Plugin::CreateAgentBlackBoard(Agent* pAgent)
{
	Blackboard* pBlackboard = new Blackboard();

	//agent realted stuff
	pBlackboard->AddData("Agent", m_pAgent);
	pBlackboard->AddData("AgentInfo", m_pInterface->Agent_GetInfo());
	//pBlackboard->AddData("RegisterHouses", m_pAgent->GetRegisteredHouses());
	//pBlackboard->AddData("SearchedHouses", m_pAgent->GetSearchedHouses());

	VisitedHouse* phouse{ nullptr };
	pBlackboard->AddData("TargetHouse", phouse);

	//inventory
	pBlackboard->AddData("Inventory", m_pAgent->GetInventory());

	//INTERFACE
	pBlackboard->AddData("pInterface", m_pInterface);

	//bool for which gun to use
	bool multipleEnemies{ false };
	pBlackboard->AddData("MultipleEnemies", multipleEnemies);

	//INFO
	pBlackboard->AddData("WorldInfo", m_pInterface->World_GetInfo());
	pBlackboard->AddData("HousesFOV", m_HousesFOV);
	pBlackboard->AddData("EnemiesFOV", m_EnemiesFOV);
	pBlackboard->AddData("ItemsFOV", m_ItemsFOV);

	//Steerings
	pBlackboard->AddData("SteeringOutput", m_pSteeringOutput);
	pBlackboard->AddData("LookAt", Vector2{});
	pBlackboard->AddData("Seek", Vector2{});
	pBlackboard->AddData("Face", Vector2{});

	//Stats
	pBlackboard->AddData("MinimumHealth", m_pAgent->GetMinimumHealth());
	pBlackboard->AddData("MinimumEnergy", m_pAgent->GetMinimumEnergy());

	return pBlackboard;
}


void Plugin::UpdateBlackBoard()
{	
	m_HousesFOV.clear();
	m_EnemiesFOV.clear();
	m_ItemsFOV.clear();
	GetHousesInFOV();
	GetEntitiesInFOV();

	//INTERFACE
	m_pBlackBoard->ChangeData("pInterface", m_pInterface);
	//INFO
	m_pBlackBoard->ChangeData("AgentInfo", m_pInterface->Agent_GetInfo());
	m_pBlackBoard->ChangeData("HousesFOV", m_HousesFOV);
	m_pBlackBoard->ChangeData("EnemiesFOV", m_EnemiesFOV);
	m_pBlackBoard->ChangeData("ItemsFOV", m_ItemsFOV);
	
	//STEERING
	m_pBlackBoard->ChangeData("SteeringOutput", m_pSteeringOutput);

}


//Called only once
void Plugin::DllInit()
{
	//Called when the plugin is loaded
}

//Called only once
void Plugin::DllShutdown()
{
	//Called wheb the plugin gets unloaded
}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 100; //How many enemies? (Default = 20)
	params.GodMode = false; //GodMode > You can't die, can be useful to inspect certain behaviors (Default = false)
	params.LevelFile = "GameLevel.gppl";
	params.AutoGrabClosestItem = true; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	params.StartingDifficultyStage = 1;
	params.InfiniteStamina = false;
	params.SpawnDebugPistol = true;
	params.SpawnDebugShotgun = true;
	params.SpawnPurgeZonesOnMiddleClick = true;
	params.PrintDebugMessages = true;
	params.ShowDebugItemNames = true;
	params.Seed = 36;
}


//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::Update(float dt)
{
	//Demo Event Code
	//In the end your AI should be able to walk around without external input
	if (m_pInterface->Input_IsMouseButtonUp(Elite::InputMouseButton::eLeft))
	{
		//Update target based on input
		Elite::MouseData mouseData = m_pInterface->Input_GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y));
		m_Target = m_pInterface->Debug_ConvertScreenToWorld(pos);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Space))
	{
		m_CanRun = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Left))
	{
		m_AngSpeed -= Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Right))
	{
		m_AngSpeed += Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_G))
	{
		m_GrabItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_U))
	{
		m_UseItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_R))
	{
		m_RemoveItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyUp(Elite::eScancode_Space))
	{
		m_CanRun = false;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Delete))
	{
		m_pInterface->RequestShutdown();
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Minus))
	{
		if (m_InventorySlot > 0)
			--m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Plus))
	{
		if (m_InventorySlot < 4)
			++m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Q))
	{
		ItemInfo info = {};
		m_pInterface->Inventory_GetItem(m_InventorySlot, info);
		std::cout << (int)info.Type << std::endl;
	}
}

//Update
//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{
	auto worldStats = m_pInterface->World_GetStats();

	m_pSteeringOutput = SteeringPlugin_Output();

	auto steering = SteeringPlugin_Output();
	//steering.AutoOrient = false;
	
	
	//Use the Interface (IAssignmentInterface) to 'interface' with the AI_Framework
	auto agentInfo = m_pInterface->Agent_GetInfo();

	auto position = agentInfo.Position;

	steering.AutoOrient = true;

	//Use the navmesh to calculate the next navmesh point
	//auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(checkpointLocation);

	//OR, Use the mouse target
	auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(m_Target); //Uncomment this to use mouse position as guidance
	//m_pInterface


	//auto vHousesInFOV = GetHousesInFOV();//uses m_pInterface->Fov_GetHouseByIndex(...)
	//auto vEntitiesInFOV = GetEntitiesInFOV(); //uses m_pInterface->Fov_GetEntityByIndex(...)

	//UINT maxCapacity = m_pInterface->Inventory_GetCapacity();

	//for (auto& e : vEntitiesInFOV)
	//{
	//	if (e.Type == eEntityType::PURGEZONE)
	//	{
	//		PurgeZoneInfo zoneInfo;
	//		m_pInterface->PurgeZone_GetInfo(e, zoneInfo);
	//		//std::cout << "Purge Zone in FOV:" << e.Location.x << ", "<< e.Location.y << "---Radius: "<< zoneInfo.Radius << std::endl;
	//	}
	//	if (e.Type == eEntityType::ITEM)
	//	{
	//		
	//		ItemInfo itemInfo;
	//		bool isIn = m_pInterface->Item_Grab(e, itemInfo);
	//		if (isIn)
	//		{
	//			if (m_InventorySlot == maxCapacity)
	//			{
	//				--m_InventorySlot;
	//				m_pInterface->Inventory_RemoveItem(m_InventorySlot);
	//			}
	//			if (m_InventorySlot < maxCapacity)
	//			{
	//				m_pInterface->Inventory_AddItem(m_InventorySlot, itemInfo);
	//				//++m_InventorySlot;
	//			}
	//		}
	//			
	//		
	//		//std::cout << " in FOV:" << e.Location.x << ", "<< e.Location.y << std::endl;
	//	}

	//}

	//INVENTORY USAGE DEMO
	//********************

	if (m_GrabItem)
	{
		ItemInfo item;
		//Item_Grab > When DebugParams.AutoGrabClosestItem is TRUE, the Item_Grab function returns the closest item in range
		//Keep in mind that DebugParams are only used for debugging purposes, by default this flag is FALSE
		//Otherwise, use GetEntitiesInFOV() to retrieve a vector of all entities in the FOV (EntityInfo)
		//Item_Grab gives you the ItemInfo back, based on the passed EntityHash (retrieved by GetEntitiesInFOV)
		if (m_pInterface->Item_Grab({}, item))
		{
			//Once grabbed, you can add it to a specific inventory slot
			//Slot must be empty
			m_pInterface->Inventory_AddItem(m_InventorySlot, item);
		}
	}

	if (m_UseItem)
	{
		//Use an item (make sure there is an item at the given inventory slot)
		m_pInterface->Inventory_UseItem(m_InventorySlot);
	}

	if (m_RemoveItem)
	{
		//Remove an item from a inventory slot
		m_pInterface->Inventory_RemoveItem(m_InventorySlot);
	}

	//Simple Seek Behaviour (towards Target)
	steering.LinearVelocity = nextTargetPos - agentInfo.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agentInfo.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agentInfo.Position) < 2.f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
	}

	//steering.AngularVelocity = m_AngSpeed; //Rotate your character to inspect the world while walking
	steering.AutoOrient = true; //Setting AutoOrient to TRue overrides the AngularVelocity

	steering.RunMode = m_CanRun; //If RunMode is True > MaxLinSpd is increased for a limited time (till your stamina runs out)
	

//@End (Demo Purposes)
	m_GrabItem = false; //Reset State
	m_UseItem = false;
	m_RemoveItem = false;

	UpdateBlackBoard();
	m_pAgent->Update(dt, m_pInterface);
	m_pBehaviorTree->Update(dt);


	m_pBlackBoard->GetData("SteeringOutput", steering);	
	return steering;
}


//This function should only be used for rendering debug elements
void Plugin::Render(float dt) const
{
	Vector2 aCenter = m_pInterface->Agent_GetInfo().Position;

	Vector2 Orientation = OrientationToVector(m_pInterface->Agent_GetInfo().Orientation);
	Vector2 aDirection = m_pInterface->Agent_GetInfo().LinearVelocity;
	float shootingRange = m_pInterface->Agent_GetInfo().FOV_Range;
	Vector2 shot = aDirection * shootingRange;

	m_pInterface->Draw_Segment(aCenter, shot, { 1, 0,0 });

	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Target, .7f, { 0,0 }, { 1, 0, 0 });
}

vector<HouseInfo> Plugin::GetHousesInFOV()
{
	vector<HouseInfo> vHousesInFOV = {};

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{
			m_HousesFOV.push_back(hi);
			vHousesInFOV.push_back(hi);
			//m_pAgent->AddHouse(hi);
			continue;
		}

		break;
	}
	//m_HousesFOV = vHousesInFOV;
	return vHousesInFOV;
}

vector<EntityInfo> Plugin::GetEntitiesInFOV()
{
	vector<EntityInfo> vEntitiesInFOV = {};

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetEntityByIndex(i, ei))
		{
			if (ei.Type == eEntityType::ITEM)
			{
				m_ItemsFOV.push_back(ei);
			}
			if (ei.Type == eEntityType::ENEMY)
			{
				m_EnemiesFOV.push_back(ei);
			}
			vEntitiesInFOV.push_back(ei);
			continue;
		}

		break;
	}

	return vEntitiesInFOV;
}






void Plugin::CreateBehaviorTree()
{
	m_pBehaviorTree = new BehaviorTree(m_pBlackBoard,
		new BehaviorSelector(
			{
				new BehaviorSequence(
				{
					new BehaviorConditional(BT_Conditions::IsItemEmpty),
					new BehaviorAction(BT_Actions::RemoveEmpty)
				}),

				new BehaviorSequence(
				{
						new BehaviorConditional(BT_Conditions::IsEnemyNearBy),
						new BehaviorConditional(BT_Conditions::HasGun),
						new BehaviorAction(BT_Actions::ChangeToFace),
						new BehaviorSelector
						({
							new BehaviorSequence
							({
									new BehaviorConditional(BT_Conditions::EnemyInSight),
									new BehaviorAction(BT_Actions::ShootEnemy),
							}),
							new BehaviorAction(BT_Actions::FAKE),
						}),
				}),


		/*		new BehaviorSequence(
				{
						new BehaviorConditional(BT_Conditions::IsEnemyNearBy),
						new BehaviorSequence(
						{
								new BehaviorConditional(BT_Conditions::HasGun),
								new BehaviorAction(BT_Actions::ChangeToFace),
									new BehaviorSequence(
									{
										new BehaviorConditional(BT_Conditions::EnemyInSight),
										new BehaviorAction(BT_Actions::ShootEnemy)
									}),
						}),
				}),*/

				new BehaviorSequence(
				{
					new BehaviorConditional(BT_Conditions::IsItemNearby),
					new BehaviorAction(BT_Actions::ChangeToSeek),
					new BehaviorAction(BT_Actions::AddItem)
				}),

				new BehaviorSequence(
				{
						new BehaviorConditional(BT_Conditions::IsEnergLow),
						new BehaviorAction(BT_Actions::UseFood)
				}),

				new BehaviorSequence(
				{
						new BehaviorConditional(BT_Conditions::IsHealthLow),
						new BehaviorAction(BT_Actions::UseHealth)
				}),


				new BehaviorSequence(
				{
					new BehaviorConditional(BT_Conditions::NeedsVisiting),
					new BehaviorAction(BT_Actions::ChangeToSeek),
					new BehaviorConditional(BT_Conditions::IsInsideHouse)
				}),




//				new BehaviorSequence(
//				{
//						new BehaviorSequence(
//						{
//								new BehaviorConditional(BT_Conditions::IsEnemyNearBy),
//									new BehaviorSelector(
//										{
//											new BehaviorSequence(
//											{
//													new BehaviorSequence(
//													{
//														new BehaviorConditional(BT_Conditions::HasPistol),
//														new BehaviorAction(BT_Actions::ChangeToFace)
//													}),
//
//													new BehaviorConditional(BT_Conditions::EnemyInSight),
//													new BehaviorAction(BT_Actions::ShootEnemy)
//											}),
///*											new BehaviorSequence(
//											{
//													new BehaviorConditional(BT_Conditions::EnemyInSight),
//													new BehaviorAction(BT_Actions::ShootEnemy)
//											}),	*/	
//										}),
//										new BehaviorAction(BT_Actions::ChangeToFlee),
//						}),
//				}),
				new BehaviorSequence(
				{
					new BehaviorConditional(BT_Conditions::IsHouseSeen),
					new BehaviorAction(BT_Actions::AddHouse)
				}),

				new BehaviorSequence(
				{
					new BehaviorConditional(BT_Conditions::Explore),
					new BehaviorAction(BT_Actions::ChangeToSeek),
					new BehaviorConditional(BT_Conditions::IsAtPoint),
				}),
				//new BehaviorAction(BT_Actions::ChangeToWander)
			}));
}