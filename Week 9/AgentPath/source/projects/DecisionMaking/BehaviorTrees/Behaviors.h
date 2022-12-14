/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for the BT version of the Agario Game
/*=============================================================================*/
#ifndef ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
#define ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteMath/EMath.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteBehaviorTree/EBehaviorTree.h"
#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------

namespace BT_Actions
{
	Elite::BehaviorState ChangeToWander(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pAgent->SetToWander();
		return 	Elite::BehaviorState::Success;
	}

	Elite::BehaviorState ChangeToEvadeBorder(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		bool isValid = pBlackBoard->GetData("Agent", pAgent);
		if (isValid == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}


		float* trimWorld;
		isValid = pBlackBoard->GetData("BorderSize", trimWorld);
		if (isValid == false || trimWorld == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}


		Elite::Vector2 agentPos{ pAgent->GetPosition() };

		Elite::Vector2 agentDirection{ pAgent->GetDirection() };
		float trimWorldSize = *trimWorld;
		pAgent->SetToSeek(Elite::Vector2{ trimWorldSize / 2, trimWorldSize / 2 });

		std::cout << "Evading Border " << '\n';
	}



	Elite::BehaviorState ChangeToFlee(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		Elite::Vector2 targetPos;

		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackBoard->GetData("EnemyTargetFlee", targetPos))
		{
			return Elite::BehaviorState::Failure;
		}

		pAgent->SetToFlee(targetPos);
		return 	Elite::BehaviorState::Success;
	}

	Elite::BehaviorState ChangeToSeek(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		Elite::Vector2 targetPos;

		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackBoard->GetData("Target", targetPos))
		{
			return Elite::BehaviorState::Failure;
		}

		pAgent->SetToSeek(targetPos);
		return 	Elite::BehaviorState::Success;
	}



	Elite::BehaviorState ChangeToPursuit(Elite::Blackboard* pBlackBoard)
	{
		AgarioAgent* pAgent;
		Elite::Vector2 targetPos;

		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (!pBlackBoard->GetData("EnemyTarget", targetPos))
		{
			return Elite::BehaviorState::Failure;
		}

		pAgent->SetToSeek(targetPos);
		return 	Elite::BehaviorState::Success;
	}
}

namespace BT_Conditions
{
	AgarioAgent* pAgent;
	std::vector<AgarioFood*>* pFoodVec;
	std::vector<AgarioAgent*>* pEnemiesVec;

	float* trimWorld;


	bool IsEnemyNearBy(Elite::Blackboard* pBlackBoard)
	{
		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return false;
		}
		if (!pBlackBoard->GetData("AgentsVec", pEnemiesVec) || pEnemiesVec == nullptr)
		{
			return false;
		}

		const float nearRadius{ 100.0f + pAgent->GetRadius() };


		Elite::Vector2 agentPos{ pAgent->GetPosition() };
		auto elemnetdis = [agentPos](AgarioAgent* pEnemy1, AgarioAgent* pEnemy2)
		{
			float dist1 = agentPos.DistanceSquared(pEnemy1->GetPosition());
			float dist2 = agentPos.DistanceSquared(pEnemy2->GetPosition());
			return dist1 < dist2;
		};

		auto closestEnemy = std::min_element(pEnemiesVec->begin(), pEnemiesVec->end(), elemnetdis);


		if (closestEnemy != pEnemiesVec->end())
		{
			AgarioAgent* pEnemy = *closestEnemy;
			if (agentPos.DistanceSquared(pEnemy->GetPosition()) < nearRadius * nearRadius)
			{
				pBlackBoard->ChangeData("EnemyTarget", pEnemy->GetPosition());
				return true;
			}
		}
		return false;
	}

	bool IsEnemyNearBySmaller(Elite::Blackboard* pBlackBoard)
	{
		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return false;
		}
		if (!pBlackBoard->GetData("AgentsVec", pEnemiesVec) || pEnemiesVec == nullptr)
		{
			return false;
		}

		const float catchRadius{ 100.0f + pAgent->GetRadius() };


		Elite::Vector2 agentPos{ pAgent->GetPosition() };
		auto elemnetdis = [agentPos](AgarioAgent* pEnemy1, AgarioAgent* pEnemy2)
		{
			float dist1 = agentPos.DistanceSquared(pEnemy1->GetPosition());
			float dist2 = agentPos.DistanceSquared(pEnemy2->GetPosition());
			return dist1 < dist2;
		};

		auto closestEnemy = std::min_element(pEnemiesVec->begin(), pEnemiesVec->end(), elemnetdis);


		if (closestEnemy != pEnemiesVec->end())
		{
			AgarioAgent* pEnemy = *closestEnemy;

			if (pEnemy->GetRadius() < pAgent->GetRadius())
			{
				if (agentPos.DistanceSquared(pEnemy->GetPosition()) < catchRadius * catchRadius)
				{
					pBlackBoard->ChangeData("EnemyTarget", pEnemy->GetPosition());
					return true;
				}
			}
		}
		return false;
	}

	bool IsEnemyNearByLarger(Elite::Blackboard* pBlackBoard)
	{
		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return false;
		}
		if (!pBlackBoard->GetData("AgentsVec", pEnemiesVec) || pEnemiesVec == nullptr)
		{
			return false;
		}

		const float catchRadius{ 50.0f + pAgent->GetRadius() };


		Elite::Vector2 agentPos{ pAgent->GetPosition() };
		auto elemnetdis = [agentPos](AgarioAgent* pEnemy1, AgarioAgent* pEnemy2)
		{
			float dist1 = agentPos.DistanceSquared(pEnemy1->GetPosition());
			float dist2 = agentPos.DistanceSquared(pEnemy2->GetPosition());
			return dist1 < dist2;
		};

		auto closestEnemy = std::min_element(pEnemiesVec->begin(), pEnemiesVec->end(), elemnetdis);


		if (closestEnemy != pEnemiesVec->end())
		{
			AgarioAgent* pEnemy = *closestEnemy;

			if (pEnemy->GetRadius() > pAgent->GetRadius())
			{
				if (agentPos.DistanceSquared(pEnemy->GetPosition()) < catchRadius * catchRadius)
				{
					pBlackBoard->ChangeData("EnemyTargetFlee", pEnemy->GetPosition());
					return true;
				}
			}
		}
		return false;
	}


	bool IsFoodNearBy(Elite::Blackboard* pBlackBoard)
	{
		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return false;
		}
		if (!pBlackBoard->GetData("FoodVec", pFoodVec) || pFoodVec == nullptr)
		{
			return false;
		}

		const float searchRadius{ 50.0f + pAgent->GetRadius()};
		AgarioFood* pClosestFood = nullptr;
		float closestDistSq{searchRadius * searchRadius};

		Elite::Vector2 agentPos = pAgent->GetPosition();
		//TODO: DEBUG RENDERING!!

		for (auto& pFood : *pFoodVec)
		{
			float distSq = pFood->GetPosition().DistanceSquared(agentPos);

			if (distSq < closestDistSq)
			{
				pClosestFood = pFood;
				closestDistSq = distSq;
			}
		}

		if (pClosestFood != nullptr)
		{
			pBlackBoard->ChangeData("Target", pClosestFood->GetPosition());
			return true;
		}
		return false;
	}

	bool IsFoodNearByClose(Elite::Blackboard* pBlackBoard)
	{
		if (!pBlackBoard->GetData("Agent", pAgent) || pAgent == nullptr)
		{
			return false;
		}
		if (!pBlackBoard->GetData("FoodVec", pFoodVec) || pFoodVec == nullptr)
		{
			return false;
		}

		const float searchRadius{ 7.0f + pAgent->GetRadius() };
		AgarioFood* pClosestFood = nullptr;
		float closestDistSq{ searchRadius * searchRadius };

		Elite::Vector2 agentPos = pAgent->GetPosition();
		//TODO: DEBUG RENDERING!!

		for (auto& pFood : *pFoodVec)
		{
			float distSq = pFood->GetPosition().DistanceSquared(agentPos);

			if (distSq < closestDistSq)
			{
				pClosestFood = pFood;
				closestDistSq = distSq;
			}
		}

		if (pClosestFood != nullptr)
		{
			pBlackBoard->ChangeData("Target", pClosestFood->GetPosition());
			return true;
		}
		return false;
	}


	bool IsAgentNearBorder(Elite::Blackboard* pBlackBoard)
	{
		bool isValid = pBlackBoard->GetData("Agent", pAgent);
		if (isValid == false || pAgent == nullptr)
		{
			return false;
		}
		isValid = pBlackBoard->GetData("BorderSize", trimWorld);
		if (isValid == false || trimWorld == nullptr)
		{
			return false;
		}


		Elite::Vector2 agentPos{ pAgent->GetPosition() };

		Elite::Vector2 agentDirection{ pAgent->GetDirection() };
		float trimWorldSize = *trimWorld;

		if ((agentPos.x < pAgent->GetRadius()) ||
			(agentPos.x > trimWorldSize - pAgent->GetRadius()) ||
			(agentPos.y < pAgent->GetRadius()) ||
			(agentPos.y > trimWorldSize - pAgent->GetRadius()))
			return true;

		return false;
	}



}

#endif