#pragma once

#include <vector>
#include <queue>
#include <string>
#include <SDL.h>
#include <GL/glew.h>
#include <InputManager.h>
#include <Renderer.h>
#include <Camera.h>
#include <BufferedRendererData.h>
#include <Map.h>

namespace Main
{
    using namespace std;
    using namespace Framework;
    using namespace RendererLib;

    class Engine : public InputObserver
    {
    public:

        Engine();
        ~Engine();

        bool Initialize();
        void Run();
        void Shutdown();

        void Update(float time);
        void Render(float time);

        void OnEvent(SDL_Event evt, float time) override;
        void OnKeyPressed(Keys keys, float time) override;
        void OnMouseMove(MouseArgs args, float time) override;
        void OnMouseButton(MouseArgs args, float time) override;
        void OnMouseWheel(MouseArgs args, float time) override;

    private:

        int _width;
        int _height;

        bool InitSDL();
        void InitRenderer();
        void InitSandbox();

        void AddRenderingTokens();

        bool _quit;
        bool _hasShutdown;

        unsigned int _frameTimeRender;
        unsigned int _frameTimeUpdate;

        BufferedRendererData logicBufferedData;
        BufferedRendererData rendererBufferedData;
        BufferedRendererData commonBufferedData;

        Map* _map;

        CameraLib::Camera _mainCamera;

        vector<string> _errors;
        queue<SDL_Event*> _events;
        
        SDL_Window* _window;
        SDL_Renderer* _renderer;
        SDL_GLContext _context;
    };

}

