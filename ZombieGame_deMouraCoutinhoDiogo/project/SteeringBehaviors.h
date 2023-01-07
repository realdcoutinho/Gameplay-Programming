#pragma once

#include "Exam_HelperStructs.h"
#include <vector>


class IBaseInterface;
class IExamInterface;
class Agent;

using namespace std;
using namespace Elite;


class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringPlugin_Output CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface) = 0;

protected:
	float m_OffsetDistance = 6.0f; // OffSet (Agent Direction)
	float m_Radius = 4.0f; // WanderRadius
	float m_MaxAngleChange = Elite::ToRadians(90); //Max WanderAngle change per frame
	float m_WanderAngle = 0.f; //Interval

};
#pragma endregion

/////////////////////////////////////////
////FACE
////****
class Wander : public ISteeringBehavior
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	SteeringPlugin_Output CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface) override;
};

/////////////////////////////////////////
////SEEK
////****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;
	//Seek Behaviour
	SteeringPlugin_Output CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface) override;
};

/////////////////////////////////////////
////FACE
////****
class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;
	SteeringPlugin_Output CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface) override;
};

/////////////////////////////////////////
////FLEE
////****
class Flee : public ISteeringBehavior
{
public:
	Flee() = default;
	virtual ~Flee() = default;
	SteeringPlugin_Output CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface) override;
};

///////////////////////////////////////////
//////ARRIVE
//////****
//class Arrive : public ISteeringBehavior
//{
//public:
//	Arrive() = default;
//	virtual ~Arrive() = default;
//	SteeringPlugin_Output CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface) override;
//};


////Flee
//
//class Flee : public ISteeringBehavior
//{
//public:
//	Flee() = default;
//	virtual ~Flee() = default;
//	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
//	void SetFleeRadious(float fleeRadious) { m_FleeRadious = fleeRadious; }
//private:
//	float m_FleeRadious = 20.f;
//};
//
//class Arrive : public ISteeringBehavior
//{
//public:
//	Arrive() = default;
//	virtual ~Arrive() = default;
//	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
//};
//
//
//


