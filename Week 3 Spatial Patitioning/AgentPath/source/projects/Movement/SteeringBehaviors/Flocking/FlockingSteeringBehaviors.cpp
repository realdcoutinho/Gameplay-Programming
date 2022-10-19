#include "stdafx.h"
#include "FlockingSteeringBehaviors.h"
#include "Flock.h"
#include "../SteeringAgent.h"
#include "../SteeringHelpers.h"


//*******************
//COHESION (FLOCKING)
SteeringOutput Cohesion::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	Elite::Vector2 target{ m_pFlock->GetAverageNeighborPos()};
	Elite::Vector2 agentPosition{ pAgent->GetPosition() };
	SetTarget(target);

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(target, 5, { 1, 0, 1 });
		DEBUGRENDERER2D->DrawDirection(agentPosition, target, 5, { 1, 0, 1 });
	}
	return Seek::CalculateSteering(deltaT, pAgent);
}

//*********************
//SEPARATION (FLOCKING)

SteeringOutput Separation::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{	
	Elite::Vector2 totalInvPropVector{};
	
	std::vector<SteeringAgent*>neighbors{ m_pFlock->GetNeighbors() };
	int nrOfNeightbors{m_pFlock->GetNrOfNeighbors()};

	for (int index{}; index < nrOfNeightbors; ++index)
	{
			float distance{ Elite::Distance(pAgent->GetPosition(), neighbors[index]->GetPosition()) };
			Elite::Vector2 awayVector{ pAgent->GetPosition() -  neighbors[index]->GetPosition() };
			Elite::Vector2 invPropVector{ awayVector / powf(distance, 2)};

			totalInvPropVector += invPropVector;
	}
	totalInvPropVector /= float(nrOfNeightbors);

	SetTarget(totalInvPropVector + pAgent->GetPosition());

	SteeringOutput steering{ Seek::CalculateSteering(deltaT, pAgent) };
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5,{ 1, 0, 1 });
	}
	return steering;
}

//*************************
//VELOCITY MATCH (FLOCKING)
SteeringOutput VelocityMatch::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	Elite::Vector2 averageVelocity{ m_pFlock->GetAverageNeighborVelocity() };


	SteeringOutput steering = {};

	steering.LinearVelocity = averageVelocity;

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0, 1, 0 });
	}

	return steering;
}

//*************************
//EVADE (FLOCKING)
SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering{};
	steering = Flee::CalculateSteering(deltaT, pAgent);
	return steering;
}
