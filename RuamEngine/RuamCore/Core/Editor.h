#pragma once

#include "Entity.h"
#include <typeinfo>
#include <unordered_map>
namespace RuamEngine
{
	class Entity;
	class Editor
	{
	public:
		static void UpdateHierarchy();
		static void UpdateInspector();

		// static void DrawOnInspector(const std::string& name, const std::type_info& type, void* value);
		static void DrawOnInspector(glm::vec3& vec);
		// static void DrawOnInspector(ModelRenderer* modelRenderer);

		static Entity* selectedEntity;

		static std::unordered_map<std::type_index, std::function<void(const std::string&, void*)>> s_inspectorDrawers;
	};
}
