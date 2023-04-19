#include "state_machines/menu_states/MenuState.h"
#include "state_machines/MenuStateMachine.h"

MenuState::MenuState(MenuStateMachine* a_StateMachine, ImGuiIO& a_IO, const char* a_FragmentShaderLocation,
                     const char* a_VertexShaderLocation) : m_StateMachineRef(*a_StateMachine)
{
	
}

MenuState::~MenuState()
{
}
