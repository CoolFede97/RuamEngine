#pragma once
#include "Serial.hpp"
#include "Component.hpp"

class TestComponent : public Component {
public:
	using Component::Component;
	TestComponent(const nlohmann ::json &j, unsigned int obj_id) : Component(obj_id) {
		secret = j["secret"];
		name = j["name"];
	}

	void start() {
		std ::cout << "TestComponent with name: " << name << " started on object "
			<< std ::endl;
	}

	void update() {
		std ::cout << "TestComponent updated on object " << std ::endl;
	}
	
	std ::string getName() const { return name; }
	void setName(const std ::string value) { name = value; }
	int getSecret() const { return secret; }
	void setSecret(const int value) { secret = value; }

private:
	std ::string name = "";
	int secret = 0;

	IMPL_SERIALIZE(TestComponent, {"secret", secret}, {"name", name})
};

REGISTER_COMPONENT(TestComponent)
