#include "stdafx.h"
#include "SteeringBehaviors.h"
#include "IExamInterface.h"
#include "Agent.h"
//#include <iostream>
//



SteeringPlugin_Output Seek::CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface)
{
	steering.AutoOrient = true;
	auto target = pInterface->NavMesh_GetClosestPathPoint(pAgent->GetSeekPoint()); //sets the random point as the new target

	steering.LinearVelocity = target - pInterface->Agent_GetInfo().Position; //Desired Velocity

	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= pInterface->Agent_GetInfo().MaxLinearSpeed;

	steering.LinearVelocity;

	return steering;
}

SteeringPlugin_Output Wander::CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface)
{
	steering.AutoOrient = true;
	//float maxLinearSpeed = m_pInterface->Agent_GetInfo().MaxLinearSpeed;
	Vector2 circleCenter{ pInterface->Agent_GetInfo().Position + pInterface->Agent_GetInfo().LinearVelocity.GetNormalized() * m_OffsetDistance }; //gets agents position. multiples the direction vector by the offset distance and adds it.

	float randomAngle{ Elite::randomFloat(-m_MaxAngleChange, m_MaxAngleChange) }; //creates a radon float between -m_MaxAngleChange and m_MaxAngleChange
	m_WanderAngle += randomAngle; //Sets a new wander angle
	Vector2 randomPoint{ cosf(m_WanderAngle), sinf(m_WanderAngle) }; //cxreates a new point on a unit circle based on the values by the random angle added to the m_WanderAngle
	randomPoint *= m_Radius; // multiplies those same points by the radius, placing them on a circle of the same size
	randomPoint += circleCenter; // adds the CircleCenter Vector to it, making the random point be place within the limits of the circle it self in from of the agent

	auto target = pInterface->NavMesh_GetClosestPathPoint(randomPoint); //sets the random point as the new target

	steering.LinearVelocity = target - pInterface->Agent_GetInfo().Position; //Desired Velocity
	//steering.LinearVelocity.Normalize(); //Normalize Desired Velocity

	//steering.LinearVelocity = target;/* * m_pInterface->Agent_GetInfo().MaxLinearSpeed;*/ //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= pInterface->Agent_GetInfo().MaxLinearSpeed;
	return steering;
}


//FACE
SteeringPlugin_Output Face::CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface)
{
	steering.AutoOrient = false;
	AgentInfo agent = pInterface->Agent_GetInfo();
	Vector2 ePositon = pAgent->GetEnemyPosition();

	auto target = pAgent->GetSeekPoint(); //sets the random point as the new target

	//agent.
	Vector2 targetVector{ agent.Position - ePositon };
	Vector2 agentDirection{ cosf(agent.Orientation), sinf(agent.Orientation) };

	float angle{ acosf(Dot(agentDirection, targetVector) / (agentDirection.Magnitude() * targetVector.Magnitude())) };
	Vector3 targetVectorZ{ ePositon - agent.Position };
	Vector3 agentDirectionZ{ cosf(agent.AngularVelocity), sinf(agent.AngularVelocity), 0.0f };
	Vector3 crossZCheck{ Cross(targetVectorZ, agentDirectionZ) };
	if (crossZCheck.z > 0)
		angle = -angle;
	float haltAngle{ static_cast<float>(ToRadians(0.005f)) };

	if (haltAngle <= angle)
	{
		steering.AngularVelocity = agent.MaxAngularSpeed;
	}
	else if (angle <= haltAngle)
	{
		steering.AngularVelocity = agent.MaxAngularSpeed;
	}
	if (-haltAngle <= angle && angle <= haltAngle)
	{
		steering.AngularVelocity = 0;
	}
	steering.AngularVelocity = angle;
	return steering;
}


SteeringPlugin_Output Flee::CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface)
{
	AgentInfo agent = pInterface->Agent_GetInfo();
	Vector2 ePositon = pAgent->GetEnemyPosition();

	Vector2 runFrom = agent.Position - ePositon;

	steering.LinearVelocity = runFrom;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent.MaxLinearSpeed;

	return steering;
}

//SteeringPlugin_Output Arrive::CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface)
//{
//	const float arrivalRadius{ 1.0f };
//	const float slowRadious{ 15.0f };
//
//	steering.AutoOrient = false;
//	auto target = pInterface->NavMesh_GetClosestPathPoint(pAgent->GetSeekPoint()); //sets the random point as the new target
//
//
//	Elite::Vector2 toTarget{ target - pAgent->GetPosition() };
//	float distance{ toTarget.Magnitude() };
//
//	AgentInfo agent = pInterface->Agent_GetInfo();
//	Vector2 ePositon = pAgent->GetEnemyPosition();
//
//	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
//	steering.LinearVelocity.Normalize();
//	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
//
//	if (distance < arrivalRadius)
//	{
//		steering.LinearVelocity = Elite::Vector2{ 0, 0 };
//		//return steering;
//	}
//	if (distance < slowRadious)
//	{
//		steering.LinearVelocity *= distance / slowRadious;
//	}
//	else {
//		//steering.LinearVelocity *= maxSpeed;
//	}
//
//	if (pAgent->CanRenderBehavior())
//	{
//		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0, 1, 0 });
//	}
//
//	return steering;
//}
