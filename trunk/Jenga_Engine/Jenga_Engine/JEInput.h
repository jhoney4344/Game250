/*****************************************************************************/
/*!
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

\file JEInput.h
\author Sunghak Jin
\par email: shjin920\@gmail.com
\brief 
header file for JEInput.cpp
*/
/*****************************************************************************/
#ifndef JENGA_ENGINE_INPUT
#define JENGA_ENGINE_INPUT
#include "JESTL.h"//windows
void InitWinHeight(int _height,int _width);
int GetWinHeight();
int GetWinWidth();


//keycodes
enum JEKeyCode
{
    JE_INVALID,
    JE_BACKSPACE,
    JE_TAB,
    JE_RETURN,
    JE_SHIFT,
    JE_CAPS_LOCK,
    JE_ESCAPE,
    JE_SPACE,
    JE_ARROW_UP,
    JE_ARROW_DOWN,
    JE_ARROW_LEFT,
    JE_ARROW_RIGHT,
    JE_0,
    JE_1,
    JE_2,
    JE_3,
    JE_4,
    JE_5,
    JE_6,
    JE_7,
	JE_8,
	JE_9,
	JE_NUMPAD0,
	JE_NUMPAD1,
	JE_NUMPAD2,
	JE_NUMPAD3,
	JE_NUMPAD4,
	JE_NUMPAD5,
	JE_NUMPAD6,
	JE_NUMPAD7,
	JE_NUMPAD8,
	JE_NUMPAD9,
	JE_A,
	JE_B,
	JE_C,
	JE_D,
	JE_E,
	JE_F,
	JE_G,
	JE_H,
    JE_I,
    JE_J,
    JE_K,
    JE_L,
    JE_M,
    JE_N,
    JE_O,
    JE_P,
    JE_Q,
    JE_R,
    JE_S,
    JE_T,
    JE_U,
    JE_V,
    JE_W,
    JE_X,
    JE_Y,
    JE_Z,
    JE_F1,
    JE_F2,
    JE_F3,
    JE_F4,
    JE_F5,
    JE_F6,
    JE_F7,
    JE_F8,
    JE_F9,
    JE_F10,
    JE_F11,
    JE_F12,
    JE_SHIFT_LEFT,
    JE_SHIFT_RIGHT,
    JE_CONTROL,
    JE_MOUSE_LEFT,
    JE_MOUSE_RIGHT,
    JE_MOUSE_MIDDLE,
    JE_MOUSE_MIDDLE_UP,
    JE_MOUSE_MIDDLE_DOWN,
    JE_ALT,
    JE_LAST
};

/*****************************************************************************/
/*!
\class JEInput
\brief 
class for managing all the keyboard and mouse inputs
*/
/*****************************************************************************/
class JEInput
{
private:
	bool *pressed; // for checking key pressed
	bool *triggered; // for checking key triggered
	bool *repeating; // for checking key repeating
	bool *iskeyup; // for checking when the key is up

	int m_x; //mouse x, y position
	int m_y;
	
	JEKeyCode* pUnpress; // array of keys to unpress
	int unpressSize; // The number of elements in the stack
	int unpressCapacity; //  The max number of elements in the state
	
public:
	JEInput();
	~JEInput();
	void SetPressed(JEKeyCode, bool);
	void Init(void);
	void ShutDown(void);
	bool IsUp(JEKeyCode);
	bool IsPressed(JEKeyCode);
	bool IsTriggered(JEKeyCode);
	bool IsRepeating(JEKeyCode);
	bool IsAnyPressed(void);
	bool IsAnyTriggered(void);
	int  GetMouse_x(void) const;
	int  GetMouse_y(void) const;
	void SetMouse_x(int);
	void SetMouse_y(int);
    void Reset(void);
	void GrowStack(void);
	void PushKeyCode(JEKeyCode);
};

//extern pointer for input
extern JEInput *g_Input;

//translate the keycodes
JEKeyCode JETranslate(WPARAM);

#endif