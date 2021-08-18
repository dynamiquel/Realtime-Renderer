#pragma once

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

/**
 * \brief Responsible for keeping track of user input.
 */
class Input
{
public:
	~Input();

	bool Initialise(HINSTANCE hinstance, HWND hwnd);
	bool Frame();

	/**
	 * \brief Returns true if the given key is pressed down.
	 */
	bool GetKeyDown(const unsigned char key);

	/**
	* \brief Returns true if the given mouse button is pressed down.
	*/
	bool GetMouseButtonDown(const unsigned char key);
	
	/**
	 * \brief Returns the current X position of the cursor.
	 */
	int GetMouseX() { return mouseX; }

	/**
	* \brief Returns the current Y position of the cursor.
	*/
	int GetMouseY() { return mouseY; }

	/**
	 * \brief Returns the delta of the mouse since the last frame.
	 * \param x OUT X delta
	 * \param y OUT Y delta
	 */
	void GetDeltaMouse(int& x, int& y);

private:
	LPDIRECTINPUT8W directInput;
	LPDIRECTINPUTDEVICE8W keyboard;
	LPDIRECTINPUTDEVICE8W mouse;

	unsigned char keyboardState[256];
	DIMOUSESTATE2 mouseState;

	int mouseX;
	int mouseY;

	bool ReadKeyboard();
	bool ReadMouse();
};