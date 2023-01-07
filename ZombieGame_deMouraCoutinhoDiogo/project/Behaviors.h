#pragma once
#include "Blackboard.h"
#include "EBehaviorTree.h"
#include <vector>
#include "Inventory.h"

	namespace BT_Actions
	{
		BehaviorState FAKE(Blackboard* pBlackBoard)
		{
			return 	BehaviorState::Success;
		}

#pragma region Steering
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
		//Steering Behaviours
		BehaviorState ChangeToFace(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			SteeringPlugin_Output steering;
			Vector2 enemyLocation;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("SteeringOutput", steering))
			{
				return BehaviorState::Failure;

			}
			if (!pBlackBoard->GetData("Face", enemyLocation))
			{
				return BehaviorState::Failure;

			}

			pAgent->SetEnemyPosition(enemyLocation);
			pAgent->SetFace(steering);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

		BehaviorState ChangeToSeek(Blackboard* pBlackBoard)
		{
			IExamInterface* pInterface;
			Vector2 seek;
			SteeringPlugin_Output steering;
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("Seek", seek))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("SteeringOutput", steering))
			{
				return BehaviorState::Failure;

			}

			pAgent->SetSeekPoint(seek);
			pAgent->SetSeek(steering);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

		BehaviorState ChangeToFlee(Blackboard* pBlackBoard)
		{
			IExamInterface* pInterface;
			Vector2 flee;
			SteeringPlugin_Output steering;
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("Face", flee))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("SteeringOutput", steering))
			{
				return BehaviorState::Failure;

			}

			pAgent->SetEnemyPosition(flee);
			pAgent->SetFlee(steering);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

#pragma endregion

#pragma region Inventory
		BehaviorState AddItem(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<EntityInfo> itemsFOV;
			IExamInterface* pInterface;
			Inventory* inventory;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("ItemsFOV", itemsFOV))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("Inventory", inventory))
			{
				return BehaviorState::Failure;
			}


			ItemInfo itemInfo{};
			EntityInfo currentEntity = itemsFOV[0];
			if (pInterface->Item_GetInfo(currentEntity, itemInfo))
			{
				if (inventory->AddItem(itemInfo, currentEntity, pInterface))
				{
					pBlackBoard->ChangeData("Inventory", inventory);
					return BehaviorState::Success;
				}
			}
			return 	BehaviorState::Success;
		}

		BehaviorState RemoveEmpty(Blackboard* pBlackBoard)
		{
			IExamInterface* pInterface;
			Inventory* inventory;
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("Inventory", inventory))
			{
				return BehaviorState::Failure;
			}
			if (inventory->RemoveEmpty(pInterface))
			{
				return BehaviorState::Success;
			}
			return BehaviorState::Failure;
		}



#pragma endregion

#pragma region Houses

		BehaviorState AddHouse(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<HouseInfo> housesFOV;
			//IExamInterface* pInterface;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("HousesFOV", housesFOV))
			{
				return BehaviorState::Failure;
			}
			vector<VisitedHouse>& registeredHouses = agent->GetRegisteredHouses();


			for (auto& houseSeen : housesFOV) //go through all the seen houses from the blackbaord
			{
				auto houseExists = [&houseSeen](const VisitedHouse& houseRecorded) { return houseSeen.Center == houseRecorded.Center; }; //lamba to check each individual house 
																																	// from the blackboard agaisnt the ones from the 
																																	//agent
				auto house = std::find_if(registeredHouses.begin(), registeredHouses.end(), houseExists);

				if (house == std::end(registeredHouses))
				{
					agent->RegisterHouse(houseSeen);
					//pBlackBoard->ChangeData("RegisteredHouses", registeredHouses);
					return 	BehaviorState::Success;
				}
			}
			return BehaviorState::Failure;
		}

		BehaviorState SearchHouse(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<HouseInfo> bbHouses;
			//IExamInterface* pInterface;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("HousesFOV", bbHouses))
			{
				return BehaviorState::Failure;
			}
			vector<VisitedHouse> registeredHouses = agent->GetRegisteredHouses();
			if (registeredHouses.size() == 0)
			{
				return BehaviorState::Failure;
			}

			return 	BehaviorState::Success;
		}



#pragma endregion

#pragma region ItemAction
		BehaviorState ShootEnemy(Blackboard* pBlackBoard)
		{
			IExamInterface* pInterface;
			Inventory* inventory;
			bool mutiple;
			if(!pBlackBoard->GetData("pInterface", pInterface))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("Inventory", inventory))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("MultipleEnemies", mutiple))
			{
				return BehaviorState::Failure;
			}
			if (mutiple)
			{
				ItemInfo shotGun;
				shotGun.Type = eItemType::SHOTGUN;
				if (inventory->UseType(pInterface, shotGun))
				{
					return 	BehaviorState::Success;
				}
				return BehaviorState::Failure;
			}
			else
			{
				ItemInfo pistol;
				pistol.Type = eItemType::PISTOL;
				if (inventory->UseType(pInterface, pistol))
				{
					return 	BehaviorState::Success;
				}
				return BehaviorState::Failure;
			}
		}

		BehaviorState UseHealth(Blackboard* pBlackBoard)
		{
			IExamInterface* pInterface;
			Inventory* inventory;
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("Inventory", inventory))
			{
				return BehaviorState::Failure;
			}

			ItemInfo health;
			health.Type = eItemType::MEDKIT;
			if (inventory->UseType(pInterface, health))
			{
				return 	BehaviorState::Success;
			}
			return BehaviorState::Failure;
		}

		BehaviorState UseFood(Blackboard* pBlackBoard)
		{
			IExamInterface* pInterface;
			Inventory* inventory;
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("Inventory", inventory))
			{
				return BehaviorState::Failure;
			}

			ItemInfo food;
			food.Type = eItemType::FOOD;
			if (inventory->UseType(pInterface, food))
			{
				return 	BehaviorState::Success;
			}
			return BehaviorState::Failure;
		}



#pragma endregion 


	}

	namespace BT_Conditions
	{


#pragma region HasItemn?

		bool NeedsVisiting(Blackboard* pBlackBoard)
		{
			Agent* agent;
			Vector2 seekPoint;
			if (!pBlackBoard->GetData("Agent", agent))
			{
				return false;
			}
			if (!pBlackBoard->GetData("Seek", seekPoint))
			{
				return false;
			}
			for (auto& house : agent->GetRegisteredHouses())
			{
				if (house.needRevisiting)
				{
					seekPoint = house.Center;
					pBlackBoard->ChangeData("Seek", seekPoint);
					pBlackBoard->ChangeData("TargetHouse", &house);
					return true;
				}
			}
			return false;
		}

		bool Explore(Blackboard* pBlackBoard)
		{
			Vector2 seekPoint;
			Agent* agent;
			if (!pBlackBoard->GetData("Seek", seekPoint))
			{
				return false;
			}
			if (!pBlackBoard->GetData("Agent", agent))
			{
				return false;
			}

			auto& pointsVector = agent->GetExplorationPoints();

			if (pointsVector.size() == 0)
				return false;

			pBlackBoard->ChangeData("Seek", pointsVector[pointsVector.size()-1]);
			return true;
		}

		bool IsAtPoint(Blackboard* pBlackBoard)
		{
			Vector2 seekPoint;
			Agent* agent;
			AgentInfo agentInfo;
			if (!pBlackBoard->GetData("Seek", seekPoint))
			{
				return false;
			}
			if (!pBlackBoard->GetData("Agent", agent))
			{
				return false;
			}
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}

			Vector2 aLocation = agentInfo.Position;
			float maxDist = 10.0f;
			float currDist = DistanceSquared(aLocation, seekPoint);
			if (currDist < maxDist)
			{
				agent->GetExplorationPoints().pop_back();
				return true;
			}
			return true;
		}




		bool IsInsideHouse(Blackboard* pBlackBoard)
		{
			Vector2 seekPoint;
			VisitedHouse* singleHouse;
			AgentInfo agentInfo;
			if (!pBlackBoard->GetData("Seek", seekPoint))
			{
				return false;
			}
			if (!pBlackBoard->GetData("TargetHouse", singleHouse))
			{
				return false;
			}
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}

			float distance = DistanceSquared(agentInfo.Position, seekPoint);
			float maxDistance = 5.0f;

			if (distance < maxDistance)
			{
				singleHouse->needRevisiting = false;
			}
			return true;
		}

		bool IsHealthLow(Blackboard* pBlackBoard)
		{
			AgentInfo agentInfo;
			float minimumHealth;

			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}
			if (!pBlackBoard->GetData("MinimumHealth", minimumHealth))
			{
				return false;
			}

			if (agentInfo.Health < minimumHealth)
			{
				Inventory* inventory;
				ItemInfo healthCheck{};
				healthCheck.Type = eItemType::MEDKIT;
				if (!pBlackBoard->GetData("Inventory", inventory))
				{
					return false;
				}
				return inventory->HasType(healthCheck);
			}
			return false;
		}

		bool IsEnergLow(Blackboard* pBlackBoard)
		{
			AgentInfo agentInfo;
			float minimumEnergy;

			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}
			if (!pBlackBoard->GetData("MinimumHealth", minimumEnergy))
			{
				return false;
			}

			if (agentInfo.Energy < minimumEnergy)
			{
				Inventory* inventory;
				ItemInfo energyCheck{};
				energyCheck.Type = eItemType::FOOD;
				if (!pBlackBoard->GetData("Inventory", inventory))
				{
					return false;
				}
				return inventory->HasType(energyCheck);
			}
			return false;
		}

		bool HasGun(Blackboard* pBlackBoard)
		{
			Inventory* inventory;
			if (!pBlackBoard->GetData("Inventory", inventory))
			{
				return false;
			}

			ItemInfo pistolCheck{};
			pistolCheck.Type = eItemType::PISTOL;
			bool hasPistol = inventory->HasType(pistolCheck);

			ItemInfo shotGunCheck{};
			shotGunCheck.Type = eItemType::SHOTGUN;
			bool hasShotGun = inventory->HasType(shotGunCheck);

			return (hasPistol || hasShotGun);
		}

#pragma endregion



		bool IsItemNearby(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<EntityInfo> items;
			IExamInterface* pInterface;
			Vector2 seek;
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
			if (!pBlackBoard->GetData("Seek", seek))
			{
				return false;
			}

			if (items.size() > 0)
			{
				if (items[0].Type == eEntityType::ITEM)
				{
					seek = items[0].Location;
					if (pBlackBoard->ChangeData("Seek", seek))
					{
						return true;
					}
				}
			}
			return false;
		}

		bool CheckHouse(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<HouseInfo> bbHouses;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{

			}
			if (!pBlackBoard->GetData("HousesFOV", bbHouses))
			{

			}
			return false;
		}


		bool IsItemEmpty(Blackboard* pBlackBoard)
		{
			Inventory* pInventory;
			IExamInterface* pInterface;
			Vector2 seek;
			if (!pBlackBoard->GetData("Inventory", pInventory))
			{
				return false;
			}
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return false;
			}
			return pInventory->IsEmpty(pInterface);
		}

		bool IsHouseSeen(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<HouseInfo> houses;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{
				return false;
			}
			if (!pBlackBoard->GetData("HousesFOV", houses))
			{
				return false;
			}
			return(houses.size() > 0);
		}


		bool IsEnemyNearBy(Blackboard* pBlackBoard)
		{
			Agent* agent;
			vector<EntityInfo> enemies;
			IExamInterface* pInterface;
			Vector2 enemyLocation;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{
				return false;
			}
			if (!pBlackBoard->GetData("EnemiesFOV", enemies))
			{
				return false;
			}
			if (!pBlackBoard->GetData("pInterface", pInterface))
			{
				return false;
			}
			if (!pBlackBoard->GetData("Face", enemyLocation))
			{
				return false;
			}
			if (enemies.size() > 0)
			{
				if (enemies.size() < 2)
				{
					if (enemies[0].Type == eEntityType::ENEMY)
					{
						bool multiple = false;
						enemyLocation = enemies[0].Location;
						pBlackBoard->ChangeData("Face", enemyLocation);
						pBlackBoard->ChangeData("MultipleEnemies", multiple);
						return true;
					}
				}
				if (enemies[0].Type == eEntityType::ENEMY)
				{
					bool multiple = true;
					enemyLocation = enemies[0].Location;
					pBlackBoard->ChangeData("Face", enemyLocation);
					pBlackBoard->ChangeData("MultipleEnemies", multiple);
					return true;
				}


			}

			return false;
		}

		bool EnemyInSight(Blackboard* pBlackBoard)
		{
			Agent* agent;
			AgentInfo agentInfo;
			vector<EntityInfo> enemies;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
			{
				return false;
			}
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}
			if (!pBlackBoard->GetData("EnemiesFOV", enemies))
			{
				return false;
			}
			if (enemies.size() == 0)
			{
				return false;
			}

			EntityInfo enemy = enemies[0];


			const auto agentPos{ agentInfo.Position };
			const auto enemyPos{ enemy.Location };
			const auto vecToEnemy{ enemyPos - agentPos };
			const float angleToEnemy{ atan2f(vecToEnemy.y, vecToEnemy.x) };
			const float angleOfAgent{ agentInfo.Orientation };

			if (abs(angleToEnemy - angleOfAgent) < 0.05f)
			{
				return true;
			}

			return false;
		}
	}