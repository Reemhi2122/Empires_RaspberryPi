#include "state_machines/MenuStateMachine.h"
#include "Headers/objects/Planet.h"
#include "state_machines/menu_states/GenerationState.h"
#include "utilities/Renderer.h"

MenuStateMachine::MenuStateMachine(ImGuiIO& a_IO, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation)
{
	m_MainRenderer = new Renderer();
	m_MainRenderer->Init(a_IO, a_FragmentShaderLocation, a_VertexShaderLocation);
	m_MainMenu_ = new MainMenuState(this, a_IO, a_FragmentShaderLocation, a_VertexShaderLocation);
	m_GenerationState_ = new GenerationState(this, a_IO, a_FragmentShaderLocation, a_VertexShaderLocation);
	m_InGameMenu_ = new InGameMenuState(this, a_IO, a_FragmentShaderLocation, a_VertexShaderLocation);
	m_EndGameMenu_ = new EndGameMenuState(this, a_IO, a_FragmentShaderLocation, a_VertexShaderLocation);
	
	m_CurrentState_ = m_MainMenu_;
}

MenuStateMachine::~MenuStateMachine()
{
	delete m_MainMenu_;
	delete m_InGameMenu_;
	delete m_EndGameMenu_;
}

void MenuStateMachine::HandleMenu(ImGuiIO& a_IO, bool a_MousePressed, glm::vec2 a_MousePos, int a_CurInput, float a_ScrollOffset) const
{
	m_CurrentState_->HandleMenu(a_IO, a_MousePressed, a_MousePos, a_CurInput, a_ScrollOffset);
}

const MenuState& MenuStateMachine::GetCurrentState() const
{
	return *m_CurrentState_;
}

void MenuStateMachine::SetCurrentState(MenuState& a_State)
{
	m_CurrentState_ = &a_State;
}

MenuState& MenuStateMachine::GetMainMenuState() const
{
	return *m_MainMenu_;
}

MenuState& MenuStateMachine::GetGenerationState() const
{
	return *m_GenerationState_;
}

MenuState& MenuStateMachine::GetInGameMenuState() const
{
	return *m_InGameMenu_;
}

MenuState& MenuStateMachine::GetEndGameMenuState() const
{
	return *m_EndGameMenu_;
}

Renderer& MenuStateMachine::GetRenderer()
{
	return *m_MainRenderer;
}

