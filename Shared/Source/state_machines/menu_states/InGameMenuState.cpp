#include "Headers/state_machines/menu_states/InGameMenuState.h"
#include "Headers/state_machines/MenuStateMachine.h"
#include "Headers/utilities/DataStorage.h"
#include "Headers/Camera.h"
#include "IMGUI/imgui.h"
#include <vector>
#include <iostream>

#include "Headers/utilities/Renderer.h"

InGameMenuState::InGameMenuState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation,
                                 const char* a_VertexShaderLocation): MenuState(
	a_StateMachine, a_IO, a_FragmentShaderLocation, a_VertexShaderLocation)
{
	
}

InGameMenuState::~InGameMenuState()
{
}

void InGameMenuState::HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset)
{
	m_Timer_.Reset();

	m_StateMachineRef.GetRenderer().UpdateImGui(a_IO, delta, a_MousePressed, a_MousePos, a_CurInput);
	m_StateMachineRef.GetRenderer().Update(a_IO, a_MousePressed, a_MousePos, a_CurInput, a_ScrollOffset, delta);
	
	std::string formatString;
	DataStorage* storage;
	storage = storage->GetInstance();

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	
	m_StorageInstance_ = m_StorageInstance_->GetInstance();
	
	ImGui::NewFrame();
	{
		Empire* empire = m_StorageInstance_->GetEmpire(m_PlayerEmpire);
		ImGui::Begin(empire->GetName().c_str(), 0, flags);
		if (!m_StorageInstance_->GetEmpire(m_PlayerEmpire)->IsDefeat()) {
			ImGui::SetWindowSize({ 200, 450 });
			ImGui::SetWindowPos({ 10, 110 });
			formatString = "Credits:" + std::to_string(empire->GetCredits());
			ImGui::Text(formatString.c_str());

			ImGui::Spacing();

			formatString = "Cost:" + std::to_string(storage->GetDiscoveryVesselCost());
			ImGui::Text(formatString.c_str());
			if (ImGui::Button("Create Discovery vessel")) { empire->CreateShip(ShipClass::EDiscoveryVessel); }
			formatString = "Cost:" + std::to_string(storage->GetCarrierVesselCost());
			ImGui::Text(formatString.c_str());
			if (ImGui::Button("Create Carrier vessel")) { empire->CreateShip(ShipClass::ECarrierVessel); }
			formatString = "Cost:" + std::to_string(storage->GetSupplyVesselCost());
			ImGui::Text(formatString.c_str());
			if (ImGui::Button("Create Supply vessel")) { empire->CreateShip(ShipClass::ESupplyVessel); }

			ImGui::Spacing();

			formatString = "Discovery Vessels:" + std::to_string(empire->GetDiscoveryVessels()->size());
			ImGui::Text(formatString.c_str());
			formatString = "Supply Vessels:" + std::to_string(empire->GetSupplyVessels()->size());
			ImGui::Text(formatString.c_str());
			formatString = "Carrier Vessels:" + std::to_string(empire->GetCarrierVessel()->size());
			ImGui::Text(formatString.c_str());

			ImGui::Spacing();
			if (ImGui::CollapsingHeader("Closest planets")) {
				ImGui::Indent(20);
				for (int i = 0; i < empire->GetPlanets()->size(); i++)
				{
					for (int x = 0; x < empire->GetPlanets()->at(i)->m_ClosePlanets_.size(); x++)
					{
						Planet* closeplanet = empire->GetPlanets()->at(i)->m_ClosePlanets_.at(x);
						if (closeplanet->GetOwner() == empire) continue;
						if (ImGui::CollapsingHeader(closeplanet->GetName().c_str()))
						{
							formatString = "Tech level: " + std::to_string(closeplanet->GetTechLevel());
							ImGui::Text(formatString.c_str());
							formatString = "Resource level: " + std::to_string(closeplanet->GetResourceLevel());
							ImGui::Text(formatString.c_str());
							formatString = "Resources: " + std::to_string(closeplanet->GetResources());
							ImGui::Text(formatString.c_str());
							formatString = "Fighters: " + std::to_string(closeplanet->GetFighterAmount());
							ImGui::Text(formatString.c_str());

							if (ImGui::Button("Go to planet")) {
								m_StateMachineRef.GetRenderer().GetCamera().SetPosition(closeplanet->GetPosition() + glm::vec3(0, 0, -10));
							}
						}
					}
				}
				ImGui::Unindent(20);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Your Planets"))
			{
				std::vector<Planet*>::iterator it;
				std::vector<Planet*>* planets = empire->GetPlanets();

				for (it = planets->begin(); it != planets->end(); it++)
				{
					ImGui::Indent(20);
					if (ImGui::CollapsingHeader((*it)->GetName().c_str()))
					{
						formatString = "Tech level: " + std::to_string((*it)->GetTechLevel());
						ImGui::Text(formatString.c_str());
						formatString = "Resource level: " + std::to_string((*it)->GetResourceLevel());
						ImGui::Text(formatString.c_str());
						formatString = "Resources: " + std::to_string((*it)->GetResources());
						ImGui::Text(formatString.c_str());
						formatString = "Fighters: " + std::to_string((*it)->GetFighterAmount());
						ImGui::Text(formatString.c_str());

						ImGui::Spacing();
						if (ImGui::Button("Buy Fighter")) {
							if (empire->CreateShip(ShipClass::EFighterShip)) {
								(*it)->AddFighterAmount(1);
							}
						}

						ImGui::Spacing();
						if (ImGui::Button("Go to planet")) {
							m_StateMachineRef.GetRenderer().GetCamera().SetPosition((*it)->GetPosition() + glm::vec3(0, 0, -10));
						}
						if (ImGui::Button("Select planet")) {
							empire->SetSelectedPlanet((*it));
						}
					}
					ImGui::Unindent(20);
				}
			}
		}
		else {
			ImGui::Text("Defeat!");
		}
		ImGui::End();
	}

	flags = 0;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("All Empires:", 0, flags);
	for (int i = 0; i < m_StorageInstance_->max_num_empires; i++)
	{
		glm::vec4 glmtemp = m_StorageInstance_->GetEmpire(i)->GetColor();
		ImVec4 temp = { glmtemp.x, glmtemp.y, glmtemp.z, 1.f };
		ImGui::PushStyleColor(ImGuiCol_Text, temp);
		ImGui::Text(m_StorageInstance_->GetEmpire(i)->GetName().c_str());
		ImGui::PopStyleColor();
	}
	ImGui::End();

	{
		ImGui::Begin("All Planets", 0, flags);
		ImGui::SetWindowSize({ 180, 100 });

		for (int i = 0; i < m_StorageInstance_->GetPlanetStorageSize(); i++) {

			if (m_StorageInstance_->GetPlanet(i)->GetOwner() != nullptr)
			{
				glm::vec4 temp = m_StorageInstance_->GetPlanet(i)->GetOwner()->GetColor();
				ImVec4 StyleColor = { temp.x, temp.y, temp.z, 0.5 };

				ImGui::PushStyleColor(ImGuiCol_Header, StyleColor);
			}

			if (ImGui::CollapsingHeader(m_StorageInstance_->GetPlanet(i)->GetName().c_str()))
			{
				formatString = "Tech level: " + std::to_string(m_StorageInstance_->GetPlanet(i)->GetTechLevel());
				ImGui::Text(formatString.c_str());
				formatString = "Resource level: " + std::to_string(m_StorageInstance_->GetPlanet(i)->GetResourceLevel());
				ImGui::Text(formatString.c_str());
				formatString = "Resources: " + std::to_string(m_StorageInstance_->GetPlanet(i)->GetResources());
				ImGui::Text(formatString.c_str());
				formatString = "Fighters: " + std::to_string(m_StorageInstance_->GetPlanet(i)->GetFighterAmount());
				ImGui::Text(formatString.c_str());

				if (ImGui::Button("Go to planet")) {
					m_StateMachineRef.GetRenderer().GetCamera().SetPosition(m_StorageInstance_->GetPlanet(i)->GetPosition() + glm::vec3(0, 0, -10));
				}
			}

			if (m_StorageInstance_->GetPlanet(i)->GetOwner() != nullptr)
			{
				ImGui::PopStyleColor();
			}
		}
		ImGui::End();
	}

	// End Game Button
	{
		ImGui::Begin("Main Menu", 0, flags);
		ImGui::SetWindowSize({ 250, 100 });
		ImGui::SetWindowPos({ 10, 10 });
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("End Game"))
		{
			m_StateMachineRef.SetCurrentState(m_StateMachineRef.GetEndGameMenuState());
		}
		ImGui::End();
	}

	flags |= ImGuiWindowFlags_NoCollapse;

	Planet* a_Planet = m_StateMachineRef.GetRenderer().GetCurrentPlanet();
	
	if (m_WindowInFocus) a_Planet = m_OldFocus;
	if (a_Planet != nullptr || m_WindowInFocus) {
		ImGui::Begin(a_Planet->GetName().c_str(), 0, flags);
		ImGui::SetWindowSize({ 2, 150 });

		if (!m_WindowInFocus) ImGui::SetWindowPos(ImGui::GetMousePos());

		std::string string = "Fighters: " + std::to_string(a_Planet->GetFighterAmount());
		ImGui::Text(string.c_str());

		if (a_Planet->GetOwner() != nullptr){
			string = "Credits: " + std::to_string(a_Planet->GetOwner()->GetCredits());
			ImGui::Text(string.c_str());
		}
	
		string = "Resources: " + std::to_string(a_Planet->GetResources());
		ImGui::Text(string.c_str());
		string = "TechLevel: " + std::to_string(a_Planet->GetTechLevel());
		ImGui::Text(string.c_str());
		string = "Resources Level: " + std::to_string(a_Planet->GetResourceLevel());
		ImGui::Text(string.c_str());

		ImGui::Spacing();
		m_OldFocus = a_Planet;
		m_WindowInFocus = ImGui::IsWindowHovered() | ImGui::IsMouseDown(0) || ImGui::IsMouseReleased(0);

		if (a_Planet->GetOwner() != storage->GetEmpire(m_PlayerEmpire)) {
			Empire* empire = storage->GetEmpire(m_PlayerEmpire);
			Planet* planet = empire->GetSelectedPlanet();
			if (a_Planet->GetOwner() == nullptr) {
				std::string string = "Overtaken: " + std::to_string(a_Planet->GetColonizationCounter());
				ImGui::Text(string.c_str());
				if (empire->GetFreeDiscoveryVessel() != nullptr) {
					if (ImGui::Button("Explore")) {
						empire->Explore(a_Planet);
					}
				}
				else {
					ImGui::Text("No free discovery ships!");
				}
			}
			else {
				if (empire->GetSelectedPlanet() != nullptr) {
					if (empire->GetFreeCarrierVessel() != nullptr) {
						ImGui::Text("Amount of fighters to attack with:");
						ImGui::SliderInt("", &m_AmountOfFighters, 0, planet->GetFighterAmount());
						if (ImGui::Button("Attack")) {
							empire->Attack(a_Planet, m_AmountOfFighters);
						}
					}
					else {
						ImGui::Text("No free carrier ships!");
					}
				}
				else {
					ImGui::Text("None of your planets selected!");
				}
			}
		}
		ImGui::End();
	}
	
	ImGui::Render();

	m_Timer_.Stop();

	delta = m_Timer_.GetDeltaTime();
}
