#include "TestComponent.hpp"

using sm = SceneManager;

int main() {
	bool sceneIsNew = sm::StartScene(0, "SceneA");
	auto s = sm::ActiveScene();
	if (sceneIsNew) {
		auto o = s->newObject();
		o->setName("ObjectA");
		auto c = o->addComponent<TestComponent>();
		c->setName("hola");
		c->setSecret(7);
	}
	std::cout << s->getObjectByIdx(0)->getComponent<TestComponent>()->getSecret() << std::endl;
	std::cout << s->getObjectByIdx(0)->getComponent<TestComponent>()->getName() << std::endl;
	Serial::serialise(s);
}