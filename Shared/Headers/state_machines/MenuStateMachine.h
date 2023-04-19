#pragma once
#include "menu_states/EndGameMenuState.h"
#include "menu_states/InGameMenuState.h"
#include "menu_states/MainMenuState.h"

class GenerationState;
class Renderer;
class Planet;
class MenuState;

class MenuStateMachine
{
public:
	MenuStateMachine(ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation);
	~MenuStateMachine();

	void HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset) const;
	const MenuState& GetCurrentState() const;
	void SetCurrentState(MenuState& a_State);

	MenuState& GetMainMenuState() const;
	MenuState& GetGenerationState() const;
	MenuState& GetInGameMenuState() const;
	MenuState& GetEndGameMenuState() const;

	Renderer& GetRenderer();

private:
	Renderer* m_MainRenderer;
	
	MenuState* m_CurrentState_ = nullptr;
	MainMenuState* m_MainMenu_;
	GenerationState* m_GenerationState_;
	InGameMenuState* m_InGameMenu_;
	EndGameMenuState* m_EndGameMenu_;
};
