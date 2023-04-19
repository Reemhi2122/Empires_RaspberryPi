#pragma once
#include "MenuState.h"
class Timer;
class GenerationState : public MenuState
{
public:
	GenerationState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	~GenerationState();

	void HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset) override;
private:
	const char* m_FragShaderLoc;
	const char* m_VertShaderLoc;

	int m_Counter_ = 0;
	int m_Seed = 511;
	float m_Delta = 0.0000003;

	Timer* m_Timer_;
};
