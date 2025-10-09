#include "TestComponent.hpp"

using sm = SceneManager;

void main_serial() {
	Scene s("SceneA");
	int id = SceneManager::AddScene(&s);
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
	Serial::serialise(scene);
}

int main() {
	auto s = Serial::deserialise("SceneA");
	if (!s) {
		main_serial();
		s = Serial::deserialise("SceneA");
	}
	auto c = s->getObjectByIdx(1)->getComponent<TestComponent>()->getSecret();
	std::cout << c << std::endl;
	return 0;
}