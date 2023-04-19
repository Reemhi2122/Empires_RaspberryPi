#pragma once
#include "glm/vec2.hpp"
struct ImGuiIO;
class MenuStateMachine;
class Camera;
class Planet;

class MenuState
{
public:
	MenuState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	~MenuState();

	virtual void HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset) = 0;
	MenuStateMachine& m_StateMachineRef;

	int m_ImGuiFlags;
};