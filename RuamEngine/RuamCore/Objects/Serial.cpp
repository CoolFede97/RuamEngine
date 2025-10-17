#include "Serial.hpp"
#include <fstream>
#include <filesystem>

void to_json(json& j, Scene* s) {
    json serialisedObjects = json::array();
    json o;
    int i = 0;
    for (Object* obj : s->getObjects()) {
        o = obj;
        o.push_back({"idx", i});
		serialisedObjects.push_back(o);
        i++;
    }

    j = json{
        { "id", s->id() },
        {"name", s->name()},
        {"objects", serialisedObjects}
    };
}

void to_json(json& j, Object* o) {

	json serialisedComponents = json::array();

    for(auto c : o->getComponents()) {
        nlohmann::json comp = *c;
        serialisedComponents.push_back(comp);
    }

    j = json{
        {"name", o->name()},
		{"components", serialisedComponents}
	};
}

void Serial::serialise(Scene* s) {
    json j = s;
    std::string filename = s->name() + ".json";
    std::ofstream file(filename);
    file << j.dump(1, '\t');
    file.close();
}

Scene* Serial::deserialise(const std::string &scene_name) {
    std::string filename = scene_name + ".json";
    if (!std::filesystem::exists(filename)) {
        std::cout << "File " << filename << " does not exist!" << std::endl;
        return nullptr;
    }
    std::ifstream file(filename);
    json j;
    file >> j;
    const std::string name = j["name"];
    unsigned int id = j["id"];
    Scene *s = new Scene(id, name);
    if (!j["objects"].is_null()) {
        for (const auto& obj : j["objects"]) {
            Object *o = s->newObject(obj["idx"]);
            std::string objName = obj["name"];
            o->setName(objName);
            if (!obj["components"].is_null()) {
                for (const auto& comp : obj["components"]) {
                    std::string type = comp["type"];
                    if (!Component::componentRegistry.contains(type)) {
                        std::cerr << "Component type " << type << " not registered!" << std::endl;
                        return nullptr;
                    }
                    auto constructor = Component::componentRegistry[type];
                    constructor(comp, o);
                }
            }
        }
    }
    return s;
}