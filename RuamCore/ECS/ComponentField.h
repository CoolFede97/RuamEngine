#include <typeindex>
#include <functional>
#include "nlohmann/json.hpp"

namespace RuamEngine
{
   	struct FieldInfo
   	{
   	    const char* name;
  		std::type_index type;
        void* value;
        std::function<void()> callbackOnChange;
        std::function<nlohmann::json()> serialize;
        std::function<void(nlohmann::json&)> deserialize;
   	};
    template<typename T>
    FieldInfo makeFieldInfo(const char* name, T& value, std::function<void()> callback = {})
    {
        return {
            name,
            typeid(T),
            &value,
            std::move(callback),
            [&value]()
            {
                return nlohmann::json(value);
            },
            [&value, name](const nlohmann::json& json)
            {
                if (json.contains(name)) value = json[name].get<T>();
            }
        };
    }
}
