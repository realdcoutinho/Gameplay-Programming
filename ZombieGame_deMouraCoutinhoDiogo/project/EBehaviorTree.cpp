//=== General Includes ===
#include "stdafx.h"
#include "EBehaviorTree.h"
using namespace Elite;

//-----------------------------------------------------------------
// BEHAVIOR TREE COMPOSITES (IBehavior)
//-----------------------------------------------------------------
#pragma region COMPOSITES
//SELECTOR
BehaviorState BehaviorSelector::Execute(Blackboard* pBlackBoard)
{
	//TODO: Fill in this code

	for (auto& child : m_ChildBehaviors) 	// Loop over all children in m_ChildBehaviors
	{
		m_CurrentState = child->Execute(pBlackBoard); //Every Child: Execute and store the result in m_CurrentState

		//Check the currentstate and apply the selector Logic:

		if (m_CurrentState == BehaviorState::Success)  //if a child returns Success:
		{
			return m_CurrentState; //stop looping over all children and return Success	
		}

		//if a child returns Running:
		if (m_CurrentState == BehaviorState::Running)
		{
			return m_CurrentState;//Running: stop looping and return Running
		}
		if (m_CurrentState == BehaviorState::Failure) //The selector fails if all children failed.
		{
			continue; //All children failed
		}
	}

	//try to make a switch that functions. Telllisense breaks.
	m_CurrentState = BehaviorState::Failure;
	return m_CurrentState;
}
//SEQUENCE
BehaviorState BehaviorSequence::Execute(Blackboard* pBlackBoard)
{
	//TODO: FIll in this code
	//Loop over all children in m_ChildBehaviors
	for (auto& child : m_ChildBehaviors) 	// Loop over all children in m_ChildBehaviors
	{
		m_CurrentState = child->Execute(pBlackBoard); //Every Child: Execute and store the result in m_CurrentState

		//Check the currentstate and apply the sequence Logic:
		//if a child returns Failed:
		if (m_CurrentState == BehaviorState::Failure)
		{
			//stop looping over all children and return Failed
			return BehaviorState::Failure;
		}

		//if a child returns Running:
		//Running: stop looping and return Running
		if (m_CurrentState == BehaviorState::Running)
		{
			return BehaviorState::Running;
		}
	}

	//The selector succeeds if all children succeeded.
	//All children succeeded 
	m_CurrentState = BehaviorState::Success;
	return m_CurrentState;
}
//PARTIAL SEQUENCE
BehaviorState BehaviorPartialSequence::Execute(Blackboard* pBlackBoard)
{
	while (m_CurrentBehaviorIndex < m_ChildBehaviors.size())
	{
		m_CurrentState = m_ChildBehaviors[m_CurrentBehaviorIndex]->Execute(pBlackBoard);
		switch (m_CurrentState)
		{
		case BehaviorState::Failure:
			m_CurrentBehaviorIndex = 0;
			return m_CurrentState;
		case BehaviorState::Success:
			++m_CurrentBehaviorIndex;
			m_CurrentState = BehaviorState::Running;
			return m_CurrentState;
		case BehaviorState::Running:
			return m_CurrentState;
		}
	}

	m_CurrentBehaviorIndex = 0;
	m_CurrentState = BehaviorState::Success;
	return m_CurrentState;
}
#pragma endregion
//-----------------------------------------------------------------
// BEHAVIOR TREE CONDITIONAL (IBehavior)
//-----------------------------------------------------------------
BehaviorState BehaviorConditional::Execute(Blackboard* pBlackBoard)
{
	if (m_fpConditional == nullptr)
		return BehaviorState::Failure;

	switch (m_fpConditional(pBlackBoard))
	{
	case true:
		m_CurrentState = BehaviorState::Success;
		return m_CurrentState;
	case false:
		m_CurrentState = m_CurrentState = BehaviorState::Failure;
		return m_CurrentState;
	//default:
	}

}
//-----------------------------------------------------------------
// BEHAVIOR TREE ACTION (IBehavior)
//-----------------------------------------------------------------
BehaviorState BehaviorAction::Execute(Blackboard* pBlackBoard)
{
	if (m_fpAction == nullptr)
		return BehaviorState::Failure;

	m_CurrentState = m_fpAction(pBlackBoard);
	return m_CurrentState;
}