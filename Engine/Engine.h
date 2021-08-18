#pragma once
#include <string>
#include <windows.h>

#include "graphicsclass.h"
#include "Timer.h"
#include "Window.h"

/**
 * \brief Responsible for generic engine-level logic and maintaining an update loop.
 */
class Engine : Window
{
public:
    static void Exit();

public:
    bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, bool fullscreen);
    
    bool ProcessMessages();
    
    // Called once per frame.
    void Update();

    // Called once per frame; after Update.
    void RenderFrame();
    
    void Shutdown() override;

private:
    Timer timer;
    GraphicsClass* gfx;

    Scene* sceneNode;
    Camera* camera;
    Actor* sun;
    Actor* earth;

    // Creates the Scene and all the starting Actors to be included in the Scene.
    bool InitialiseScene();

    // Decides which assets to load on engine initialisation.
    void PreloadAssets();

    // Updates all the enabled Nodes in the Scene.
    void UpdateScene(const float deltaTime);
};

// Singleton.
static Engine* instance = nullptr;
static Engine* GetInstance() { return instance; }
