#include "state_machines/menu_states/EndGameMenuState.h"
#include "state_machines/MenuStateMachine.h"
#include "IMGUI/imgui.h"
#include <iostream>

EndGameMenuState::EndGameMenuState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO,
                                   const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation) :
	MenuState(a_StateMachine, a_IO, a_FragmentShaderLocation, a_VertexShaderLocation)
{
}

EndGameMenuState::~EndGameMenuState()
{
}

void EndGameMenuState::HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset)
{
	std::string formatString;

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;
	
	ImGui::NewFrame();
	{
		ImGui::Begin("End Game Menu", 0, flags);
		ImGui::SetWindowSize({ 180, 200 });
		ImGui::SetWindowPos({10, 10});

		ImGui::Text("Game Ended!");
		ImGui::Text("Thank you for playing Empires!");
		ImGui::End();
	}

	ImGui::Render();
	
}
