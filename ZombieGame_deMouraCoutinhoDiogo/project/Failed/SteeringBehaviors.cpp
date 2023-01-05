//#include "stdafx.h"
//#include "SteeringBehaviors.h"
//#include <iostream>
//
//SteeringBehaviors::SteeringBehaviors()
//{
//	std::wcout << L"Steering Behavior Created" << '\n';
//}
//
//void SteeringBehaviors::Wander(SteeringPlugin_Output& input, AgentInfo& agent)
//{
//	//m_WanderAngle = 0;
//	Elite::Vector2 agentDirection{ cosf(agent.AngularVelocity), sinf(agent.AngularVelocity) }; //uses rotation to get agents vector direction. so, its his direction in the unit circle
//	Elite::Vector2 circleCenter{ agent.Position.x + (agent.Orientation * m_OffsetDistance),
//		agent.Position.y + (agent.Orientation * agent.GrabRange) }; //gets agents position. multiples the direction vector by the offset distance and adds it.
//
//	float randomAngle{ -m_MaxAngleChange + (std::rand() % static_cast<int>(m_MaxAngleChange - (-m_MaxAngleChange) + 1)) }; //creates a radon float between -m_MaxAngleChange and m_MaxAngleChange
//	m_WanderAngle += randomAngle; //Sets a new wander angle
//	Elite::Vector2 randomPoint{ cosf(m_WanderAngle), sinf(m_WanderAngle) }; //cxreates a new point on a unit circle based on the values by the random angle added to the m_WanderAngle
//	randomPoint *= m_Radius; // multiplies those same points by the radius, placing them on a circle of the same size
//	randomPoint += circleCenter; // adds the CircleCenter Vector to it, making the random point be place within the limits of the circle it self in from of the agent
//	//SetTarget(randomPoint); //sets the random point as the new target
//
//
//	input.LinearVelocity = randomPoint; //Desired Velocity
//	//steering.LinearVelocity = { 0, 0 }; //Desired Velocity
//
//	input.LinearVelocity.Normalize(); //Normalize Desired Velocity
//	input.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed
//	//return randomPoint;
//}