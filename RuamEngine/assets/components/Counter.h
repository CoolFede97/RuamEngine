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
	Counter(const nlohmann::json& j, int obj_id) : Component(obj_id) {};
	void start() {
		std::cout << "HOADA\n";
	}

	void update() {
	}

	IMPL_SIMPLE_SERIALIZE(Counter)
};

REGISTER_COMPONENT(Counter)
