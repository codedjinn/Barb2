#pragma once

#include <algorithm>
#include <vector>

#include <Observable.h>
#include <SDL.h>

#include "FrameworkServices.h"

namespace Framework
{
    using namespace std;

    class Keys
    {
        friend class InputManager;

    public:

        SDL_Keymod Modifier;
        //SDL Modifier;

        Keys()
        {
        }

        bool IsKeyDown(SDL_Keycode key)
        {
            auto item = find(_keysDown.begin(), _keysDown.end(), key);
            return item != _keysDown.end();
        }

        bool IsKeyUp(SDL_Keycode key)
        {
            auto item = find(_keysUp.begin(), _keysUp.end(), key);
            return item != _keysUp.end();
        }

        vector<int> GetPressedKeys()
        {
            return _keysDown;
        }

        vector<int> GetReleasedKeys()
        {
            return _keysUp;
        }

        SDL_Event GetEvent()
        {
            return _event;
        }


    private:

        void Erase(vector<int> list, SDL_Keycode key)
        {
            for (int i = 0; i < list.size(); i++)
            {
                auto code = list.at(i);
                if (code == key)
                {
                    list.erase(list.begin() + i);
                    break;
                }
            }
        }

        vector<int> _keysDown;
        vector<int> _keysUp;
        SDL_Event _event;

    };

    enum class MouseButtonState
    {
        None = -1,
        Pressed = 0,
        Released = 1
    };

    enum class MouseWheelState
    {
        WheelNone,
        WheelUp,
        WheelDown
    };

    class MouseArgs
    {
        friend class InputManager;

    public:

        int x;
        int y;

        int relativeX;
        int relativeY;

        MouseWheelState MouseWheel;

        MouseButtonState LeftButton;
        MouseButtonState MiddleButton;
        MouseButtonState RightButton;

        MouseArgs()
        {
            x = 0;
            y = 0;
            relativeX = 0;
            relativeY = 0;

            MouseWheel = MouseWheelState::WheelNone;

            LeftButton = MouseButtonState::None;
            MiddleButton = MouseButtonState::None;
            RightButton = MouseButtonState::None;
        }
    };

    class InputObserver
    {

    public:

        bool operator==(InputObserver& rhs)
        {
            return *this == rhs;
        }

        virtual void OnEvent(SDL_Event evt, float time) {}
        virtual void OnKeyPressed(Keys keys, float time) {}
        virtual void OnMouseMove(MouseArgs args, float time) {}
        virtual void OnMouseButton(MouseArgs args, float time) {}
        virtual void OnMouseWheel(MouseArgs args, float time) {}

    };


    class InputManager : public Common::Observable<InputObserver>
    {
    public:

        InputManager() {}

        void SetCursorPosition(int x, int y);

        void HandleEvent(SDL_Event evt, float time);

        bool IsKeyDown(SDL_Keycode key);

        static InputManager& Instance();

    private:

        Keys _keys;
        MouseArgs _args;

        static InputManager* _instance;
    };
}