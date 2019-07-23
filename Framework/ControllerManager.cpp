
#include "ControllerManager.h"


using namespace Framework;


ControllerManager* ControllerManager::_instance = nullptr;
ControllerManager& ControllerManager::Instance()
{
    if (ControllerManager::_instance == nullptr)
    {
        ControllerManager::_instance = new ControllerManager();
    }
    return *ControllerManager::_instance;
}

