    
#include "Engine.h"
#include <DirectionalLight.h>
#include <LightManager.h>
#include <iostream>
#include <Texture.h>
#include <StaticMesh.h>
#include <Map.h>
#include <RenderTokenSky.h>

using namespace Main;
using namespace Framework;
using namespace CameraLib;

Engine::Engine()
{
    _width = 800;
    _height = 600;
    _quit = false;
    _hasShutdown = false;
    _frameTimeRender = 0;
    _frameTimeUpdate = 0;
    _window = nullptr;
    _renderer = nullptr;

    
}

Engine::~Engine()
{
    if (!_hasShutdown)
    {
        this->Shutdown();
    }
}

bool Engine::Initialize()
{
    if (!this->InitSDL())
    {
        SDL_Quit();
        return false;
    }

    GLenum error = glewInit();
    if (GLEW_OK != error)
    {
        std::cout << "glew!!" << glewGetErrorString(error) << endl;
        //Log::Error("Fatal error glew failed to initialize properly!");
        this->Shutdown();
        exit(-1);
    }

    this->InitRenderer();    

    this->InitSandbox();

    return true;
}

void Engine::InitRenderer()
{
    Renderer& renderer = Renderer::GetInstance();
    /*renderer.GetMainCamera().ResetCameraOrientation();
    renderer.GetMainCamera().SetPosition(0.0f, 100.0f, 105.0f);
    renderer.GetMainCamera().Update();
    renderer.GetMainCamera().SetFarClipPlaneDistance(16000.0f);
    renderer.Resize(_width, _height);*/

    renderer.SetRendererOutputMode(RENDERER_OUTPUT_STANDARD);
    renderer.SetRenderWireframe(false);
    renderer.SetBoundingVolumesVisible(false);
    renderer.SetFXAAEnabled(false);

    InputManager::Instance().AddObserver(this);

    DirectionalLight* directionalLight = new DirectionalLight(MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f), MathLib::vector4(0.0f, -1.0f, 0.8f, 0.0f));
    LightManager::GetInstance().GetDirectionalLightList().push_back(directionalLight);

    Camera camera;
    camera.SetNearClipPlaneDistance(1.0f);
    camera.SetFarClipPlaneDistance(16000.0f);
    camera.SetCameraYFov(60.0f);
    camera.GetPerspectiveProjectionMatrix(renderer.GetViewportWidth(), renderer.GetViewportHeight());
    camera.SetPosition(50, 300, 50);
    camera.ResetCameraOrientation();
    camera.RotateXAxis(-75);
    camera.RotateYAxis(180);
    camera.Update();
    _mainCamera = camera;
    renderer.SetUpCamera(camera);

    renderer.SetUpCamera(camera);
}

bool Engine::InitSDL()
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        _errors.push_back(SDL_GetError());
        return false;
    }

    _window = SDL_CreateWindow("GoBarb", 200, 200, _width, _height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_TEXTUREACCESS_STATIC);
    if (_window == nullptr) 
    {
        _errors.push_back(SDL_GetError());
        return false;
    }

    _context = SDL_GL_CreateContext(_window);
    if (nullptr == _context)
    {
        SDL_GL_DeleteContext(_context);
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (_renderer == nullptr) {
        SDL_DestroyWindow(_window);
        _errors.push_back(SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);

    return true;
}

void Engine::InitSandbox()
{
    // Load texture database for map.
    const char* texturesFileName = "Textures\\textures.tdb";
    FILE* texturesFileHandle;
    fopen_s(&texturesFileHandle, texturesFileName, "rb");
    TextureManager& textureManagerHandle = TextureManager::GetInstance();
    textureManagerHandle.SetTextureDatabase(texturesFileHandle);

    // Load static meshes database for map.
    const char* meshesFileName = "Models\\tank_meshes.mdb";
    FILE* meshesFileHandle;
    fopen_s(&meshesFileHandle, meshesFileName, "rb");
    StaticMeshManager& staticMeshManagerHandle = StaticMeshManager::GetInstance();
    staticMeshManagerHandle.SetMeshDatabase(meshesFileHandle);

    _map = new Framework::Map(4, 1024, vector4(0, 0, 0, 0));
    //map->Initialize();
    _map->Load("new_map\\test_map.map");
}

void Engine::AddRenderingTokens()
{
    // Add render tokens
    {
        Renderer& renderer = Renderer::GetInstance();
        RendererLib::RenderableSet& renderableSet = logicBufferedData.renderableSet;
        renderableSet.Reset();

        /*RenderList& renderList = RenderList::Instance();
        List<Component*> renderables = renderList.GetRenderables();
        for (int i = 0; i < renderables.Count(); i++)
        {
            StaticMeshComponent* staticMesh = dynamic_cast<StaticMeshComponent*>(renderables[i]);
            if (staticMesh != NULL)
            {
                renderableSet.AddStaticMeshToken(new RenderTokenStaticMesh(staticMesh->GetMeshInstance()));
            }
        }*/

        if (_map != nullptr)
        {
            std::vector<RenderToken*> terrainTokens = _map->GetRendererTerrain()->GetRenderTokens(_mainCamera);
            for (unsigned int i = 0; i < terrainTokens.size(); i++)
                renderableSet.AddTerrainToken(terrainTokens[i]);
        }

        //// Test add a basic shape token.
        //MathLib::quaternion identityQuaternion;
        //MathLib::quaternion_setToIdentity(identityQuaternion);

        std::vector<std::string> faceTextures;
        faceTextures.push_back("skybox_sunny_0_front");
        faceTextures.push_back("skybox_sunny_0_right");
        faceTextures.push_back("skybox_sunny_0_back");
        faceTextures.push_back("skybox_sunny_0_left");
        faceTextures.push_back("skybox_sunny_0_top");
        faceTextures.push_back("skybox_sunny_0_bottom");
        renderableSet.AddSkyToken(new RendererLib::RenderTokenSky(faceTextures));

        /*renderableSet.AddBasicShapeToken(new RendererLib::RenderTokenBasicShape(BasicShapes::BASIC_SHAPE_CAPSULE,
            MathLib::vector4(1.0f, 1.0f, 1.0f, 1.0f),
            MathLib::vector4(250.0f, 150.0f, 250.0f, 1.0f),
            identityQuaternion,
            MathLib::vector4(100.0f, 20.0f, 0.0f, 0.0f),
            faceTextures));

        if (_playerController != nullptr)*/
        logicBufferedData.mainCamera = _mainCamera;

        //rendererMutex.lock();
        commonBufferedData.CopyFrom(logicBufferedData);
        //rendererMutex.unlock();
    }
}

void Engine::Run()
{
    unsigned int timeSinceLastUpdate = 0;
    unsigned int frameTime = 0;

    while (!_quit)
    {
        unsigned int startFrameTime = SDL_GetTicks();

        timeSinceLastUpdate += frameTime;

        float deltaTime = (float)frameTime / 1000.0f;

        this->Render(deltaTime);
        this->Update(deltaTime);        

        frameTime = SDL_GetTicks() - startFrameTime;
    }
}

void Engine::Shutdown()
{
    _quit = true;
    if (_renderer != nullptr)
    {
        SDL_DestroyRenderer(_renderer);
    }
    if (_window != nullptr)
    {
        SDL_DestroyWindow(_window);
    }
    if (_context != nullptr)
    {
        SDL_GL_DeleteContext(_context);
    }
    if (_map != nullptr)
    {
        delete _map;
    }
    _hasShutdown = true;
}

void Engine::Update(float time)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        InputManager::Instance().HandleEvent(event, time);
    }

    this->AddRenderingTokens();

    //do
    //{
    //    if (!_events.empty())
    //    {
    //        event = _events.front();
    //    }

    //    if (event != nullptr)
    //    {
    //        // do something
    //    }

    //} while (event != nullptr);
}

void Engine::Render(float time)
{
    RendererLib::Renderer& renderer = RendererLib::Renderer::GetInstance();

    // Copy buffered data.
    {
        //rendererMutex.lock();
        rendererBufferedData.CopyFrom(commonBufferedData);
        //rendererMutex.unlock();

        Camera& rendererCamera = renderer.GetMainCamera();
        rendererCamera = rendererBufferedData.mainCamera;

        renderer.CopyRenderableSet(rendererBufferedData.renderableSet);
    }

    renderer.Draw();

    glClear(GL_DEPTH_BUFFER_BIT);
    renderer.TransferGBufferDepth();

    SDL_GL_SwapWindow(_window);
    
}

void Engine::OnEvent(SDL_Event evt, float time)
{
    if (evt.type == SDL_QUIT)
    {
        if (!_hasShutdown)
        {
            this->Shutdown();
        }
        exit(0);
    }
}

void Engine::OnKeyPressed(Keys keys, float time)
{
    if (keys.IsKeyUp(SDLK_ESCAPE))
    {
        _quit = true;
    }
}

void Engine::OnMouseMove(MouseArgs args, float time)
{
}

void Engine::OnMouseButton(MouseArgs args, float time)
{
}

void Engine::OnMouseWheel(MouseArgs args, float time)
{
}






