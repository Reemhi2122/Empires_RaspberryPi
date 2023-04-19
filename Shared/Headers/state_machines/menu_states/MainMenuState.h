#pragma once
#include "MenuState.h"
#include "utilities/Timer.h"

class MainMenuState : public MenuState
{
public:
	MainMenuState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	~MainMenuState();
	
	void HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset) override;

private:
	Timer m_Timer_;
};
