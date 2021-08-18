#include "Window.h"
#include "Logger.h"
#include "StringHelper.h"

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// All other messages
		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			DestroyWindow(hwnd);
			return 0;
		}

	default:
		{
			// forward message to window class handler
			return WindowHandle->MessageHandler(hwnd, uMsg, wParam, lParam);
		}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    	case WM_NCCREATE:
    	{
    		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
    		return WindowHandle->MessageHandler(hwnd, uMsg, wParam, lParam);
    	}

        // All other messages pass to the message handler in the system class.
        default:
        {
    		return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}

bool Window::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int& width, int& height, bool fullscreen)
{	
    this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringHelper::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringHelper::StringToWide(this->window_class); //wide string representation of class string (used for registering class and creating window)
	this->fullscreen = fullscreen;
	
	this->SetupWindowClass();

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->height / 2;

	RECT wr; //Widow Rectangle

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(fullscreen)
	{
		// Determine the resolution of the clients desktop screen.
		width  = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
		
		DEVMODE dmScreenSettings;
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		wr.left = 0;
		wr.top = 0;
		wr.right = 0;
		wr.bottom = 0;
	}
	else
	{
		wr.left = centerScreenX;
		wr.top = centerScreenY;
		wr.right = wr.left + this->width;
		wr.bottom = wr.top + this->height;
		AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	}

	this->handle = CreateWindowEx(0, //Extended Windows style - we are using the default. For other options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx
		this->window_class_wide.c_str(), //Window class name
		this->window_title_wide.c_str(), //Window Title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //Windows style - See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
		wr.left, //Window X Position
		wr.top, //Window Y Position
		wr.right - wr.left, //Window Width
		wr.bottom - wr.top, //Window Height
		NULL, //Handle to parent of this window. Since this is the first window, it has no parent window.
		NULL, //Handle to menu or child window identifier. Can be set to NULL and use menu in WindowClassEx if a menu is desired to be used.
		this->hInstance, //Handle to the instance of module to be used with this window
		NULL); //Param to create window

	if (this->handle == nullptr)
	{
		Logger::Log(LogSource::Application, LogType::Error, "HWMD Handle is null.");
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);
	
	input = new Input();
	if (!input->Initialise(hInstance, GetHWND()))
	{
		Logger::Log(LogSource::Input, LogType::Error, "Input couldn't be initialised.");
		return false;
	}

	Logger::Log(LogSource::Input, LogType::Log, "Input initialised.");

	return true;
}

bool Window::ProcessMessages()
{
	// Handle the windows messages.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

	while (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
        this->handle, //Handle to window we are checking messages for
        0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
        0,    //Maximum Filter Msg Value
        PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
    {
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
    }

	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = nullptr; //Message processing loop takes care of destroying this window
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Window::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	//	return true;

	switch (uMsg)
	{
		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

void Window::Shutdown()
{
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	
	if (handle != nullptr)
	{
		UnregisterClass(window_class_wide.c_str(), hInstance);
		DestroyWindow(handle);
	}

	if (input)
	{
		delete input;
		input = nullptr;
	}

	handle = nullptr;
	hInstance = nullptr;
	WindowHandle = nullptr;
}

Window::~Window()
{
	Shutdown();
}

void Window::SetupWindowClass()
{
    WNDCLASSEX wc;
    
    // Setup the windows class with default settings.
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = HandleMessageSetup;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm       = wc.hIcon;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = window_class_wide.c_str();
    wc.cbSize        = sizeof(WNDCLASSEX);

    // Register the class so that it is usable.
    RegisterClassEx(&wc);
}
