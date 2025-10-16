#include "TestComponent.hpp"

using sm = SceneManager;

sm::ScenePtr SceneA() {
	Scene s("SceneA");
	auto o = s.newObject();
	std::string name = "ObjectA";
	o->setName(name);
	o->transform().setPosition({1.0f, 2.0f, 3.0f});
	o->addComponent<TestComponent>()->setSecret(7);
	return std::make_unique<Scene>(s);
}

int main_deserial() {
	auto s = Serial::deserialise("SceneA");
	if (!s) {
		// main_serial();
		s = Serial::deserialise("SceneA");
	}
	for (auto obj : s->getObjects()) {
		auto t = obj->transform();
		std::cout << "Object " << obj->id() << " named " << obj->name() << std::endl;
		std::cout << " - Position: " << t.position().x << ", " << t.position().y << ", " << t.position().z << std::endl;
		for (auto comp : obj->getComponents()) {
			std::cout << " - Component " << comp->id() << std::endl;
		}
	}
	auto x = s->getObjectByIdx(0)->getComponent<TestComponent>()->getSecret();
	std::cout << "Secret is " << x << std::endl;
	s->start();
	return 0;
}

int main() {
	sm::AddScene(0, SceneA);
	sm::SetActiveScene(0);
	auto s = sm::ActiveScene();
	std::cout << s->getObjectByIdx(0)->getComponent<TestComponent>()->getSecret() << std::endl;
}