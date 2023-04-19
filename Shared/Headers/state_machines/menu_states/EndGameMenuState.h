#pragma once
#include "MenuState.h"

class EndGameMenuState : public MenuState
{
public:
	EndGameMenuState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	~EndGameMenuState();
	
	void HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset) override;
};
