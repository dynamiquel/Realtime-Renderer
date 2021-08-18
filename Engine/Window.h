#pragma once

#include <string>
#include <windows.h>
#include "Input.h"

/**
 * \brief Responsible for most of the Win32 window functionality.
 */
class Window
{
public:
    bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int& width, int& height, bool fullscreen);
    bool ProcessMessages();
    HWND GetHWND() const { return handle; }
    LRESULT MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    Input* GetInput() { return input; }
    virtual void Shutdown();
    ~Window();

private:
    void SetupWindowClass();
    HWND handle = nullptr; //Handle to this window
    HINSTANCE hInstance = nullptr; //Handle to application instance
    std::string window_title = "";
    std::wstring window_title_wide = L""; //Wide string representation of window title
    std::string window_class = "";
    std::wstring window_class_wide = L""; //Wide string representation of window class name
    int width = 0;
    int height = 0;
    bool fullscreen = false;

    Input* input;
};

/////////////
// GLOBALS //
/////////////
static Window* WindowHandle = 0;
