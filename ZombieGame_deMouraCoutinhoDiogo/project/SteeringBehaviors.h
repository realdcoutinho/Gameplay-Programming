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
	ISteeringBehavior(const ISteeringBehavior& other) = delete; //copy constructor
	ISteeringBehavior& operator=(const ISteeringBehavior& other) = delete; // assignment operator
	ISteeringBehavior(ISteeringBehavior&& other) = delete; // move constructor
	ISteeringBehavior& operator=(ISteeringBehavior&& other) = delete; // move assignment operator
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
////Wander
////****
class Wander : public ISteeringBehavior
{
public:
	Wander() = default;
	Wander(const Wander& other) = delete; //copy constructor
	Wander& operator=(const Wander& other) = delete; // assignment operator
	Wander(Wander&& other) = delete; // move constructor
	Wander& operator=(Wander&& other) = delete; // move assignment operator
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
	Seek(const Seek& other) = delete; //copy constructor
	Seek& operator=(const Seek& other) = delete; // assignment operator
	Seek(Seek&& other) = delete; // move constructor
	Seek& operator=(Seek&& other) = delete; // move assignment operator
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
	Face(const Face& other) = delete; //copy constructor
	Face& operator=(const Face& other) = delete; // assignment operator
	Face(Face&& other) = delete; // move constructor
	Face& operator=(Face&& other) = delete; // move assignment operator
	virtual ~Face() = default;

	SteeringPlugin_Output CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface) override;
};

/////////////////////////////////////////
////Turn
////****
class Turn : public ISteeringBehavior
{
public:
	Turn() = default;
	Turn(const Turn& other) = delete; //copy constructor
	Turn& operator=(const Turn& other) = delete; // assignment operator
	Turn(Turn&& other) = delete; // move constructor
	Turn& operator=(Turn&& other) = delete; // move assignment operator
	virtual ~Turn() = default;

	SteeringPlugin_Output CalculateSteering(SteeringPlugin_Output& steering, Agent* pAgent, IExamInterface* pInterface) override;
};

/////////////////////////////////////////
////FLEE
////****
class Flee : public ISteeringBehavior
{
public:
	Flee() = default;
	Flee(const Flee& other) = delete; //copy constructor
	Flee& operator=(const Flee& other) = delete; // assignment operator
	Flee(Flee&& other) = delete; // move constructor
	Flee& operator=(Flee&& other) = delete; // move assignment operator
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


