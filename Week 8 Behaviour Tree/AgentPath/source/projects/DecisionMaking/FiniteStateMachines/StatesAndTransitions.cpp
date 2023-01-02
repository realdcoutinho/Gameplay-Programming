#include "stdafx.h"
#include "StatesAndTransitions.h"

using namespace Elite;
using namespace FSMStates;
using namespace FSMConditions;

void WanderState::OnEnter(Blackboard* pBlackBoard)
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return;
	}

	pAgent->SetToWander();
}


void EvadeBorderState::OnEnter(Blackboard* pBlackBoard)
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return;
	}


	float* trimWorld;
	isValid = pBlackBoard->GetData("BorderSize", trimWorld);
	if (isValid == false || trimWorld == nullptr)
	{
		return;
	}


	Vector2 agentPos{ pAgent->GetPosition() };

	Vector2 agentDirection{ pAgent->GetDirection() };
	float trimWorldSize = *trimWorld;
	pAgent->SetToSeek(Vector2{ trimWorldSize / 2, trimWorldSize / 2 });
}

void SeekFoodState::OnEnter(Blackboard* pBlackBoard)
{
	//TODO
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return;
	}

	AgarioFood* pFood;
	isValid = pBlackBoard->GetData("FoodTarget", pFood);
	pFood;
	if (isValid == false || pFood == nullptr)
	{
		return;
	}



	Vector2 foodPosition{ pFood->GetPosition() };

	pAgent->SetToSeek(foodPosition);
	//pAgent->SetToFace(foodPosition);
}

void PursuitSmallAgent::OnEnter(Blackboard* pBlackBoard)
{
	AgarioAgent* pAgent;


	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return;
	}

	AgarioAgent* pEnemies;
	isValid = pBlackBoard->GetData("Enemy", pEnemies);
	if (isValid == false || pEnemies == nullptr)
	{
		return;
	}

	Vector2 enemyPosition{ pEnemies->GetPosition() };

	pAgent->SetToSeek(enemyPosition);
	//pAgent->SetToFace(foodPosition);
}


///Transsition

bool FoodNearByCondition::Evaluate(Blackboard* pBlackBoard) const
{
	//TODO
	
	AgarioAgent* pAgent;


	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return false;
	}

	std::vector<AgarioFood*>* pFoodVec;
	isValid = pBlackBoard->GetData("FoodVecPtr", pFoodVec);
	if (isValid == false || pFoodVec == nullptr)
	{
		return false;
	}



	const float foodRadius{ 20.0f + pAgent->GetRadius()};


	Vector2 agentPos{ pAgent->GetPosition() };
	auto elemnetdis = [agentPos](AgarioFood* pFood1, AgarioFood* pFood2) 
	{ 
		float dist1 = agentPos.DistanceSquared(pFood1->GetPosition());
		float dist2 = agentPos.DistanceSquared(pFood2->GetPosition());
		return dist1 < dist2;
	};

	auto closestFoodIt = std::min_element(pFoodVec->begin(), pFoodVec->end(), elemnetdis);


	if (closestFoodIt != pFoodVec->end())
	{
		AgarioFood* pFood = *closestFoodIt;
		
		if (agentPos.DistanceSquared(pFood->GetPosition()) < foodRadius * foodRadius)
		{
			pBlackBoard->ChangeData("FoodTarget", pFood);
			return true;
		}
	}
	return false;
}


bool AgentNearBorder::Evaluate(Blackboard* pBlackBoard) const
{
	AgarioAgent* pAgent;

	float* trimWorld;
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


	Vector2 agentPos{ pAgent->GetPosition() };

	Vector2 agentDirection{ pAgent->GetDirection() };
	float trimWorldSize = *trimWorld;

	if ((agentPos.x < pAgent->GetRadius()) || 
		(agentPos.x > trimWorldSize - pAgent->GetRadius()) ||
		(agentPos.y < pAgent->GetRadius()) || 
		(agentPos.y > trimWorldSize - pAgent->GetRadius()))
		return true;

	return false;
}

bool IsFoodGone::Evaluate(Blackboard* pBlackBoard) const
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return false;
	}

	AgarioFood* pFood;
	isValid = pBlackBoard->GetData("FoodTarget", pFood);
	pFood;
	if (isValid == false || pFood == nullptr)
	{
		return false;
	}

	Vector2 foodPos{ pFood->GetPosition() };
	Vector2 agentPos{ pAgent->GetPosition() };
	float agentRadius{ pAgent->GetRadius() };


	Elite::Vector2 distanceBetween = agentPos - foodPos;
	float distanceSquared = distanceBetween.MagnitudeSquared();

	if (distanceSquared > agentRadius * agentRadius)
	{
		return true;
	}
	return false;
}

bool IsAwayFromBorder::Evaluate(Blackboard* pBlackBoard) const
{
	AgarioAgent* pAgent;

	float* trimWorld;
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


	Vector2 agentPos{ pAgent->GetPosition() };

	Vector2 agentDirection{ pAgent->GetDirection() };
	float trimWorldSize = *trimWorld;

	if ((agentPos.x > pAgent->GetRadius()) &&
		(agentPos.x < trimWorldSize - pAgent->GetRadius()) &&
		(agentPos.y > pAgent->GetRadius()) &&
		(agentPos.y < trimWorldSize - pAgent->GetRadius()))
		return true;

	return false;
}

bool IsAgentWondering::Evaluate(Blackboard* pBlackBoard) const
{
	AgarioAgent* pAgent;
	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return false;
	}

	ISteeringBehavior* WanderBehaviour;
	isValid = pBlackBoard->GetData("WanderBehaviour", WanderBehaviour);
	if (isValid == false || WanderBehaviour == nullptr)
	{
		return false;
	}

	if (pAgent->GetSteeringBehavior() == WanderBehaviour)
		return true;
	return false;
}

bool EnemyNearBySmaller::Evaluate(Blackboard* pBlackBoard) const
{

	AgarioAgent* pAgent;


	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return false;
	}

	std::vector<AgarioAgent*>* pEnemies;
	isValid = pBlackBoard->GetData("EnemiesVecPtr", pEnemies);
	if (isValid == false || pEnemies == nullptr)
	{
		return false;
	}



	const float catchRadius{ 100.0f + pAgent->GetRadius() };


	Vector2 agentPos{ pAgent->GetPosition() };
	auto elemnetdis = [agentPos](AgarioAgent* pEnemy1, AgarioAgent* pEnemy2)
	{
		float dist1 = agentPos.DistanceSquared(pEnemy1->GetPosition());
		float dist2 = agentPos.DistanceSquared(pEnemy2->GetPosition());
		return dist1 < dist2;
	};

	auto closestEnemy = std::min_element(pEnemies->begin(), pEnemies->end(), elemnetdis);


	if (closestEnemy != pEnemies->end())
	{
		AgarioAgent* pEnemy = *closestEnemy;

		if (pEnemy->GetRadius() < pAgent->GetRadius())
		{
			if (agentPos.DistanceSquared(pEnemy->GetPosition()) < catchRadius * catchRadius)
			{
				pBlackBoard->ChangeData("Enemy", pEnemy);
				return true;
			}
		}
	}
	return false;
}

bool EnemyNotCaught::Evaluate(Blackboard* pBlackBoard) const
{
	AgarioAgent* pAgent;


	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return false;
	}

	AgarioAgent* pEnemy;
	isValid = pBlackBoard->GetData("Enemy", pEnemy);
	ISteeringBehavior* wander;
	if (isValid == false || pEnemy == nullptr || pEnemy->m_pWander == nullptr)
	{


		return false;
	}

	Vector2 enemyPos{ pEnemy->GetPosition() };
	float enemyRadius{ pEnemy->GetRadius() };

	Vector2 agentPos{ pAgent->GetPosition() };
	float agentRadius{ pAgent->GetRadius() };

	Elite::Vector2 distanceBetween = agentPos - enemyPos;
	float distanceSquared = distanceBetween.MagnitudeSquared();

	if (distanceSquared > agentRadius * agentRadius + enemyRadius * enemyRadius)
	{
		return true;
	}


	return false;
}

bool EnemyCaught::Evaluate(Blackboard* pBlackBoard) const
{
	AgarioAgent* pAgent;


	bool isValid = pBlackBoard->GetData("Agent", pAgent);
	if (isValid == false || pAgent == nullptr)
	{
		return false;
	}

	AgarioAgent* pEnemy;
	isValid = pBlackBoard->GetData("Enemy", pEnemy);
	if (isValid != false || pEnemy != nullptr)
	{
		Vector2 enemyPos{ pEnemy->GetPosition() };
		float enemyRadius{ pEnemy->GetRadius() };

		Vector2 agentPos{ pAgent->GetPosition() };
		float agentRadius{ pAgent->GetRadius() };

		Elite::Vector2 distanceBetween = agentPos - enemyPos;
		float distanceSquared = distanceBetween.MagnitudeSquared();

		if (distanceSquared > agentRadius * agentRadius + enemyRadius * enemyRadius)
		{
			return false;
		}
	}
	return true;
}