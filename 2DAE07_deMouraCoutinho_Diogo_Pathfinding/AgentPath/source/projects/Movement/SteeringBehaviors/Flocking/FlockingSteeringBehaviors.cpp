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

	float distance{ Elite::Distance(target, agentPosition) };
	float absDistance{ abs(distance) };

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(target, 5, { 1, 0, 1 });
		//DEBUGRENDERER2D->DrawDirection(agentPosition, target, distance, { 1, 0, 1 });
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

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawSegment(pAgent->GetPosition(), m_Target.Position, Elite::Color(0, 0, 1));
	}

	SteeringOutput steering{ Seek::CalculateSteering(deltaT, pAgent) };
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
	// Tries to evade the path of the target	

	// Get the vector to the target to compare with
	SteeringOutput steering{};

	//const Elite::Vector2 targetVector{ m_Target.Position - pAgent->GetPosition() };
	//float distanceSquared{ targetVector.MagnitudeSquared() };
	//if (distanceSquared > Elite::Square(m_EvadeRadius))
	//{
	//	// Return if too far to evade
	//	steering.IsValid = false;
	//	return steering;
	//}
	//
	//// Get the direction of the target and calculate where it will be in the future
	//const Elite::Vector2 targetDirection{ m_Target.LinearVelocity * m_LookAheadSeconds };
	//Elite::Vector2 targetFuturePosition{ m_Target.Position + targetDirection };
	//
	//// Shorten the future calculation if the future position is closer than the current distance
	//const float ratio{ targetVector.MagnitudeSquared() / targetDirection.MagnitudeSquared() };
	//if (ratio < 1.2f)
	//{
	//	// Target is too close (flee directly from the current targets position and abandon all hope)
	//	targetFuturePosition = m_Target.Position;
	//}

	if (pAgent->CanRenderBehavior())
	{
		//DEBUGRENDERER2D->DrawSegment(m_Target.Position, targetFuturePosition, Elite::Color(1.0f, 1.0f, 0.0f), 0.0f);
		//DEBUGRENDERER2D->DrawPoint(targetFuturePosition, 5.0f, Elite::Color(0.0f, 0.67f, 1.0f), 0.0f);
		//DEBUGRENDERER2D->DrawCircle(m_Target.Position, m_EvadeRadius, Elite::Color(1.0f, 1.0f, 0.0f), 0);
	}

	//m_Target.Position = targetFuturePosition;
	steering = Flee::CalculateSteering(deltaT, pAgent);
	return steering;
}
