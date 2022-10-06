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
	SetTarget(target);

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(target, 5, { 0, 1, 0 });
	}
	std::cout << target << '\n';
	return Seek::CalculateSteering(deltaT, pAgent);
}

//*********************
//SEPARATION (FLOCKING)

SteeringOutput Separation::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	Elite::Vector2 target{ m_pFlock->GetAverageNeighborPos() };
	SetTarget(target);

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(target, 5, { 0, 1, 0 });
	}
	std::cout << target << '\n';
	return Flee::CalculateSteering(deltaT, pAgent);
}

//*************************
//VELOCITY MATCH (FLOCKING)