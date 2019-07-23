
#include "InputManager.h"
#include <algorithm>

namespace Framework
{

void InputManager::HandleEvent(SDL_Event event, float time)
{
    _keys.Modifier = SDL_GetModState();

    for (int i = 0; i < _observers.size(); i++)
    {
        auto observer = _observers[i];
        observer->OnEvent(event, time);
    }

    switch (event.type)
    {
    case SDL_MOUSEMOTION:
        _args.x = event.motion.x;
        _args.y = event.motion.y;
        _args.relativeX = event.motion.xrel;
        _args.relativeY = event.motion.yrel;
        for (int i = 0; i < _observers.size(); i++)
        {
            _observers[i]->OnMouseMove(_args, time);
        }
        break;

    case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            _args.LeftButton = MouseButtonState::Pressed;
            for (int i = 0; i < _observers.size(); i++)
            {
                _observers[i]->OnMouseButton(_args, time);
            }
            _args.LeftButton = MouseButtonState::None;
            break;
        case SDL_BUTTON_MIDDLE:
            _args.MiddleButton = MouseButtonState::Pressed;
            for (int i = 0; i < _observers.size(); i++)
            {
                _observers[i]->OnMouseButton(_args, time);
            }
            _args.MiddleButton = MouseButtonState::None;
            break;
        case SDL_BUTTON_RIGHT:
            _args.RightButton = MouseButtonState::Pressed;
            for (int i = 0; i < _observers.size(); i++)
            {
                _observers[i]->OnMouseButton(_args, time);
            }
            _args.RightButton = MouseButtonState::None;
            break;
        }
        break;

    case SDL_MOUSEBUTTONUP:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            _args.LeftButton = MouseButtonState::Released;
            for (int i = 0; i < _observers.size(); i++)
            {
                _observers[i]->OnMouseButton(_args, time);
            }
            _args.LeftButton = MouseButtonState::None;
            break;
        case SDL_BUTTON_MIDDLE:
            _args.MiddleButton = MouseButtonState::Released;
            for (int i = 0; i < _observers.size(); i++)
            {
                _observers[i]->OnMouseButton(_args, time);
            }
            _args.MiddleButton = MouseButtonState::None;
            break;
        case SDL_BUTTON_RIGHT:
            _args.RightButton = MouseButtonState::Released;
            for (int i = 0; i < _observers.size(); i++)
            {
                _observers[i]->OnMouseButton(_args, time);
            }
            _args.RightButton = MouseButtonState::None;
            break;
        }
        break;


    case SDL_KEYDOWN:
        _keys._keysDown.push_back(event.key.keysym.sym);
        break;

    case SDL_KEYUP:
        int keyCode = event.key.keysym.sym;
        _keys.Erase(_keys._keysDown, keyCode);
        _keys._keysUp.push_back(keyCode);
        for (int i = 0; i < _observers.size(); i++)
        {
            _observers[i]->OnKeyPressed(_keys, time);
        }
        _keys.Erase(_keys._keysUp, keyCode);
        break;
    }

    if (_args.MouseWheel == MouseWheelState::WheelUp ||
        _args.MouseWheel == MouseWheelState::WheelDown)
    {
        for (int i = 0; i < _observers.size(); i++)
        {
            _observers[i]->OnMouseWheel(_args, time);
        }
        _args.MouseWheel = MouseWheelState::WheelNone;
    }

    if (_keys._keysDown.size() > 0)
    {
        for (int i = 0; i < _observers.size(); i++)
        {
            _observers[i]->OnKeyPressed(_keys, time);
        }
    }
}

bool InputManager::IsKeyDown(SDL_Keycode key)
{
    auto it = std::find(_keys._keysDown.begin(), _keys._keysDown.end(), key);
    return it != _keys._keysDown.end();
}

void InputManager::SetCursorPosition(int x, int y)
{
    SDL_WarpMouseGlobal(x, y);
}

InputManager* InputManager::_instance = NULL;
InputManager& InputManager::Instance()
{
    if (InputManager::_instance == NULL)
    {
        InputManager::_instance = new InputManager();
    }
    return *InputManager::_instance;
}
}