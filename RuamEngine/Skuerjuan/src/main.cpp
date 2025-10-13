#include "TestComponent.hpp"

using sm = SceneManager;

void main_serial() {
	Scene s("SceneA");
	const unsigned int id = SceneManager::AddScene(&s);
	SceneManager::SetActiveScene(id);

	Scene* scene = sm::ActiveScene();
	Object* o = scene->newObject();
	std::string name = "My Object";
	o->setName(name);
	Object* o2 = scene->newObject();
	name = "Hola";
	o2->setName(name);
	auto c = o->addComponent<TestComponent>();
	c->setSecret(42);
	c->setName("The Answer");
	Serial::serialise(scene);
}

int main() {
	auto s = Serial::deserialise("SceneA");
	if (!s) {
		main_serial();
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