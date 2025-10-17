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
		m_audio = object()->getComponent<AudioSource>();
		if (m_audio != nullptr) {
			std::cout << "Inicio: volumen=" << m_audio->volume() << "\n";
			m_audio->setVolume(2);
			std::cout << "Inicio: volumen=" << m_audio->volume() << "\n";
		}
	}

	void update() {
	}

private:
	AudioSource* m_audio;
};
