/* Hello Cubes
code adapted from from OpenGLï¿½ ES 2.0 Programming Guide
and code snippets from RPI Forum to set up Dispmanx

Presented here for Block C as a basic Framework, Rendering method is sub optimal, please take note of that, You should impliment
your own render systems and shader managment systems when you understand the methods available, workshops will be run to discuss this

Initialises a Raspberry format EGL window for rendering and provides an example a simple shader system

Provides a skeletal framework for a processing loop 
Provides input systems for mouse and keyboard (requires init to work)
(note...Linux key reads are unpredictable, if you find keys are not working use event0 or event 1 or event2.. etc
This is a known issue, we could fix with a listener on all events but other methods are being investigated)

Provides a simple Gui demonstrated on start up which allows resolution changes and info display
Provides Graphic file loading capability via MyFiles.h a wrapper class for stb_image
Provides access to a Physics Library (Bullet) but no examples of usage yet, workshops will be run
GLM is used as a standard maths library, you may use your own if you wish but why???

Recommended course..
Review ObjectModel base class, provide the functions stated
Review CubeModel as a standard object type. 
Provide init update and draw routines

Review the Draw function here in HelloCubes which is the main processing loop...make it work


Project builds as Visual GDB (Raspberry), or Visual GDB (X11)
Also C++ for Linux Builds for Raspberry  or X11
Rpi4 is considered an X11 system, see the info on CMGT regarding setup of systems/

*/

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <vector>

#ifdef RASPBERRY
#include "bcm_host.h"
#endif

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Headers/Graphics.h"
#include "Headers/utilities/Renderer.h"
#include "utilities/XInput.h"

#include "IMGUI/imgui.h"
#include "IMGUI/backends/imgui_impl_opengl3.h"
#include "state_machines/MenuStateMachine.h"

using namespace glm;

Graphics graphics;
Target_State state; 
Target_State* p_state = &state;
XInput* m_XInput = new XInput();
//Renderer* m_Renderer;
MenuStateMachine* m_MainStateMachine;


/*****************************************
Up date all objects shape stored in the MyObjects list

This is however not an optimal way to process objects
updating them, then doing their draw is inefficient
Consider ways to improve this
******************************************/

void Input(int a_Input, int a_CurScroll) {
	switch (a_Input)
	{
	case XK_w:
		m_MainStateMachine->GetRenderer().Input(KeyInput::W);
		break;
	case XK_a:
		m_MainStateMachine->GetRenderer().Input(KeyInput::A);
		break;
	case XK_s:
		m_MainStateMachine->GetRenderer().Input(KeyInput::S);
		break;
	case XK_d:
		m_MainStateMachine->GetRenderer().Input(KeyInput::D);
		break;
	case XK_space:
		m_MainStateMachine->GetRenderer().Input(KeyInput::SPACE);
		break;
	default:
		m_MainStateMachine->GetRenderer().Input(KeyInput::NONE);
		break;
	}
	
	switch (a_CurScroll)
	{
	case 1:
		m_MainStateMachine->GetRenderer().Input(KeyInput::SCROLLUP);
		break;
	case -1:
		m_MainStateMachine->GetRenderer().Input(KeyInput::SCROLLDOWN);
		break;
	default:
		m_MainStateMachine->GetRenderer().Input(KeyInput::NONE);
		break;
	}
}

// Our projects main  Game loop (could be put into a class, hint hint)
void  MainLoop(Target_State *esContext)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL3_Init("#version 300 es");
	//m_Renderer->Init(io, "../../../HelloCubes/Resources/Shaders/defaultshader.frag", "../../../HelloCubes/Resources/Shaders/defaultshader.vert");
	m_MainStateMachine = new MenuStateMachine(io, "../../../HelloCubes/Resources/Shaders/defaultshader.frag",
	                                          "../../../HelloCubes/Resources/Shaders/defaultshader.vert");

	int Counter = 0;
	while (Counter++ < 99999999999999) // for a timed loop check the counter otherwise find another exit condition
	{
			int CurInput = 0;
			int CurScroll = 0;
			m_XInput->InputCheck(&CurInput, &CurScroll);

			glViewport(0, 0, p_state->width, p_state->height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			Input(CurInput, CurScroll);
			ImGui_ImplOpenGL3_NewFrame();
			//m_Renderer->Update(io, m_XInput->GetMouseButtonState(), m_XInput->GetMousePos(), CurInput, 0);
			m_MainStateMachine->HandleMenu(io, m_XInput->GetMouseButtonState(), m_XInput->GetMousePos(), CurInput, 0);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glFlush();
			eglSwapBuffers(esContext->display, esContext->surface);
		}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	delete m_MainStateMachine;
}

int main(int argc, char* argv[])
{
	MyFiles* Handler = new MyFiles();
	graphics.GetDisplay();
	graphics.esInitContext(p_state);
	graphics.init_ogl(p_state, 1024, 768,1024,768); // this is the window size we set , you can set it to anything, the 3rd and 4th params are for older pis'
	
	ImGuiContext* context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	ImGui::StyleColorsDark();

	graphics.GetRes(graphics.ResolutionX, graphics.ResolutionY);

	ObjectData object_data;  // create a single instance of object_data to initially draw all objects with the same data.
	p_state->object_data = &object_data;
	p_state->display;

	m_XInput = new XInput(graphics.GetDisplayPtr(), graphics.GetWindow());
	//m_Renderer = new Renderer();

	MainLoop(p_state);
}
