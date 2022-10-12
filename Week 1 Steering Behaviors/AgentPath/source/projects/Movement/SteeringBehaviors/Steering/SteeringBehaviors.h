/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "../SteeringHelpers.h"
class SteeringAgent;
class Obstacle;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) = 0;

	//Seek Functions
	void SetTarget(const TargetData& target) { m_Target = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	TargetData m_Target;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

//Flee

class Flee : public ISteeringBehavior
{
public:
	Flee() = default;
	virtual ~Flee() = default;
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
private:

};

class Arrive : public ISteeringBehavior
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};



class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetWanderOffSet(float offset) { m_OffsetDistance = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetMaxAngleChange(float rad) { m_MaxAngleChange = rad; }

protected:
	float m_OffsetDistance = 6.0f; // OffSet (Agent Direction)
	float m_Radius = 4.0f; // WanderRadius
	float m_MaxAngleChange = Elite::ToRadians(90); //Max WanderAngle change per frame
	float m_WanderAngle = 0.f; //Interval


	bool m_IsStarted{ false };
};

class Evade : public Flee
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetEvadeRadius(float radius) { m_EvadeRadius = radius; }
	void SetLookAheadSeconds(float seconds) { m_LookAheadSeconds = seconds; }

protected:
	float m_EvadeRadius = 8.0f;
	float m_LookAheadSeconds = 5.0f; 
};


#endif



