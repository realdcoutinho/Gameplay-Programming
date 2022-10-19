#include "stdafx.h"
#include "Flock.h"


#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"

#include "../SpacePartitioning/SpacePartitioning.h"

using namespace Elite;

//Constructor & Destructor
Flock::Flock(
	int flockSize /*= 50*/, 
	float worldSize /*= 100.f*/, 
	SteeringAgent* pAgentToEvade /*= nullptr*/, 
	bool trimWorld /*= false*/)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ 4000 }
	, m_TrimWorld { trimWorld }
	, m_pAgentToEvade{pAgentToEvade}
	, m_NeighborhoodRadius{ 5 }
	, m_NrOfNeighbors{0}
{
	// TODO: initialize the flock and the memory pool
	InitializeFlock();
	InitializeEvadingAgent();
	InitializeCellSpace();
}

#pragma region initializations
void Flock::InitializeFlock()
{
	m_Agents.resize(m_FlockSize);
	m_Neighbors.resize(m_FlockSize);

	m_pSeekBehavior = new Seek();
	m_pWanderBehavior = new Wander();
	m_pCohesionBehavior = new Cohesion(this);
	m_pSeparationBehavior = new Separation(this);
	m_pVelMatchBehavior = new VelocityMatch(this);
	m_pEvadeBehavior = new Evade();

	m_pBlendedSteering = new BlendedSteering({
		{m_pCohesionBehavior, 0.3f},
		{m_pSeparationBehavior, 0.3f},
		{m_pSeekBehavior, 0.0f},
		{m_pWanderBehavior, 0.75f},
		{m_pVelMatchBehavior, 0.0f}
		});
	m_pPrioritySteering = new PrioritySteering({
		m_pEvadeBehavior,
		m_pBlendedSteering
		});

	for (int i{}; i < m_FlockSize; ++i)
	{
		m_Agents[i] = new SteeringAgent();
		m_Agents[i]->SetSteeringBehavior(m_pPrioritySteering);
		m_Agents[i]->SetPosition(Elite::Vector2(randomFloat(0, m_WorldSize), randomFloat(0, m_WorldSize)));
		m_Agents[i]->SetOldPosition(m_Agents[i]->GetPosition());
		m_Agents[i]->SetMaxLinearSpeed(m_MaxAgentVelocity);
		m_Agents[i]->SetAutoOrient(true);
		m_Agents[i]->SetMass(0.3f);
		m_Agents[i]->SetLinearVelocity({ randomVector2(-m_MaxAgentVelocity,m_MaxAgentVelocity) });
	}
	int m_DebugAgentIndexPosition{ static_cast<int>(randomFloat(0.0f, static_cast<float>(m_FlockSize))) };
}

void Flock::InitializeEvadingAgent()
{
	m_pAgentToEvade = new SteeringAgent();
	m_pAgentToEvade->SetPosition(Elite::Vector2{ randomFloat(-m_WorldSize, m_WorldSize), randomFloat(-m_WorldSize, m_WorldSize) });
	m_pAgentToEvade->SetSteeringBehavior(m_pWanderBehavior);
	m_pAgentToEvade->SetMaxLinearSpeed(m_MaxEvadeAgentVelocity);
	m_pAgentToEvade->SetAutoOrient(true);
	m_pAgentToEvade->SetBodyColor({ 1, 0, 0, 0 });
	m_pAgentToEvade->SetLinearVelocity({ randomVector2(-m_MaxEvadeAgentVelocity,m_MaxEvadeAgentVelocity) });
	m_pAgentToEvade->SetMass(1.0f);
	m_pEvadeBehavior->SetFleeRadious(m_EvadeAgentRadius); //change it to evade once you complte the evade behaviour
}

void Flock::InitializeCellSpace()
{
	m_CellSpace = new CellSpace(m_WorldSize, m_WorldSize, m_NrOfRowsPartition, m_NrOfColumsPartition, m_FlockSize);
	m_CellSpace->SetDebugAgent(m_Agents[m_DebugAgentIndexPosition]);
	m_CellSpace->SetDebugAgentNeighborhoodRadius(m_NeighborhoodRadius);

	for (int index{}; index < m_FlockSize; ++index)
	{
		m_CellSpace->AddAgent(m_Agents[index]);
	}
}


#pragma endregion

#pragma region updates
void Flock::Update(float deltaT)
{
	UpdateAgents(deltaT);
	UpdateEvadingAgent(deltaT);
}

void Flock::UpdateEvadingAgent(float deltaT)
{
	m_pAgentToEvade->Update(deltaT); //updates it
	m_pAgentToEvade->TrimToWorld(m_WorldSize); //trims to worls (same as for all other agents)
	m_pEvadeBehavior->SetTarget(m_pAgentToEvade->GetPosition()); //passes the evadeTarget to the setTarget fucntion
}

void Flock::UpdateAgents(float deltaT)
{
	for (size_t index{}; index < m_Agents.size(); ++index)
	{
		// register its neighbors	(-> memory pool is filled with neighbors of the currently evaluated agent)
		if(m_CanDebugRenderPartitions)
			m_CellSpace->UpdateAgentCell(m_Agents[index], m_Agents[index]->GetOldPosition());

		RegisterNeighbors(m_Agents[index]);
		// update it				
		m_Agents[index]->SetOldPosition(m_Agents[index]->GetPosition());
		m_Agents[index]->Update(deltaT); //(->the behaviors can use the neighbors stored in the pool, next iteration they will be the next agent's neighbors)


		if (m_TrimWorld)
			m_Agents[index]->TrimToWorld(m_WorldSize);
	}
	m_DebugAgentWorldPosition = m_Agents[m_DebugAgentIndexPosition]->GetPosition(); //Updates the position of the agent debugger so that multiple function dont have to. 
}

void Flock::RegisterNeighbors(SteeringAgent* pAgent)
{
	m_NrOfNeighbors = 0;

	if (m_CanDebugRenderPartitions)
	{
		m_CellSpace->RegisterNeighbors(pAgent, m_NeighborhoodRadius);
		m_NrOfNeighbors = m_CellSpace->GetNrOfNeighbors();
		m_Neighbors = m_CellSpace->GetNeighbors();
	}
	if (!m_CanDebugRenderPartitions)
	{
		for (int index{}; index < m_FlockSize; ++index)
		{
			if (m_Agents[index] == pAgent)
				continue;
			if (IsPointInCircle(m_Agents[index]->GetPosition(), pAgent->GetPosition(), m_NeighborhoodRadius))
				m_Neighbors[m_NrOfNeighbors++] = m_Agents[index];
		}
	}
}
#pragma endregion

#pragma region calculations
Elite::Vector2 Flock::GetAverageNeighborPos() const
{
	// TODO: Implement
	Elite::Vector2 initalVector{};

	for (int index{}; index < m_NrOfNeighbors; ++index)
	{
		initalVector += m_Neighbors[index]->GetPosition();
	}
	return initalVector /= static_cast<float>(m_NrOfNeighbors);
}



Elite::Vector2 Flock::GetAverageNeighborVelocity() const
{
	// TODO: Implement
	if (m_NrOfNeighbors == 0)
	{
		return { 0, 0 };
	}
	Elite::Vector2 initalVelocity{};

	for (int index{}; index < m_NrOfNeighbors; ++index)
	{
		initalVelocity += m_Neighbors[index]->GetLinearVelocity();
	}

	return initalVelocity /= static_cast<float>(m_NrOfNeighbors);
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
		if (it != weightedBehaviors.end())
			return &it->weight;
	}
	return nullptr;
}
#pragma endregion


Flock::~Flock()
{
	// TODO: clean up any additional data
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pSeparationBehavior);
	SAFE_DELETE(m_pCohesionBehavior);
	SAFE_DELETE(m_pVelMatchBehavior);
	SAFE_DELETE(m_pWanderBehavior);
	SAFE_DELETE(m_pEvadeBehavior);
	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pPrioritySteering);

	SAFE_DELETE(m_pAgentToEvade);
	m_CellSpace->EmptyCells();
	SAFE_DELETE(m_CellSpace);

	for (auto pAgent : m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();
	//for (auto pnei : m_Neighbors)
	//{
	//	SAFE_DELETE(pnei);
	//}
	//m_Neighbors.clear();
}


#pragma region renders and debugs
void Flock::Render(float deltaT)
{
	m_pAgentToEvade->Render(deltaT);



	if(m_CanRenderCells)
		m_CellSpace->RenderCells();

	if (m_CanDebugSpatialPartitioningAgent)
		m_CellSpace->DebugAgent();

	if(m_CanDrawWorldBounds)
		RenderWorldBounds();

	CanDebugRender(deltaT);
	CanDebugEvadeAgent(deltaT);
}

void Flock::CanDebugRender(float deltaT)
{
	if (m_CanDebugRender)
	{
		m_Agents[m_DebugAgentIndexPosition]->Render(deltaT);
		m_Agents[m_DebugAgentIndexPosition]->SetRenderBehavior(true);
		m_Agents[m_DebugAgentIndexPosition]->SetBodyColor({ 1.0f, 1.0f, 1.0f });
		DEBUGRENDERER2D->DrawCircle(m_DebugAgentWorldPosition, m_NeighborhoodRadius, { 1.0f, 1.0f, 1.0f }, 0);
	}
	else
	{
		m_Agents[m_DebugAgentIndexPosition]->SetRenderBehavior(false);
		m_Agents[m_DebugAgentIndexPosition]->SetBodyColor({ 1.0f, 0.0f, 0.0f });
	}
}

void Flock::RenderWorldBounds()
{
	std::vector<Elite::Vector2> points =
	{
		{ 0, m_WorldSize },
		{ m_WorldSize, m_WorldSize },
		{ m_WorldSize, 0 },
		{ 0, 0 }
	};
	DEBUGRENDERER2D->DrawPolygon(&points[0], 4, { 1,0,0,1 }, 0.4f);
}

//void Flock::DebugRenderNeighborhoodAndSteering(float deltaT)
//{
//	for (int i{}; i < m_FlockSize; ++i)
//	{
//		m_Agents[i]->Render(deltaT);
//		if (m_CanDebugRenderNeighborhood)
//		{
//			m_Agents[m_DebugAgentIndexPosition]->Render(deltaT);
//			m_Agents[m_DebugAgentIndexPosition]->SetRenderBehavior(true);
//			m_Agents[m_DebugAgentIndexPosition]->SetBodyColor({ 1.0f, 1.0f, 1.0f });
//			if (m_Agents[i] != m_Agents[m_DebugAgentIndexPosition])
//			{
//				if (IsPointInCircle(m_DebugAgentWorldPosition, m_Agents[i]->GetPosition(), m_NeighborhoodRadius))
//				{
//					//if(m_CanDebugRender)
//					//	m_Agents[i]->SetRenderBehavior(true);
//					
//				}
//				else
//				{
//
//					m_Agents[i]->SetRenderBehavior(false);
//					m_Agents[i]->SetBodyColor({ 1.0f, 1.0f, 0.0f });
//				}
//				DEBUGRENDERER2D->DrawCircle(m_DebugAgentWorldPosition, m_NeighborhoodRadius, { 1.0f, 1.0f, 1.0f }, 0);
//			}
//		}
//		if (!m_CanDebugRenderNeighborhood)
//		{
//			m_Agents[m_DebugAgentIndexPosition]->SetBodyColor({ 1.0f, 1.0f, 0.0f });
//			if (m_Agents[i] != m_Agents[m_DebugAgentIndexPosition])
//			{
//				if (IsPointInCircle(m_DebugAgentWorldPosition, m_Agents[i]->GetPosition(), m_NeighborhoodRadius))
//					m_Agents[i]->SetBodyColor({ 1.0f, 1.0f, 0.0f });
//			}
//		}
//
//
//	}
//	//if (m_CanDebugRenderSteering)
//	//{
//	//	m_Agents[m_DebugAgentIndexPosition]->Render(deltaT);
//	//	m_Agents[m_DebugAgentIndexPosition]->SetRenderBehavior(true);
//	//	m_Agents[m_DebugAgentIndexPosition]->SetBodyColor({ 0.0f, 1.0f, 0.0f });
//	//}
//	//else
//	//{
//	//		m_Agents[m_DebugAgentIndexPosition]->SetRenderBehavior(false);
//	//}
//
//}

void Flock::CanDebugEvadeAgent(float deltaT)
{
	if (m_CanDebugRenderEvadeAgent)
	{
		m_pAgentToEvade->SetRenderBehavior(true);
		DEBUGRENDERER2D->DrawCircle(m_pAgentToEvade->GetPosition(), m_EvadeAgentRadius, Elite::Color(0.0f, 0.0f, 1.0f), 0);
	}
	else
	{
		m_pAgentToEvade->SetRenderBehavior(false);
	}
	m_pAgentToEvade->Render(deltaT);
}
#pragma endregion


void Flock::UpdateAndRenderUI()
{
	//Setup
	int menuWidth = 350;
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

	// TODO: Implement checkboxes for debug rendering and weight sliders here
	ImGui::Text("Flocking");
	ImGui::Spacing();
	ImGui::Checkbox("Debug Rendering", &m_CanDebugRender);
	ImGui::Spacing();
	ImGui::Checkbox("Debug Render Evading Agent", &m_CanDebugRenderEvadeAgent);
	ImGui::Spacing();
	ImGui::Checkbox("World Bounds", &m_CanDrawWorldBounds);
	ImGui::Spacing();
	ImGui::Text("Behavior Weights");
	ImGui::SliderFloat("Cohesion", &m_pBlendedSteering->GetWeightedBehaviorsRef()[0].weight, 0.f, 1.f, "%.2f");
	ImGui::SliderFloat("Seperation", &m_pBlendedSteering->GetWeightedBehaviorsRef()[1].weight, 0.f, 1.f, "%.2f");
	ImGui::SliderFloat("Seek", &m_pBlendedSteering->GetWeightedBehaviorsRef()[2].weight, 0.f, 1.f, "%.2f");
	ImGui::SliderFloat("Wander", &m_pBlendedSteering->GetWeightedBehaviorsRef()[3].weight, 0.f, 1.f, "%.2f");
	ImGui::SliderFloat("Velocity Match", &m_pBlendedSteering->GetWeightedBehaviorsRef()[4].weight, 0.f, 1.0f, "%.2f");
	ImGui::Spacing();
	ImGui::Checkbox("Render Cells", &m_CanRenderCells);
	ImGui::Spacing();
	ImGui::Checkbox("Debug 1 Spatial Partitioning Agent", &m_CanDebugSpatialPartitioningAgent);
	ImGui::Spacing();
	ImGui::Checkbox("Using Spacial Partitioning", &m_CanDebugRenderPartitions);
	ImGui::Spacing();

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	
}




