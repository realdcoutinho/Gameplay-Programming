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
	return Flee::CalculateSteering(deltaT, pAgent);
}

//*************************
//VELOCITY MATCH (FLOCKING)
SteeringOutput VelocityMatch::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	Elite::Vector2 averageVelocity{ m_pFlock->GetAverageNeighborVelocity() };
	//averageVelocity = { 15, 15 };

	SteeringOutput steering = {};

	steering.LinearVelocity = averageVelocity;

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0, 1, 0 });
	}

	return steering;
}