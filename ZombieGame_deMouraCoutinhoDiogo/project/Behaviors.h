#pragma once
#include "Blackboard.h"
#include "EBehaviorTree.h"
#include <vector>

	namespace BT_Actions
	{
		//Elite::BehaviorState AddHouse(BlackBoard* pBlackBoard)
		//{
		//	return 	Elite::BehaviorState::Success;
		//}

		BehaviorState ChangeToWander(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			SteeringPlugin_Output steering;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			pBlackBoard->GetData("SteeringOutput", steering);
			pAgent->SetWander(steering);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

		BehaviorState ChangeToFace(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			SteeringPlugin_Output steering;
			vector<EntityInfo> enemies;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			pBlackBoard->GetData("SteeringOutput", steering);
			pBlackBoard->GetData("EnemiesFOV", enemies);
			if (enemies.size() == 0)
			{
				return BehaviorState::Failure;
			}


			pAgent->SetFace(steering, enemies[0]);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

		BehaviorState ChangeToPoint(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			SteeringPlugin_Output steering;
			Vector2 Look;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			pBlackBoard->GetData("SteeringOutput", steering);
			pBlackBoard->GetData("LookAt", Look);

			pAgent->SetFace(steering, Look);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

		BehaviorState GrabItem(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<EntityInfo> item;
			IExamInterface* pInterface;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("ItemsFOV", item))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return BehaviorState::Failure;
			}
			if (item[0].Type == eEntityType::ITEM)
			{
				ItemInfo info;
				pInterface->Item_Grab(item[0], info);
				pInterface->Inventory_AddItem(0, info);
			}
			return BehaviorState::Failure;
		}



		BehaviorState ChangeToEvadeBorder(Blackboard* pBlackBoard)
		{
			return 	BehaviorState::Success;
		}



		BehaviorState ChangeToFlee(Blackboard* pBlackBoard)
		{
			return 	BehaviorState::Success;
		}

		BehaviorState ChangeToSeek(Blackboard* pBlackBoard)
		{
			return 	BehaviorState::Success;
		}



		BehaviorState ChangeToPursuit(Blackboard* pBlackBoard)
		{
			return 	BehaviorState::Success;
		}
	}

	namespace BT_Conditions
	{
		bool IsItemNearby(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<EntityInfo> items;
			IExamInterface* pInterface;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{
				return false;
			}
			if (!pBlackBoard->GetData("ItemsFOV", items))
			{
				return false;
			}
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return false;
			}

			if (items.size() > 0)
			{
				if (items[0].Type == eEntityType::ITEM)
				{
					return true;
				}
			}

			return false;



		}


		bool IsEnemyNearBy(Blackboard* pBlackBoard)
		{
		
			return false;
		}

		bool IsEnemyNearBySmaller(Blackboard* pBlackBoard)
		{
			return false;
		}

		bool IsEnemyNearByLarger(Blackboard* pBlackBoard)
		{
			return false;
		}


		bool IsFoodNearBy(Blackboard* pBlackBoard)
		{

			return false;
		}

		bool IsFoodNearByClose(Blackboard* pBlackBoard)
		{
			return false;
		}


		bool IsAgentNearBorder(Blackboard* pBlackBoard)
		{

			return false;
		}
	}