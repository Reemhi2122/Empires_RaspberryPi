#include "../HelloCubes/Headers/utilities/XInput.h"

XInput::XInput() {}

XInput::~XInput() {}

XInput::XInput(Display* a_Display, Window a_Window)
{
    m_Display = a_Display;
    m_Window = a_Window;
}

void XInput::InputCheck(int* a_KeyCode, int* a_CurScroll)
{
    //Select input from display / window
    int state;
    XGetInputFocus(m_Display, &m_Window, &state);

    XSelectInput(m_Display, m_Window,   KeyPressMask | KeyReleaseMask | 
                                        ButtonPressMask | ButtonReleaseMask);
    
    XGrabPointer(m_Display, m_Window, False, ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
                 GrabModeAsync, None, None, CurrentTime);

    //Goes through every input event in frame
    while (XPending(m_Display)) {
        XEvent KeyEvent;
        XNextEvent(m_Display, &KeyEvent);
        
        switch (KeyEvent.type)
        {
        case KeyPress:
            *a_KeyCode = XLookupKeysym(&KeyEvent.xkey, 0);
            break;
        case KeyRelease:
            break;
        case ButtonPress:
            m_MouseButtonState = true;

            switch (KeyEvent.xbutton.button){
            case Button4:
                *a_CurScroll = 1;
                break;
            case Button5:
                *a_CurScroll = -1;
                break;
            }

            break;
        case ButtonRelease:
            m_MouseButtonState = false;
            break;
        default:
            *a_KeyCode = 0;
            break;
        }
    }
}

bool XInput::GetMouseButtonState()
{
    return m_MouseButtonState;
}

glm::vec2 XInput::GetMousePos()
{
    XQueryPointer(m_Display, m_Window, &m_ReturnWindow,
        &m_ReturnWindow, &m_RootX, &m_RootY, &m_WinX, &m_WinY,
        &m_MaskReturn);

    return glm::vec2(m_WinX, m_WinY);
}