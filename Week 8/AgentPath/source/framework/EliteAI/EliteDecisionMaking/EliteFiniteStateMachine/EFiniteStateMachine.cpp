//=== General Includes ===
#include "stdafx.h"
#include "EFiniteStateMachine.h"
using namespace Elite;

FiniteStateMachine::FiniteStateMachine(FSMState* startState, Blackboard* pBlackboard)
    : m_pCurrentState(nullptr),
    m_pBlackboard(pBlackboard)
{
    ChangeState(startState);
}

FiniteStateMachine::~FiniteStateMachine()
{
    SAFE_DELETE(m_pBlackboard);
}

void FiniteStateMachine::AddTransition(FSMState* startState, FSMState* toState, FSMCondition* condition)
{
    auto it = m_Transitions.find(startState);
    if (it == m_Transitions.end())
    {
        m_Transitions[startState] = Transitions();
    }
   
    m_Transitions[startState].push_back(std::make_pair(condition, toState));
}

void FiniteStateMachine::Update(float deltaTime)
{
    //TODO 4: Look if 1 or more condition/transitions exists for the current state that we are in
    //Tip: Check the transitions map for a TransitionState pair
    auto transitionsIt = m_Transitions.find(m_pCurrentState);

    float weight{ 0 };
    if (transitionsIt != m_Transitions.end())     //TODO 5: if a TransitionState exists
    {
        for (auto& transition : transitionsIt->second)        //TODO 6: Loop over all the TransitionState pairs 
        {
            FSMCondition* cond = transition.first;
            FSMState* state = transition.second;

            if (cond->Evaluate(m_pBlackboard))   //TODO 7: If the Evaluate function of the FSMCondition returns true => transition to the new corresponding state
            {
                
                ChangeState(state);     //TODO 8: Update the current state (if one exists)
                break;
                //instead store weight of transition.
                
            }
            else
            {
                //current state = wanderstate
            }
        }
    }
    if (m_pCurrentState != nullptr)
        m_pCurrentState->Update(m_pBlackboard, deltaTime);
}

Blackboard* FiniteStateMachine::GetBlackboard() const
{
    return m_pBlackboard;
}

void FiniteStateMachine::ChangeState(FSMState* newState)
{
    //TODO 1. If currently in a state => make sure the OnExit of that state gets called
    if(m_pCurrentState != nullptr)
    {
        m_pCurrentState->OnExit(m_pBlackboard);
    }

    //TODO 2. Change the current state to the new state
    m_pCurrentState = newState;

    //TODO 3. Check if received state is a nullptr
    assert((m_pCurrentState != nullptr) && "ChangeState received a nullptr instead of a valid state");

    //if (m_pCurrentState != nullptr)
    //{
    //    std::cout << "ChangeState received a nullptr instead of a valid state" << '\n';
    //    return;
    //}

    //TODO 4. If currently into a state => make sure the OnEnter of that state gets called
    m_pCurrentState->OnEnter(m_pBlackboard);       
}
