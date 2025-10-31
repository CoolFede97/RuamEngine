#pragma once

#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "imgui.h"
#include "Manager.h"
#include "DebugUtils.h"
#include "AudioSource.h"

#include <iostream>

class Counter : public Component {
public:
	using Component::Component;
	void start() {
		std::cout << "HOADA\n";
	}

	void update() {
	}
};
