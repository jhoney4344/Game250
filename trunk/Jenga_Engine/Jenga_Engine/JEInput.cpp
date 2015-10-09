/*****************************************************************************/
/*!
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

\file JEInput.cpp
\author Sunghak Jin
\par email: shjin920\@gmail.com
\brief 
this is file for keyboard and mouse input
*/
/*****************************************************************************/
#include "JEInput.h"
#include "JEDebug.h" //for debug
#include <cstring>

namespace{
	int windowHeight;
	int windowWidth;
}

void InitWinHeight(int _height, int _width)
{
	windowHeight = _height;
	windowWidth  = _width;
}

int GetWinHeight()
{
	return windowHeight;
}
int GetWinWidth()
{
	return windowWidth;
}

JEInput *g_Input = NULL;

//set all the keys
JEKeyCode JETranslate(WPARAM wp)
{
	switch(wp)
	{
	case VK_RETURN:  return JE_RETURN;
	case VK_SPACE:   return JE_SPACE;
	case VK_UP:      return JE_ARROW_UP;
	case VK_DOWN:    return JE_ARROW_DOWN;
	case VK_RIGHT:   return JE_ARROW_RIGHT;
	case VK_LEFT:    return JE_ARROW_LEFT;
	case VK_TAB:     return JE_TAB;
	case VK_SHIFT:   return JE_SHIFT;
	case VK_ESCAPE:  return JE_ESCAPE;
	case VK_BACK:    return JE_BACKSPACE;
	case 'A':        return JE_A;
	case 'B':        return JE_B;
	case 'C':        return JE_C;
	case 'D':        return JE_D;
	case 'E':        return JE_E;
	case 'F':        return JE_F;
	case 'G':        return JE_G;
	case 'H':        return JE_H;
	case 'I':        return JE_I;
	case 'J':        return JE_J;
	case 'K':        return JE_K;
	case 'L':        return JE_L;
	case 'M':        return JE_M;
	case 'N':        return JE_N;
	case 'O':        return JE_O;
	case 'P':        return JE_P;
	case 'Q':        return JE_Q;
	case 'R':        return JE_R;
	case 'S':        return JE_S;
	case 'T':        return JE_T;
	case 'U':        return JE_U;
	case 'V':        return JE_V;
	case 'W':        return JE_W;
	case 'X':        return JE_X;
	case 'Y':        return JE_Y;
	case 'Z':        return JE_Z;
	case '0':        return JE_0;
	case '1':        return JE_1;
	case '2':        return JE_2;
	case '3':        return JE_3;
	case '4':        return JE_4;
	case '5':        return JE_5;
	case '6':        return JE_6;
	case '7':        return JE_7;
	case '8':        return JE_8;
	case '9':        return JE_9;
	case VK_NUMPAD0: return JE_NUMPAD0;
	case VK_NUMPAD1: return JE_NUMPAD1;
	case VK_NUMPAD2: return JE_NUMPAD2;
	case VK_NUMPAD3: return JE_NUMPAD3;
	case VK_NUMPAD4: return JE_NUMPAD4;
	case VK_NUMPAD5: return JE_NUMPAD5;
	case VK_NUMPAD6: return JE_NUMPAD6;
	case VK_NUMPAD7: return JE_NUMPAD7;
	case VK_NUMPAD8: return JE_NUMPAD8;
	case VK_NUMPAD9: return JE_NUMPAD9;
	case VK_F1:      return JE_F1;
	case VK_F2:      return JE_F2;
	case VK_F3:      return JE_F3;
	case VK_F4:      return JE_F4;
	case VK_F5:      return JE_F5;
	case VK_F6:      return JE_F6;
	case VK_F7:      return JE_F7;
	case VK_F8:      return JE_F8;
	case VK_F9:      return JE_F9;
	case VK_F10:     return JE_F10;
	case VK_F11:     return JE_F11;
	case VK_F12:     return JE_F12;
	case VK_LSHIFT:  return JE_SHIFT_LEFT;
	case VK_RSHIFT:  return JE_SHIFT_RIGHT;
	case VK_CONTROL: return JE_CONTROL;
	case VK_LBUTTON: return JE_MOUSE_LEFT;
	case VK_RBUTTON: return JE_MOUSE_RIGHT;
	case VK_MENU:   return JE_ALT;
	default:        return JE_INVALID;
	}

}

/*****************************************************************************/
/*!
constructor
\return 
Nothing
\brief 
basic constructor. allocate memories for all pointers
*/
/*****************************************************************************/
JEInput::JEInput()
{
	pressed = new bool[JE_LAST];
	repeating = new bool[JE_LAST];
	triggered = new bool[JE_LAST];
	iskeyup = new bool[JE_LAST];
	g_Input = this;
}

/*****************************************************************************/
/*!
destructor
\return 
Nothing
\brief 
basic destructor. deallocate memories for all pointers
*/
/*****************************************************************************/
JEInput::~JEInput()
{
	delete []pressed;
	delete []repeating;
	delete []triggered;
	delete []iskeyup;
}

/*****************************************************************************/
/*!
Initialize all the members
\return 
Nothing
\brief 
Initialize all the members
*/
/*****************************************************************************/
void JEInput::Init(void)
{
	const int START_COUNT = 4;
	pUnpress = (JEKeyCode*)malloc(START_COUNT*sizeof(JEKeyCode));

	unpressCapacity = START_COUNT;
	unpressSize = 0;
	m_x = 0;
	m_y = 0;

	/*Clear arrays*/
	for(int i = JE_INVALID; i < JE_LAST; ++i)
	{
		pressed[i] = false;
		repeating[i] = false;
		triggered[i] = false;
	}
}

/*****************************************************************************/
/*!
free memory
\return 
Nothing
\brief 
clear resources
*/
/*****************************************************************************/
void JEInput::ShutDown(void)
{
	free(pUnpress);
	pUnpress = 0;
}

/*****************************************************************************/
/*!
free memory
\return 
Nothing
\brief 
clear resources
*/
/*****************************************************************************/
void JEInput::SetPressed(JEKeyCode key, bool status)
{
	char keyID = 0;

	if(key < JE_INVALID || key >= JE_LAST)
		return;

	if(status) // If the key is pressed
	{
		switch (key)
		{
		case JE_NUMPAD0: keyID = '0';
			break;
		case JE_NUMPAD1: keyID = '1';
			break;
		case JE_NUMPAD2: keyID = '2';
			break;
		case JE_NUMPAD3: keyID = '3';
			break;
		case JE_NUMPAD4: keyID = '4';
			break;
		case JE_NUMPAD5: keyID = '5';
			break;
		case JE_NUMPAD6: keyID = '6';
			break;
		case JE_NUMPAD7: keyID = '7';
			break;
		case JE_NUMPAD8: keyID = '8';
			break;
		case JE_NUMPAD9: keyID = '9';
			break;
		case JE_A: keyID = 'A';
			break;
		case JE_B: keyID = 'B';
			break;
		case JE_C: keyID = 'C';
			break;
		case JE_D: keyID = 'D';
			break;
		case JE_E: keyID = 'E';
			break;
		case JE_F: keyID = 'F';
			break;
		case JE_G: keyID = 'G';
			break;
		case JE_H: keyID = 'H';
			break;
		case JE_I: keyID = 'I';
			break;
		case JE_J: keyID = 'J';
			break;
		case JE_K: keyID = 'K';
			break;
		case JE_L: keyID = 'L';
			break;
		case JE_M: keyID = 'M';
			break;
		case JE_N: keyID = 'N';
			break;
		case JE_O: keyID = 'O';
			break;
		case JE_P: keyID = 'P';
			break;
		case JE_Q: keyID = 'Q';
			break;
		case JE_R: keyID = 'R';
			break;
		case JE_S: keyID = 'S';
			break;
		case JE_T: keyID = 'T';
			break;
		case JE_U: keyID = 'U';
			break;
		case JE_V: keyID = 'V';
			break;
		case JE_W: keyID = 'W';
			break;
		case JE_X: keyID = 'X';
			break;
		case JE_Y: keyID = 'Y';
			break;
		case JE_Z: keyID = 'Z';
			break;
		default:        break;
			//JE_1,
			//JE_2,
			//JE_3,
			//JE_4,
			//JE_5,
			//JE_6,
			//JE_7,
			//JE_8,
			//JE_9,
			//JE_TAB,
			//JE_RETURN,
			//JE_SHIFT,
			//JE_CAPS_LOCK,
			//JE_ESCAPE,
			//JE_SPACE,
			//JE_ARROW_UP,
			//JE_ARROW_DOWN,
			//JE_ARROW_LEFT,
			//JE_ARROW_RIGHT,
			//JE_F1,
			//JE_F2,
			//JE_F3,
			//JE_F4,
			//JE_F5,
			//JE_F6,
			//JE_F7,
			//JE_F8,
			//JE_F9,
			//JE_F10,
			//JE_F11,
			//JE_F12,
			//JE_SHIFT_LEFT,
			//JE_SHIFT_RIGHT,
			//JE_CONTROL,
			//JE_MOUSE_LEFT,
			//JE_MOUSE_RIGHT,
			//JE_MOUSE_MIDDLE,
			//JE_MOUSE_MIDDLE_UP,
			//JE_MOUSE_MIDDLE_DOWN,
			//JE_ALT,
			//JE_LAST
		};

		//std::cout << "key down" << std::endl; // for debug
		if(pressed[key]) // if the key was already pressed
		{
			repeating[key] = true;
			//std::cout << "and it's Repeating" << std::endl; // for debug
		}

		else // The key was not already pressed
		{
			pressed[key]   = true;
			triggered[key] = true;
			//std::cout << "and it's Triggered" << std::endl; // for debug
		}

	}

	else //If the key is not pressed
	{
		PushKeyCode(key);
		iskeyup[key] = true;
		//std::cout << "key up" << std::endl; //for debug
	}
}

/*****************************************************************************/
/*!
check when the key is up
\param key
key the user want to enter
\return 
true or false
\brief 
check when the key is up
*/
/*****************************************************************************/
bool JEInput::IsUp(JEKeyCode key)
{
	if((key < JE_INVALID) || (key >= JE_LAST))
	{
		JEPRINTF("Key is out of range\n");
		
		return 0;
	}
	return iskeyup[key];
}

/*****************************************************************************/
/*!
check when the key is pressed
\param key
key the user want to enter
\return 
true or false
\brief 
check when the key is pressed
*/
/*****************************************************************************/
bool JEInput::IsPressed(JEKeyCode key)
{
	if((key < JE_INVALID) || (key >= JE_LAST))
	{
		JEPRINTF("Key is out of range\n");
		
		return 0;
	}

	return pressed[key];
}

/*****************************************************************************/
/*!
check when the key is triggered
\param key
key the user want to enter
\return 
true or false
\brief 
check when the key is triggered
*/
/*****************************************************************************/
bool JEInput::IsTriggered(JEKeyCode key)
{
	if((key < JE_INVALID) || (key >= JE_LAST))
	{
		JEPRINTF("Key is out of range\n");
		
		return 0;
	}

	return triggered[key];
}

/*****************************************************************************/
/*!
check when the key is repeating
\param key
key the user want to enter
\return 
true or false
\brief 
check when the key is repeating
*/
/*****************************************************************************/
bool JEInput::IsRepeating(JEKeyCode key)
{
	if((key < JE_INVALID) || (key >= JE_LAST))
	{
		JEPRINTF("Key is out of range\n");
		return 0;
	}

	return repeating[key];
}

/*****************************************************************************/
/*!
check when the anykey is pressed
\return 
true or false
\brief 
check when the anykey is pressed
*/
/*****************************************************************************/
bool JEInput::IsAnyPressed(void)
{
	for (int i = JE_INVALID; i < JE_LAST; ++i)
	{
		if(pressed[i])
			return true;
	}

	return false;
}

/*****************************************************************************/
/*!
check when the anykey is triggered
\return 
true or false
\brief 
check when the anykey is triggered
*/
/*****************************************************************************/
bool JEInput::IsAnyTriggered(void)
{
	for (int i = JE_INVALID; i < JE_LAST; ++i)
	{
		if(triggered[i])
			return true;
	}

	return false;
}

/*****************************************************************************/
/*!
get x value of mouse position
\return 
mouse position(int value)
\brief 
get x value of mouse position
*/
/*****************************************************************************/
int JEInput::GetMouse_x(void) const
{ 
	return m_x;
}

/*****************************************************************************/
/*!
get y value of mouse position
\return 
mouse position(int value)
\brief 
get y value of mouse position
*/
/*****************************************************************************/
int JEInput::GetMouse_y(void) const
{
	return  windowHeight - m_y;
}

/*****************************************************************************/
/*!
set x value of mouse position
\param x
x position
\return 
nothing
\brief 
set x value of mouse position
*/
/*****************************************************************************/
void JEInput::SetMouse_x(int x)
{
	JE_UNUSED_PARAM(x);
	m_x = x;
	//std::cout<< "x position is " << x <<std::endl; // for debug
}

/*****************************************************************************/
/*!
set y value of mouse position
\param y
y position
\return 
nothing
\brief 
set y value of mouse position
*/
/*****************************************************************************/
void JEInput::SetMouse_y(int y)
{
	JE_UNUSED_PARAM(y);
	m_y = y;
	//std::cout<< "y position is " << y <<std::endl; // for debug
}

/*****************************************************************************/
/*!
reset keys
\return 
nothing
\brief 
reset keys
*/
/*****************************************************************************/
void JEInput::Reset(void)
{
	int i;/*for loop variable*/

	// Only reset the pressed values that are not being pressed
	for(i = 0; i < unpressSize; ++i)
		pressed[pUnpress[i]] = 0; // Reset the pressed value

	unpressSize = 0;

	// reset my triggered and repeating, iskeyup
	for(i = 0; i < JE_LAST; ++i)
	{
		triggered[i] = false;
		repeating[i] = false;
		iskeyup[i] = false;
	}

}

/*****************************************************************************/
/*!
function to help stacking
\return 
nothing
\brief 
function to help stacking
*/
/*****************************************************************************/
void JEInput::GrowStack(void)
{
	int i;  // For the for loop
	JEKeyCode* pNewStack;

	unpressCapacity *= 2;

	// Allocate new data
	pNewStack = (JEKeyCode*)malloc(unpressCapacity * sizeof(JEKeyCode));  

	// Copy data
	for(i = 0; i < unpressSize; ++i)
		pNewStack[i] = pUnpress[i];

	// Clear old Resources
	free(pUnpress);

	pUnpress = pNewStack;
}

/*****************************************************************************/
/*!
function to add keycode to stack
\param key
key the user want to enter
\return 
nothing
\brief 
function to add keycode to stack
*/
/*****************************************************************************/
void JEInput::PushKeyCode(JEKeyCode key)
{
	if(unpressSize == unpressCapacity)
		GrowStack();

	pUnpress[unpressSize] = key;

	++unpressSize;
}