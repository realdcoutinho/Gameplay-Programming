#include "stdafx.h"
#include "Flock.h"

#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"

using namespace Elite;

//Constructor & Destructor
Flock::Flock(
	int flockSize /*= 50*/, 
	float worldSize /*= 100.f*/, 
	SteeringAgent* pAgentToEvade /*= nullptr*/, 
	bool trimWorld /*= false*/)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ flockSize }
	, m_TrimWorld { trimWorld }
	, m_pAgentToEvade{pAgentToEvade}
	, m_NeighborhoodRadius{ 15 }
	, m_NrOfNeighbors{0}
{

	// TODO: initialize the flock and the memory pool
	m_pSeekBehavior = new Seek();
	m_pWanderBehavior = new Wander();
	m_pCohesionBehavior = new Cohesion(this);
	m_pSeparationBehavior = new Separation(this);


	m_Agents.resize(m_FlockSize);
	m_Neighbors.resize(m_FlockSize);
	for (int i{}; i < flockSize; ++i)
	{
		m_Agents[i] = new SteeringAgent();
		m_Agents[i]->SetSteeringBehavior(m_pCohesionBehavior);
		m_Agents[i]->SetPosition(Elite::Vector2(randomFloat(0, m_WorldSize), randomFloat(0, m_WorldSize)));
		m_Agents[i]->SetMaxLinearSpeed(15.0f);
		m_Agents[i]->SetAutoOrient(true);
		////pAgent->SetBodyColor({ 1, 0, 0, 0 });
		m_Agents[i]->SetMass(0.3f);
	}
}

Flock::~Flock()
{
	// TODO: clean up any additional data
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pWanderBehavior);
	SAFE_DELETE(m_pCohesionBehavior);
	SAFE_DELETE(m_pSeparationBehavior);


	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pPrioritySteering);
	



	for(auto pAgent: m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();
}

void Flock::Update(float deltaT)
{
	// TODO: update the flock
	// loop over all the agents
		// register its neighbors	(-> memory pool is filled with neighbors of the currently evaluated agent)
	for (size_t index{}; index < m_Agents.size(); ++index)
	{
		RegisterNeighbors(m_Agents[index]);
		m_Agents[index]->Update(deltaT);
		if (m_TrimWorld)
		{
			// Trim the agent to the world
			m_Agents[index]->TrimToWorld(m_WorldSize);
		}
	}
		// update it				(-> the behaviors can use the neighbors stored in the pool, next iteration they will be the next agent's neighbors)

}

void Flock::Render(float deltaT)
{
	for (size_t index{}; index < m_Agents.size(); ++index)
	{
		m_Agents[index]->Render(deltaT);
		m_Agents[index]->SetRenderBehavior(true);
	}
}


void Flock::UpdateAndRenderUI()
{
	//Setup
	int menuWidth = 235;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);

	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Text("LMB: place target");
	ImGui::Text("RMB: move cam.");
	ImGui::Text("Scrollwheel: zoom cam.");
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Flocking");
	ImGui::Spacing();

	// TODO: Implement checkboxes for debug rendering and weight sliders here

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	
}

void Flock::RegisterNeighbors(SteeringAgent* pAgent)
{
	// TODO: Implement
	Elite::Vector2 neighborhoodCenter{ pAgent->GetPosition() + pAgent->GetPosition() * m_NeighborhoodRadius };
	
	size_t nrOfAgents{ m_Agents.size() };

	m_NrOfNeighbors = 0;

	for (size_t index{}; index < nrOfAgents; ++index)
	{
		if (IsPointInCircle(m_Agents[index]->GetPosition(), neighborhoodCenter, m_NeighborhoodRadius))
			m_Neighbors[m_NrOfNeighbors] = m_Agents[index];
			
		++m_NrOfNeighbors;
	}
	std::cout << m_NrOfNeighbors << '\n';
}

Elite::Vector2 Flock::GetAverageNeighborPos() const
{
	// TODO: Implement
	Elite::Vector2 initalVector{};
	
	for (int index{}; index < m_NrOfNeighbors; ++index)
	{
		if (m_Neighbors[index] != NULL)
		{
			initalVector += m_Neighbors[index]->GetPosition();
		}
	}
	initalVector /= static_cast<float>(m_NrOfNeighbors);

	return initalVector;
}

Elite::Vector2 Flock::GetAverageNeighborVelocity() const
{
	// TODO: Implement
	Elite::Vector2 initalVelocity{};

	for (int index{}; index < m_NrOfNeighbors; ++index)
	{
		if (m_Neighbors[index] != NULL)
		{
			initalVelocity += m_Neighbors[index]->GetLinearVelocity();
		}
	}
	initalVelocity /= static_cast<float>(m_NrOfNeighbors);

	return initalVelocity;
}

void Flock::SetTarget_Seek(TargetData target)
{
	// TODO: Set target for seek behavior
	m_pSeekBehavior->SetTarget(target);
}


float* Flock::GetWeight(ISteeringBehavior* pBehavior) 
{
	if (m_pBlendedSteering)
	{
		auto& weightedBehaviors = m_pBlendedSteering->GetWeightedBehaviorsRef();
		auto it = find_if(weightedBehaviors.begin(),
			weightedBehaviors.end(),
			[pBehavior](BlendedSteering::WeightedBehavior el)
			{
				return el.pBehavior == pBehavior;
			}
		);

		if(it!= weightedBehaviors.end())
			return &it->weight;
	}

	return nullptr;
}
