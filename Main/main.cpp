
#include <iostream>

#include <FrameworkServices.h>
#include <InputManager.h>

#include "SDL_main.h"

#include "Engine.h"


int main(int, char**) 
{
    Main::Engine* engine = new Main::Engine();
    
    if (!engine->Initialize())
    {
        std::cout << "There was an error initializing SDL2" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        delete engine;
        return 1;
    }

    engine->Run();

    delete engine;
    return 0;
}