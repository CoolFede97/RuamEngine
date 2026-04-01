#pragma once

#include "Wave.h"
#include "al-wrapper.h"
#include "alc-wrapper.h"
#include "ThreadPool.h"

namespace AudioSystem {

static thread_pool pool(2);

void init();
void init(const char* device);

void shutdown();

ALC::Device device();
ALC::Context context();
}
