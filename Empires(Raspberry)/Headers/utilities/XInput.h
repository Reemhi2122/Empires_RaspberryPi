#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdint.h>
#include <iostream>
#include <glm/glm.hpp>

class XInput
{
public:
	XInput();
	~XInput();

	/// <summary>
	/// Creates a input system with the current display and window
	/// </summary>
	XInput(Display* a_Display, Window a_Window);

	/// <summary>
	/// Checks input and returns last touched key to int
	/// </summary>
	/// <param name="a_ReturnKeyCode"></param>
	void InputCheck(int* a_ReturnKeyCode, int* a_CurScroll);

	/// <summary>
	/// Gets state of main mouse button
	/// </summary>
	/// <returns></returns>
	bool GetMouseButtonState();

	/// <summary>
	/// Gets the current position to the mouse
	/// </summary>
	/// <returns></returns>
	glm::vec2 GetMousePos();

private:
	Display* m_Display;
	Window m_Window;

	Window m_ReturnWindow;
	int m_RootX, m_RootY;
	int m_WinX, m_WinY;
	unsigned int m_MaskReturn;

	bool m_MouseButtonState = false;
};