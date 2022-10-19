#pragma once
#include "../SteeringHelpers.h"
#include "FlockingSteeringBehaviors.h"


class ISteeringBehavior;
class SteeringAgent;
class BlendedSteering;
class PrioritySteering;
class SpacePartitioning;
class CellSpace;

class Flock final
{
public:
	Flock(
		int flockSize = 1000, 
		float worldSize = 1000.f, 
		SteeringAgent* pAgentToEvade = nullptr, 
		bool trimWorld = false);

	~Flock();

	void Update(float deltaT);
	void UpdateAndRenderUI() ;
	void Render(float deltaT);

	void RegisterNeighbors(SteeringAgent* pAgent);
	int GetNrOfNeighbors() const { return m_NrOfNeighbors; }
	const std::vector<SteeringAgent*>& GetNeighbors() const { return m_Neighbors; }

	Elite::Vector2 GetAverageNeighborPos() const;
	Elite::Vector2 GetAverageNeighborVelocity() const;

	void SetTarget_Seek(TargetData target);
	void SetWorldTrimSize(float size) { m_WorldSize = size; }

private:
	//Datamembers
	int m_FlockSize = 0;
	std::vector<SteeringAgent*> m_Agents;
	std::vector<SteeringAgent*> m_Neighbors;

	//sata members for initialization
	bool m_TrimWorld = false;
	float m_WorldSize = 0.f;
	float m_NeighborhoodRadius = 5.f;
	int m_NrOfNeighbors = 0;
	float m_MaxAgentVelocity{ 60.0f };
	float m_MaxEvadeAgentVelocity{ 40.0f };
	float m_EvadeAgentRadius{ 15 };

	int m_NrOfColumsPartition{ 25 };
	int m_NrOfRowsPartition{ 25 };

	//random debugging agent
	Elite::Vector2 m_DebugAgentWorldPosition{};
	int m_DebugAgentIndexPosition{};
	
	//evading agent
	SteeringAgent* m_pAgentToEvade = nullptr;
	CellSpace* m_CellSpace = nullptr;


	//Steering Behaviors
	Seek* m_pSeekBehavior = nullptr;
	Separation* m_pSeparationBehavior = nullptr;
	Cohesion* m_pCohesionBehavior = nullptr;
	VelocityMatch* m_pVelMatchBehavior = nullptr;
	Wander* m_pWanderBehavior = nullptr;
	Evade* m_pEvadeBehavior = nullptr;
	BlendedSteering* m_pBlendedSteering = nullptr;
	PrioritySteering* m_pPrioritySteering = nullptr;

	float* GetWeight(ISteeringBehavior* pBehaviour);

	void InitializeFlock();
	void InitializeEvadingAgent();
	void InitializeCellSpace();

	void UpdateAgents(float deltaT);
	void UpdateEvadingAgent(float deltaT);

	void DebugNeighborhood(float deltaT);
	void CanDebugRender(float deltaT);
	void CanDebugEvadeAgent(float deltaT);

	void RenderWorldBounds();


	//debugs and renders
	bool m_CanDebugRender{ false };
	bool m_CanDebugRenderEvadeAgent{ false };
	bool m_CanDebugRenderNeighborhood{ false };

	bool m_CanRenderCells{ false };
	bool m_CanDebugSpatialPartitioningAgent{ false };
	bool m_CanDebugRenderPartitions{ true };

private:
	Flock(const Flock& other);
	Flock& operator=(const Flock& other);
};