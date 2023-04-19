#include "state_machines/menu_states/GenerationState.h"
#include "utilities/Generator.h"
#include "IMGUI/imgui.h"
#include "state_machines/MenuStateMachine.h"
#include "utilities/Renderer.h"

GenerationState::GenerationState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation,
                                 const char* a_VertexShaderLocation) : MenuState(
	a_StateMachine, a_IO, a_FragmentShaderLocation, a_VertexShaderLocation)
{
	m_Timer_ = new Timer();
	m_FragShaderLoc = a_FragmentShaderLocation;
	m_VertShaderLoc = a_VertexShaderLocation;
}

void GenerationState::HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset)
{
	m_Timer_->Reset();
	m_StateMachineRef.GetRenderer().UpdateImGui(a_IO, m_Delta, a_MousePressed, a_MousePos, a_CurInput);
	std::string formatString;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::NewFrame();
	{
		ImGui::Begin("Main Menu", 0, flags);
		ImGui::SetWindowSize({ 500, 300 });
		ImGui::SetWindowPos({ 300, 300 });


		ImGui::Spacing();

		ImGui::Spacing();

		if (m_Counter_ == 0)
		{
			DataStorage* storageInstance;
			storageInstance = storageInstance->GetInstance();
			
			ImGui::Text("Generate Game World?");
			ImGui::SliderInt("Set Amount of Planets", &storageInstance->amount_planets, 500, 8192);
			ImGui::SliderInt("Set Seed", &m_Seed, 0, 5000);

			ImGui::Spacing();
			ImGui::Text("Windows recommended planets: 8192");
			ImGui::Text("Raspberry Pi recommended planets: 500 - 1000");
			ImGui::Spacing();

			if (ImGui::Button("Click to Generate"))
			{
				m_Counter_++;
			}
		}


		if (m_Counter_ == 2)
		{
			ImGui::Text("GameWorld successfully populated!");
			if (ImGui::Button("Start Game"))
			{
				m_StateMachineRef.SetCurrentState(m_StateMachineRef.GetInGameMenuState());
			}
		}

		ImGui::End();
	}
	ImGui::Render();

	if (m_Counter_ == 1)
	{
		Generator main_generator(m_Seed);
		main_generator.GenerateGalaxy(m_FragShaderLoc, m_VertShaderLoc);
		m_Counter_++;
	}

	m_Timer_->Stop();
	m_Delta = m_Timer_->GetDeltaTime();
}
