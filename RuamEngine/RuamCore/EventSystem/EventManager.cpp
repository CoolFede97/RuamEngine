#include "EventManager.h"

namespace RuamEngine
{
    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> EventManager::instantSubscribers = {};
    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> EventManager::subscribers = {};
    std::queue<std::unique_ptr<Event>> EventManager::eventQueue = {};
}
