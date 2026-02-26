#pragma once

#include "Component.h"
#include "Scene.h"
#include "SceneManager.h"
#include "imgui.h"
#include "Manager.h"
#include "DebugUtils.h"
#include "AudioSource.h"

#include <iostream>

namespace RuamEngine
{
	class Counter : public Component
	{
	public:
		using Component::Component;

		IMPL_SIMPLE_SERIALIZE(Counter)
		std::string name() override { return "Counter"; }
		IMPL_DRAW_SERIALIZED_MEMBERS(;)

		void start()
		{
			std::cout << "HOADA\n";
		}

		void update()
		{
		}
	};
}
