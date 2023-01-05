#include "IExamInterface.h"
#include "stdafx.h"
#include "Agent.h"
#include "BlackBoard.h"
#include "IExamInterface.h"


#include <iostream>

using namespace std;
using namespace Elite;

Agent::Agent(IExamInterface* pInterface)
	: m_pInterface{pInterface}
{
	std::cout << "agent class created" << '\n';
	
	m_Agent = m_pInterface->Agent_GetInfo();
	m_Radius = m_Agent.GrabRange;
	m_MaxAngleChange = m_Agent.FOV_Angle;
}

Agent::~Agent() 
{
	delete m_pInterface;
}

void Agent::SetDecisionMaking(BehaviorTree* decisionMakingStructure)
{
	m_pDecisionMaking = decisionMakingStructure;
}

Vector2 Agent::GetSeekPoint() 
{
	return {};
}

void Agent::SetWander(SteeringPlugin_Output& steering)
{
	steering.AutoOrient = true;
	//float maxLinearSpeed = m_pInterface->Agent_GetInfo().MaxLinearSpeed;
	Vector2 circleCenter{ m_pInterface->Agent_GetInfo().Position + m_pInterface->Agent_GetInfo().LinearVelocity * m_OffsetDistance }; //gets agents position. multiples the direction vector by the offset distance and adds it.

	float randomAngle{ -m_MaxAngleChange + (std::rand() % static_cast<int>(m_MaxAngleChange - (-m_MaxAngleChange) + 1)) }; //creates a radon float between -m_MaxAngleChange and m_MaxAngleChange
	m_WanderAngle += randomAngle; //Sets a new wander angle
	Vector2 randomPoint{ cosf(m_WanderAngle), sinf(m_WanderAngle) }; //cxreates a new point on a unit circle based on the values by the random angle added to the m_WanderAngle
	randomPoint *= m_Radius; // multiplies those same points by the radius, placing them on a circle of the same size
	randomPoint += circleCenter; // adds the CircleCenter Vector to it, making the random point be place within the limits of the circle it self in from of the agent

	auto target = m_pInterface->NavMesh_GetClosestPathPoint(randomPoint); //sets the random point as the new target

	steering.LinearVelocity = target - m_pInterface->Agent_GetInfo().Position; //Desired Velocity
	//steering.LinearVelocity.Normalize(); //Normalize Desired Velocity

	//steering.LinearVelocity = target;/* * m_pInterface->Agent_GetInfo().MaxLinearSpeed;*/ //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	//steering.LinearVelocity *= m_pInterface->Agent_GetInfo().MaxLinearSpeed;
	//steering.LinearVelocity;
}

void Agent::SetFace(SteeringPlugin_Output& steering, EntityInfo& enemy)
{
	//SteeringOutput steering = {};
	steering.AngularVelocity = 0;
	steering.AutoOrient = false;

	Elite::Vector2 targetVector{ m_pInterface->Agent_GetInfo().Position - enemy.Location };
	Elite::Vector2 agentDirection{ cosf(m_pInterface->Agent_GetInfo().AngularVelocity), sinf(m_pInterface->Agent_GetInfo().AngularVelocity) };

	//different method
	float angle{ acosf(Elite::Dot(agentDirection, targetVector) / (agentDirection.Magnitude() * targetVector.Magnitude())) };
	Elite::Vector3 targetVectorZ{ enemy.Location - m_pInterface->Agent_GetInfo().Position };
	Elite::Vector3 agentDirectionZ{ cosf(m_pInterface->Agent_GetInfo().AngularVelocity), sinf(m_pInterface->Agent_GetInfo().AngularVelocity), 0.0f };
	Elite::Vector3 crossZCheck{ Elite::Cross(targetVectorZ, agentDirectionZ) };
	if (crossZCheck.z < 0)
		angle = -angle;
	float haltAngle{ static_cast<float>(Elite::ToRadians(0.005f)) };



	//pAgent->SetAutoOrient(false);
	if (haltAngle <= angle)
	{
		steering.AngularVelocity = -m_MaxAngularSpeed;
	}
	else if (angle <= haltAngle)
	{
		steering.AngularVelocity = m_MaxAngularSpeed;
	}
	if (-haltAngle <= angle && angle <= haltAngle)
	{
		steering.AngularVelocity = 0;
	}
	steering.AngularVelocity *= abs(angle);
}

void Agent::SetFace(SteeringPlugin_Output& steering, Vector2& point)
{
	steering.AngularVelocity = 0;
	steering.AutoOrient = false;

	Elite::Vector2 targetVector{ m_pInterface->Agent_GetInfo().Position - point };
	Elite::Vector2 agentDirection{ cosf(m_pInterface->Agent_GetInfo().AngularVelocity), sinf(m_pInterface->Agent_GetInfo().AngularVelocity) };

	//different method
	float angle{ acosf(Elite::Dot(agentDirection, targetVector) / (agentDirection.Magnitude() * targetVector.Magnitude())) };
	Elite::Vector3 targetVectorZ{ point - m_pInterface->Agent_GetInfo().Position };
	Elite::Vector3 agentDirectionZ{ cosf(m_pInterface->Agent_GetInfo().AngularVelocity), sinf(m_pInterface->Agent_GetInfo().AngularVelocity), 0.0f };
	Elite::Vector3 crossZCheck{ Elite::Cross(targetVectorZ, agentDirectionZ) };
	if (crossZCheck.z < 0)
		angle = -angle;
	float haltAngle{ static_cast<float>(Elite::ToRadians(0.005f)) };



	//pAgent->SetAutoOrient(false);
	if (haltAngle <= angle)
	{
		steering.AngularVelocity = -m_MaxAngularSpeed;
	}
	else if (angle <= haltAngle)
	{
		steering.AngularVelocity = m_MaxAngularSpeed;
	}
	if (-haltAngle <= angle && angle <= haltAngle)
	{
		steering.AngularVelocity = 0;
	}
	steering.AngularVelocity *= abs(angle);
}


vector<HouseInfo> Agent::GetRegisteredHouses() const
{
	return m_RegisteredHouses;
}

void Agent::Update(IExamInterface* pInterface)
{
	m_Agent = pInterface->Agent_GetInfo();
	m_Agent.Position;
	//if (!m_pDecisionMaking)
	//{
	//	std::cout << "NULL" << '\n';
	//}
}

void Agent::AddHouse(const HouseInfo& houseSeen)
{
	auto houseExists = [&houseSeen](const HouseInfo& houseRecorded) { return houseSeen.Center == houseRecorded.Center; };
	auto house = std::find_if(m_RegisteredHouses.begin(), m_RegisteredHouses.end(), houseExists);

	if (house == std::end(m_RegisteredHouses))
	{
		m_RegisteredHouses.push_back(houseSeen);
		++nrKnownHouses;
	}
}
