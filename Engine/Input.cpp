#include "Input.h"
#include "Logger.h"

Input::~Input()
{
	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = nullptr;
	}

	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = nullptr;
	}

	if (directInput)
	{
		directInput->Release();
		directInput = nullptr;
	}
}

bool Input::Initialise(HINSTANCE hinstance, HWND hwnd)
{
	for (int i = 0; i < 256; i++)
		keyboardState[i] = false;
	
	// --- Start of DirectInput initialization ---
	// Create the abstract DirectInput connection
	DirectInput8Create(
	    hinstance,
	    DIRECTINPUT_VERSION,
	    IID_IDirectInput8W,
	    (void**)&directInput,
	    nullptr
    );
	if (directInput == nullptr)
	{
		Logger::Log("DirectInput Connection Creation Failed!");
		return false;
	}
	
	// Create the connection to the keyboard device.
	HRESULT result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	if (!FAILED(result))
	{
		keyboard->SetDataFormat(&c_dfDIKeyboard);
		keyboard->SetCooperativeLevel(hwnd,  DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		keyboard->Acquire();
	}
	else
	{
		Logger::Log("DirectInput Keyboard Initialization Failed!");
		return false;
	}

	// Create the connection to the mouse device.
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, nullptr);
	if (!FAILED(result))
	{
		mouse->SetDataFormat(&c_dfDIMouse);
		mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		mouse->Acquire();
	}
	else
	{
		Logger::Log("DirectInput Mouse Initialization Failed!");
		return false;
	}
	// --- End of DirectInput initialization ---

	return true;
}

bool Input::Frame()
{
	// Does all the per-frame processing required for input.
	
	if (!ReadKeyboard())
		Logger::Log(LogSource::Input, LogType::Error, "Unable to read keyboard.");
	
	if (!ReadMouse())
		Logger::Log(LogSource::Input, LogType::Error, "Unable to read mouse.");
	else
	{
		// Keeps the mouse moving based from the last known mouse delta.
		/*mouseX += mouseState.lX;
		mouseY += mouseState.lY;*/
	}

	return true;
}

bool Input::GetKeyDown(const unsigned char key)
{
	return keyboardState[key] & 0x80;
}

bool Input::GetMouseButtonDown(const unsigned char key)
{
	// Out of index.
	if (key > 7)
		return false;

	return mouseState.rgbButtons[key];
}

void Input::GetDeltaMouse(int& x, int& y)
{
	x = mouseState.lX;
	y = mouseState.lY;
}

bool Input::ReadKeyboard()
{
	HRESULT result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			mouse->Acquire();
		else
			return false;
	}

	mouseX += mouseState.lX;
	mouseY += mouseState.lY;

	return true;
}
