#pragma once
#include "Blackboard.h"
#include "EBehaviorTree.h"
#include <vector>
#include "Inventory.h"

	namespace BT_Actions
	{


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

		BehaviorState ChangeToTurn(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			SteeringPlugin_Output steering;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			
			if (!pBlackBoard->GetData("SteeringOutput", steering))
			{
				return BehaviorState::Failure;

			}

			pAgent->SetTurn(steering);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

		BehaviorState ChangeToSeek(Blackboard* pBlackBoard)
		{
			SteeringPlugin_Output steering;
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("SteeringOutput", steering))
			{
				return BehaviorState::Failure;

			}
			auto seekPoint = pAgent->GetSeekPoint();
			pAgent->SetSeekPoint(seekPoint);
			pAgent->SetSeek(steering);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

		BehaviorState ChangeToFlee(Blackboard* pBlackBoard)
		{

			Vector2 flee;
			SteeringPlugin_Output steering;
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
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
			pAgent->SetFlee(steering);
			pBlackBoard->ChangeData("SteeringOutput", steering);
			return 	BehaviorState::Success;
		}

#pragma endregion

#pragma region Inventory
		BehaviorState AddItem(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			vector<EntityInfo> itemsFOV;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			if (!pBlackBoard->GetData("ItemsFOV", itemsFOV))
			{
				return BehaviorState::Failure;
			}

			auto inventory = pAgent->GetInventory();
			auto pInterface = pAgent->GetInterface();

			ItemInfo itemInfo{};
			EntityInfo currentEntity = itemsFOV[itemsFOV.size()-1];
			if (pInterface->Item_GetInfo(currentEntity, itemInfo))
			{
				if (inventory->AddItem(itemInfo, currentEntity, pInterface))
				{
					return BehaviorState::Success;
				}
			}
			return 	BehaviorState::Success;
		}

		BehaviorState RemoveEmpty(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return BehaviorState::Failure;
			}
			auto pInventory = pAgent->GetInventory();
			auto pInterface = pAgent->GetInterface();
			if (pInventory->RemoveEmpty(pInterface))
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
				//lamba to check each individual house 
				// from the blackboard agaisnt the ones from the 
				//agent
				auto houseExists = [&houseSeen](const VisitedHouse& houseRecorded) { return houseSeen.Center == houseRecorded.center; }; 
				auto house = std::find_if(registeredHouses.begin(), registeredHouses.end(), houseExists);

				if (house == std::end(registeredHouses))
				{
					agent->RegisterHouse(houseSeen);
					return 	BehaviorState::Success;
				}
			}
			return BehaviorState::Failure;
		}

		BehaviorState SearchHouse(Blackboard* pBlackBoard)
		{
			Agent* agent;
			if (!pBlackBoard->GetData("Agent", agent) || agent == nullptr)
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
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			bool mutiple{};
			if (!pBlackBoard->GetData("MultipleEnemies", mutiple))
			{
				return BehaviorState::Failure;
			}

			auto pInventory = pAgent->GetInventory();
			auto pInterface = pAgent->GetInterface();


			if (mutiple)//if there are more than one enemy in the fov, use shotgun
			{
				ItemInfo shotGun;
				shotGun.Type = eItemType::SHOTGUN;
				if (pInventory->UseType(pInterface, shotGun))
				{
					return 	BehaviorState::Success;
				}
				//if there are multiple but doesnt have a shotgun
				ItemInfo pistol;
				pistol.Type = eItemType::PISTOL;
				if (pInventory->UseType(pInterface, shotGun))
				{
					return 	BehaviorState::Success;
				}
				return BehaviorState::Success;
			}
			else
			{	
				//if there not not multiple enmies
				//use pistols
				ItemInfo pistol;
				pistol.Type = eItemType::PISTOL;
				if (pInventory->UseType(pInterface, pistol))
				{
					return 	BehaviorState::Success;
				}
				//if there is no pistol
				// use shotgun
				ItemInfo shotGun;
				shotGun.Type = eItemType::SHOTGUN;
				if (pInventory->UseType(pInterface, shotGun))
				{
					return 	BehaviorState::Success;
				}
				return BehaviorState::Failure;
			}
		}

		BehaviorState UseHealth(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			auto pInventory = pAgent->GetInventory();
			auto pInterface = pAgent->GetInterface();

			ItemInfo health;
			health.Type = eItemType::MEDKIT;
			if (pInventory->UseType(pInterface, health))
			{
				return 	BehaviorState::Success;
			}
			return BehaviorState::Failure;
		}

		BehaviorState UseFood(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return BehaviorState::Failure;
			}
			auto pInventory = pAgent->GetInventory();
			auto pInterface = pAgent->GetInterface();

			ItemInfo food;
			food.Type = eItemType::FOOD;
			if (pInventory->UseType(pInterface, food))
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

		bool IsHealthLow(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}

			AgentInfo agentInfo;
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}


			if (agentInfo.Health < pAgent->GetMinimumHealth())
			{
				auto inventory = pAgent->GetInventory();
				ItemInfo healthCheck{};
				healthCheck.Type = eItemType::MEDKIT;
				return inventory->HasType(healthCheck);
			}
			return false;
		}

		bool IsEnergLow(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}

			AgentInfo agentInfo;
			
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}


			if (agentInfo.Energy < pAgent->GetMinimumEnergy())
			{
				auto inventory = pAgent->GetInventory();
				ItemInfo energyCheck{};
				energyCheck.Type = eItemType::FOOD;
				return inventory->HasType(energyCheck);
			}
			return false;
		}

		bool HasGun(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}
			auto inventory = pAgent->GetInventory();

			ItemInfo pistolCheck{};
			pistolCheck.Type = eItemType::PISTOL;
			bool hasPistol = inventory->HasType(pistolCheck);

			ItemInfo shotGunCheck{};
			shotGunCheck.Type = eItemType::SHOTGUN;
			bool hasShotGun = inventory->HasType(shotGunCheck);

			return (hasPistol || hasShotGun);
		}

		bool HasNoGun(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}
			Vector2 enemyLocation;
			if (!pBlackBoard->GetData("Face", enemyLocation))
			{
				return false;
			}
			auto inventory = pAgent->GetInventory();

			ItemInfo pistolCheck{};
			pistolCheck.Type = eItemType::PISTOL;
			bool hasPistol = inventory->HasType(pistolCheck);

			ItemInfo shotGunCheck{};
			shotGunCheck.Type = eItemType::SHOTGUN;
			bool hasShotGun = inventory->HasType(shotGunCheck);

			if (hasPistol || hasShotGun)
			{
				return false;
			}
			Vector2 empty{};
			if (empty == enemyLocation)
			{
				return false;
			}
			pAgent->SetFleePoint(enemyLocation);
			return true;
		}


#pragma endregion
		
		bool AlwaysTrue(Blackboard* pBlackBoard)
		{
			return true;
		}

		bool IsItemNearby(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			vector<EntityInfo> items;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return false;
			}
			if (!pBlackBoard->GetData("ItemsFOV", items))
			{
				return false;
			}
			auto seekPoint = pAgent->GetSeekPoint();
			auto pInterface = pAgent->GetInterface();

			if (items.size() > 0)
			{
				if (items[items.size() - 1].Type == eEntityType::ITEM)
				{
					pAgent->SetSeekPoint(items[items.size()-1].Location);
					return true;
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
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return false;
			}
			auto pInterface = pAgent->GetInterface();
			auto pInventory = pAgent->GetInventory();
			return pInventory->IsEmpty(pInterface);
		}

		bool IsHouseSeen(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			vector<HouseInfo> houses;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
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
			Agent* pAgent;
			vector<EntityInfo> enemies;
			Vector2 enemyLocation;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return false;
			}
			if (!pBlackBoard->GetData("EnemiesFOV", enemies))
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

		bool InPurge(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			AgentInfo agentInfo;
			vector<EntityInfo> purgeFOV;
			PurgeZoneInfo purgeZoneInfo;
			vector<Vector2> purgeRunPoints;
			bool inPurge;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return false;
			}
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}
			if (!pBlackBoard->GetData("PurgeFOV", purgeFOV))
			{
				return false;
			}
			if (!pBlackBoard->GetData("InPurge", inPurge))
			{
				return false;
			}
			if(!pBlackBoard->GetData("PurgePoints", purgeRunPoints))
			{
				return false;
			}

			if (purgeFOV.size() == 0)
				return false;

			auto pInterface = pAgent->GetInterface();
			pInterface->PurgeZone_GetInfo(purgeFOV[0], purgeZoneInfo);

			Vector2 center = purgeZoneInfo.Center;
			float radius = purgeZoneInfo.Radius;
			float offset = 30.0f;

			float nrOfPoints{ 10 };
			float intervals{ static_cast<float>(M_PI) * 2 / nrOfPoints };

			for (int nr{ 0 }; nr < nrOfPoints; ++nr)
			{
				Vector2 angle{ cosf(intervals * nr), sinf(intervals * nr) };
				Vector2 point = angle * (radius + offset) + center;
				purgeRunPoints.push_back(point);
			}

			agentInfo.RunMode = true;
			AgentInfo newAgentInfo = agentInfo;
			pBlackBoard->ChangeData("AgentInfo", agentInfo);
			pBlackBoard->ChangeData("PurgePoints", purgeRunPoints);
			pBlackBoard->ChangeData("InPurge", true);
			return true;
		}

		bool StillInPurge(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			AgentInfo agentInfo;
			vector<EntityInfo> purgeFOV;
			PurgeZoneInfo purgeZoneInfo;
			bool inPurge;
			vector<Vector2> purgeRunPoints;

			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return false;
			}
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}
			if (!pBlackBoard->GetData("InPurge", inPurge))
			{
				return false;
			}
			if (!pBlackBoard->GetData("PurgePoints", purgeRunPoints))
			{
				return false;
			}
			if (inPurge)
			{
				float currentDistance{ 999999 };
				int currentPoint{ 0 };
				for (size_t pos{ 0 }; pos < purgeRunPoints.size(); ++pos)
				{
					auto distance = DistanceSquared(agentInfo.Position, purgeRunPoints[pos]);
					if (distance < currentDistance)
					{
						currentDistance = distance;
						currentPoint = pos;
					}
				}

				Vector2 closestPoint = purgeRunPoints[currentPoint];
				pAgent->SetSeekPoint(closestPoint);
				auto distance = DistanceSquared(agentInfo.Position, closestPoint);
				if (distance < 25.0f)
				{

					agentInfo.RunMode = false;
					AgentInfo newAgentInfo = agentInfo;
					pBlackBoard->ChangeData("AgentInfo", agentInfo);
					pBlackBoard->ChangeData("InPurge", false);
					purgeRunPoints.clear();
					pBlackBoard->ChangeData("PurgePoints", purgeRunPoints);
					return false;
				}
				return true;
			}



			return false;
		}

		bool AwayFromEnemy(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}
			AgentInfo agent;
			if (!pBlackBoard->GetData("AgentInfo", agent))
			{
				return false;
			}

			auto aLocation = agent.Position;
			auto enemyLocation = pAgent->GetFleePoint();
			auto distance = DistanceSquared(agent.Position, enemyLocation);
			float safetyRadius{ 10.0f };
			if (distance > (safetyRadius * safetyRadius))
			{
				return false;
			}
			if (distance < (safetyRadius * safetyRadius))
			{
				return true;
			}
			return false;

		}

		bool EnemyInSight(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			AgentInfo agentInfo;
			vector<EntityInfo> enemies;
			Vector2 face;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
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
			if (!pBlackBoard->GetData("Face", face))
			{
				return false;
			}
			EntityInfo enemy = enemies[0];
			Vector2 agentPos{ agentInfo.Position };
			Vector2 enemyPos{ face };
			Vector2 agentToEnemy{ (enemyPos - agentPos) };
			float angleToEnemy{ atan2f(agentToEnemy.y, agentToEnemy.x) };

			float difference = abs(angleToEnemy - agentInfo.Orientation);
			if (difference <= 0.03f)
			{
				return true;
			}
			return false;
		}

		bool PlayerBitten(Blackboard* pBlackBoard)
		{
			AgentInfo agentInfo;
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
			{
				return false;
			}
			//bool bittenAgent;
			//if (!pBlackBoard->GetData("BittenAgent", bittenAgent))
			//{
			//	return false;
			//}
			bool bittenMine;
			if (!pBlackBoard->GetData("BittenMine", bittenMine))
			{
				return false;
			}
			float time;
			if (!pBlackBoard->GetData("TimeBitten", time)) 
			{
				return false;
			}

			if (!pBlackBoard->GetData("BittenMine", bittenMine))
			{
				return false;
			}



			if (agentInfo.WasBitten || bittenMine)
			{
				bittenMine = true;
				pBlackBoard->ChangeData("BittenMine", bittenMine);
				if (time > 5.0f)
				{
					bittenMine = false;
					time = 0;
					pBlackBoard->ChangeData("BittenMine", bittenMine);
					pBlackBoard->ChangeData("TimeBitten", time);
					return false;
				}
				return true;
			}
			return false;
		}

		bool NeedsVisiting(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}
			for (auto& house : pAgent->GetRegisteredHouses())
			{
				if (house.needRevisiting)
				{
					Vector2	seekPoint = house.center;
					pAgent->SetSeekPoint(seekPoint);

					pBlackBoard->ChangeData("TargetHouse", &house);
					return true;
				}
			}
			return false;
		}

		bool HasToExplore(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}
			auto& pointsVector = pAgent->GetExplorationPoints();

			if (pointsVector.size() == 0)
				return false;
			pAgent->SetSeekPoint(pointsVector[pointsVector.size() - 1]);
			return true;
		}

		bool IsAtPoint(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			AgentInfo agentInfo;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}
			auto seekPoint = pAgent->GetSeekPoint();


			Vector2 aLocation = agentInfo.Position;
			float maxDist = 10.0f;
			float currDist = DistanceSquared(aLocation, seekPoint);
			if (currDist < maxDist)
			{
				pAgent->GetExplorationPoints().pop_back();
				return true;
			}
			return true;
		}

		bool IsInsideHouse(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}
			auto seekPoint = pAgent->GetSeekPoint();

			VisitedHouse* singleHouse;
			AgentInfo agentInfo;
			if (!pBlackBoard->GetData("TargetHouse", singleHouse))
			{
				return false;
			}
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}

			float distance = DistanceSquared(agentInfo.Position, seekPoint);
			float maxDistance = 1.0f;

			if (distance < maxDistance)
			{
				singleHouse->needRevisiting = false;
			}
			return true;
		}

		bool AllPointsSeached(Blackboard* pBlackBoard)
		{
			Agent* pAgent;
			if (!pBlackBoard->GetData("Agent", pAgent))
			{
				return false;
			}
			auto seekPoint = pAgent->GetSeekPoint();

			VisitedHouse* singleHouse;
			AgentInfo agentInfo;
			if (!pBlackBoard->GetData("TargetHouse", singleHouse))
			{
				return false;
			}
			if (!pBlackBoard->GetData("AgentInfo", agentInfo))
			{
				return false;
			}
			
			auto points = singleHouse->GetHousePoints();
			if (points.size() == 0)
			{
				singleHouse->needRevisiting = false;
				return false;
			}
			auto pos = points.size() -1;

			float distance = DistanceSquared(agentInfo.Position, points[pos]);
			float maxDistance = (agentInfo.FOV_Range) * (agentInfo.FOV_Range);


			if (distance > maxDistance)
			{
				pAgent->SetSeekPoint(points[pos]);
				return true;
			}
			if (distance < maxDistance)
			{
				//pAgent->SetSeekPoint(points[pos]);
				singleHouse->GetHousePoints().pop_back();
				return true;
			}

			return false;
		}

	}