#include "state_machines/menu_states/MainMenuState.h"
#include "state_machines/MenuStateMachine.h"
#include "IMGUI/imgui.h"
#include <iostream>

#include "utilities/Renderer.h"

MainMenuState::MainMenuState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation,
                             const char* a_VertexShaderLocation): MenuState(
	a_StateMachine, a_IO, a_FragmentShaderLocation, a_VertexShaderLocation)
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset)
{
	float delta = m_StateMachineRef.GetRenderer().GetDeltaTime();
	m_StateMachineRef.GetRenderer().UpdateImGui(a_IO, delta, a_MousePressed, a_MousePos, a_CurInput);
	std::string formatString;
	
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;
	
	ImGui::NewFrame();
	{
		ImGui::Begin("Main Menu", 0, flags);
		ImGui::SetWindowSize({ 180, 100 });
		ImGui::SetWindowPos({ 300, 300 });
		

		ImGui::Spacing();

		ImGui::Spacing();

		if (ImGui::Button("Start Game"))
		{
			m_StateMachineRef.SetCurrentState(m_StateMachineRef.GetGenerationState());
		}
		ImGui::End();
	}
	ImGui::Render();
}
