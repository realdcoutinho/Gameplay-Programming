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

	//Elite::Vector3 vectorAgent{ pAgent->GetPosition() };

	//std::cout << vectorAgent.x << " " << vectorAgent.y << '\n';


	return steering;
}

SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{





	float yMax{ pAgent->GetPosition().y + m_OffsetDistance + m_Radius };
	float yMin{ pAgent->GetPosition().y + m_OffsetDistance - m_Radius };
	float xMax{ pAgent->GetPosition().x + m_OffsetDistance + m_Radius};
	float xMin{ pAgent->GetPosition().x + m_OffsetDistance - m_Radius};

	float xTargetPos{ static_cast<float>(xMin + (std::rand() % static_cast<int>(xMax - xMin + 1))) };
	float yTargetPos{ static_cast<float>(yMin + (std::rand() % static_cast<int>(yMax - yMin + 1))) };

	Elite::Vector2 targetPos{ xTargetPos , yTargetPos };

	SteeringOutput steering = {};
	SteeringOutput calculations = {};

	steering.LinearVelocity = targetPos - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	//Elite::Vector2

	

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0, 1, 0 });
		DEBUGRENDERER2D->DrawCircle(targetPos, m_Radius, Elite::Color{1.f, 1.f, 1.f, 1.f}, 0);

		std::cout << targetPos.x << " " << targetPos.y << '\n';

	}

	return steering;


}














SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	//return SteeringOutput();

	SteeringOutput steering = {};

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
	SteeringOutput calculations = {};
	m_Target.AngularVelocity;


	calculations.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	calculations.LinearVelocity.Normalize();
	calculations.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	Elite::Vector3 targetVector	{ Elite::Vector3{ m_Target.Position.x,m_Target.Position.y, 0 } };
	Elite::Vector3 agentVector	{ Elite::Vector3{pAgent->GetPosition().x, pAgent->GetPosition().y, 0 } };


	Elite::Vector3 agentToTarget{ targetVector - agentToTarget };
	
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), calculations.LinearVelocity, 5, { 0, 1, 0 });
	}
	return steering;
}
