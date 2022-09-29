//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"


//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0, 1, 0 });
	}

	return steering;
}

SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	//return SteeringOutput();

	SteeringOutput steering = {};


	Elite::Vector2 toTarget = pAgent->GetPosition() - (m_Target).Position;
	float distanceSquared = toTarget.MagnitudeSquared();

	if (distanceSquared > m_FleeRadious * m_FleeRadious)
	{
		steering.IsValid = false;
		return steering;
	}

	steering.LinearVelocity = pAgent->GetPosition() - m_Target.Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0, 1, 0 });
	}

	return steering;

}

SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{

	SteeringOutput steering = {};

	const float arrivalRadius{ 1.0f };
	const float slowRadious{ 15.0f };
	Elite::Vector2 toTarget{ m_Target.Position - pAgent->GetPosition() };
	float distance{ toTarget.Magnitude() };

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (distance < arrivalRadius)
	{
		steering.LinearVelocity = Elite::Vector2{ 0, 0 };
		//return steering;
	}
	if (distance < slowRadious)
	{
			steering.LinearVelocity *= distance / slowRadious;
	}
	else {
			//steering.LinearVelocity *= maxSpeed;
	}

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0, 1, 0 });
	}

	return steering;
}

SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	//m_Target.AngularVelocity;
	//steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	//steering.LinearVelocity.Normalize();
	//steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	Elite::Vector3 targetVector	{ Elite::Vector3{ m_Target.Position.x,m_Target.Position.y, 0 } };
	Elite::Vector3 agentVector	{ Elite::Vector3{pAgent->GetPosition().x, pAgent->GetPosition().y, 0 } };

	
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), Elite::Vector2{ 0, 0 }, 5, { 0, 1, 0 });
	}
	return steering;
}
