#pragma once

#include "DebugUtils.h"
#include "OpenGLUtils.h"
#include "ThreadPool.h"
#include <memory>


template <typename T>
std::shared_ptr<T> GetShared(std::weak_ptr<T> ptr) {
    if (ptr.expired())
    {
        std::cout << "Weak pointer of type " << typeid(T).name() << " is null\n";
        return nullptr;
    }
    return ptr.lock();
}
