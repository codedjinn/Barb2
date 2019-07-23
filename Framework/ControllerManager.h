#pragma once

namespace Framework
{

    class ControllerManager
    {

    public:


        static ControllerManager& Instance();

    private:

        static ControllerManager* _instance;

    };

}