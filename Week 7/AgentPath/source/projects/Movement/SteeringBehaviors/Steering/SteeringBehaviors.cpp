//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"

using namespace Elite;


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

SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	//Vector2 agentDirection{ cosf(pAgent->GetRotation()), sinf(pAgent->GetRotation()) }; //uses rotation to get agents vector direction. so, its his direction in the unit circle
	Vector2 circleCenter{ pAgent->GetPosition() + pAgent->GetDirection() * m_OffsetDistance }; //gets agents position. multiples the direction vector by the offset distance and adds it.

	float randomAngle{ -m_MaxAngleChange + (std::rand() % static_cast<int>(m_MaxAngleChange - (-m_MaxAngleChange) + 1)) }; //creates a radon float between -m_MaxAngleChange and m_MaxAngleChange
	m_WanderAngle += randomAngle; //Sets a new wander angle
	Vector2 randomPoint{ cosf(m_WanderAngle), sinf(m_WanderAngle) }; //cxreates a new point on a unit circle based on the values by the random angle added to the m_WanderAngle
	randomPoint *= m_Radius; // multiplies those same points by the radius, placing them on a circle of the same size
	randomPoint += circleCenter; // adds the CircleCenter Vector to it, making the random point be place within the limits of the circle it self in from of the agent
	SetTarget(randomPoint); //sets the random point as the new target


	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawCircle(circleCenter, m_Radius, { 0, 0, 1 }, 0);
		DEBUGRENDERER2D->DrawPoint(circleCenter, 2.0f, { 0, 0, 1 }, 0);
		DEBUGRENDERER2D->DrawPoint(randomPoint, 5.0f, { 1, 0, 1 }, 0);
	}
	return Seek::CalculateSteering(deltaT, pAgent);
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

//FACE
SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	Elite::Vector2 targetVector{ m_Target.Position - pAgent->GetPosition() };
	Elite::Vector2 agentDirection{ cosf(pAgent->GetRotation()), sinf(pAgent->GetRotation()) };

	//different method
	float angle{ acosf(Elite::Dot(agentDirection, targetVector) / (agentDirection.Magnitude() * targetVector.Magnitude())) };
	Elite::Vector3 targetVectorZ{ m_Target.Position - pAgent->GetPosition() };
	Elite::Vector3 agentDirectionZ{ cosf(pAgent->GetRotation()), sinf(pAgent->GetRotation()), 0.0f };
	Elite::Vector3 crossZCheck{ Elite::Cross(targetVectorZ, agentDirectionZ) };
	if (crossZCheck.z < 0)
		angle = -angle;
	float haltAngle{ static_cast<float>(Elite::ToRadians(0.005f)) };



	pAgent->SetAutoOrient(false);
	if (haltAngle <= angle)
	{
		steering.AngularVelocity = -pAgent->GetMaxAngularSpeed();
	}
	else if (angle <= haltAngle)
	{
		steering.AngularVelocity = pAgent->GetMaxAngularSpeed();
	}
	if (-haltAngle <= angle && angle <= haltAngle)
	{
		steering.AngularVelocity = 0;
	}
	steering.AngularVelocity *= abs(angle);



	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), targetVector, 5, { 0, 1.0f, 0 });
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), agentDirection, 5, { 0, 0, 1.0f });
	}
	return steering;
}