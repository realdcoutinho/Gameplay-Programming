/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/
#ifndef ELITE_APPLICATION_FSM_STATES_TRANSITIONS
#define ELITE_APPLICATION_FSM_STATES_TRANSITIONS

#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"

//------------
//---STATES---
//------------

namespace FSMStates
{
	class WanderState : public Elite::FSMState
	{
	public:
		WanderState() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;
	};


	class SeekFoodState : public Elite::FSMState
	{
	public:
		SeekFoodState() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;
	};

	class EvadeState : public Elite::FSMState
	{
	public:
		EvadeState() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;
	};

	class PursuitSmallAgent : public Elite::FSMState
	{
	public:
		PursuitSmallAgent() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;
	};


	class EvadeBorderState : public Elite::FSMState
	{
	public:
		EvadeBorderState() : FSMState() {};
		virtual void OnEnter(Elite::Blackboard* pBlackBoard) override;
	};

}


//-----------------
//---TRANSITIONS---
//-----------------

namespace FSMConditions
{
	class IsFoodGone : public Elite::FSMCondition
	{
	public:

		//Inherited via FSMCondition
		virtual bool Evaluate(Elite::Blackboard* pBlackBoard) const override;
	};

	class IsAwayFromBorder : public Elite::FSMCondition
	{
	public:

		//Inherited via FSMCondition
		virtual bool Evaluate(Elite::Blackboard* pBlackBoard) const override;
	};

	class EnemyNearBySmaller : public Elite::FSMCondition
	{
	public:

		//Inherited via FSMCondition
		virtual bool Evaluate(Elite::Blackboard* pBlackBoard) const override;
	};

	class EnemyNotCaught : public Elite::FSMCondition
	{
	public:

		//Inherited via FSMCondition
		virtual bool Evaluate(Elite::Blackboard* pBlackBoard) const override;
	};
	class EnemyCaught : public Elite::FSMCondition
	{
	public:

		//Inherited via FSMCondition
		virtual bool Evaluate(Elite::Blackboard* pBlackBoard) const override;
	};





	class FoodNearByCondition : public Elite::FSMCondition
	{
	public:

		//Inherited via FSMCondition
		virtual bool Evaluate(Elite::Blackboard* pBlackBoard) const override;
	};

	class AgentNearBorder : public Elite::FSMCondition
	{
	public:

		//Inherited via FSMCondition
		virtual bool Evaluate(Elite::Blackboard* pBlackBoard) const override;
	};

	class IsAgentWondering : public Elite::FSMCondition
	{
	public:

		//Inherited via FSMCondition
		virtual bool Evaluate(Elite::Blackboard* pBlackBoard) const override;
	};

}




#endif