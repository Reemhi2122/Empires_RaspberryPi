#pragma once
#include "MenuState.h"
#include "utilities/Timer.h"

struct ImGuiIO;
class DataStorage;

class InGameMenuState : public MenuState
{
public:
	InGameMenuState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	~InGameMenuState();

	void HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset) override;

private:
	float delta = 0.3;
	Timer m_Timer_;
	
	DataStorage* m_StorageInstance_ = nullptr;
	int m_PlayerEmpire = 0;

	bool m_WindowInFocus = false;
	int m_AmountOfFighters = 0;
	Planet* m_OldFocus = nullptr;
};
