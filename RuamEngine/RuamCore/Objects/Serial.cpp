#include "Serial.hpp"
#include <fstream>
#include <filesystem>

void to_json(json& j, Scene* s) {
    std::cout << "Serialising scene " << s->name() << std::endl;
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
    std::cout << "Writing to " << filename << std::endl;
    std::ofstream file(filename);
    file << j.dump(4);
    std::cout << "Wrote to " << filename << std::endl;
    file.close();
}

Scene* Serial::deserialise(const std::string &scene_name) {
    // std::string filename = scene_name + ".json";
    // if (!std::filesystem::exists(filename)) {
    //     std::cout << "File " << filename << " does not exist!" << std::endl;
    //     return nullptr;
    // }
    // std::ifstream file(filename);
    // json j;
    // file >> j;
    // const std::string name = j["name"];
    // Scene s = sm::CreateScene(name);
    // sm::SetActiveScene(s->id());
    //
    // for (const auto& obj : j["objects"]) {
    //     Object *o = s->newObject(obj["idx"]);
    //     std::string objName = obj["name"];
    //     o->setName(objName);
    //     if (!obj["components"].is_null()) {
    //         for (const auto& comp : obj["components"]) {
    //             std::string type = comp["type"];
    //             if (!Component::componentRegistry.contains(type)) {
    //                 std::cerr << "Component type " << type << " not registered!" << std::endl;
    //                 return nullptr;
    //             }
    //             auto constructor = Component::componentRegistry[type];
    //             constructor(comp, o->id());
    //         }
    //     }
    // }
    // std::cout << "Deserialised scene " << s->name() << std::endl;
    // return sm::ActiveScene();
}