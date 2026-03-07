#include "Editor.h"
#include "Entity.h"
#include "Component.h"
#include "SceneManager.h"
#include "Input.h"

#include "imgui.h"
#include <typeindex>
#include <utility>

namespace RuamEngine
{
	Entity* Editor::selectedEntity = nullptr;
	void Editor::UpdateHierarchy()
	{
		ImGui::Begin("Hierarchy");
		Scene* scene = SceneManager::ActiveScene();
		if (scene != nullptr)
		{
			if (ImGui::Button("Create Entity"))
			{
				selectedEntity = scene->createEntity();
			}

			ImGui::SameLine();

			if (selectedEntity != nullptr)
			{
				if (Input::GetKeyDown(KeyCode::Delete_Key))
				{
					selectedEntity->destroy();
					selectedEntity = nullptr;
				}
			}

			ImGui::Separator();

			std::list<Entity*> entities = scene->getEntities();
			for (Entity* entity : entities)
			{
				bool selected = (selectedEntity == entity);
				if (ImGui::Selectable(entity->name().c_str(), selected))
				{
					selectedEntity = entity;
				}
			}

			ImGui::Separator();

			if (selectedEntity != nullptr)
			{
				ImGui::Text("Selected: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "%s", selectedEntity->name().c_str());

				static char nameBuffer[128];

				static Entity* lastSelected = nullptr;

				if (selectedEntity != lastSelected)
				{
					strcpy(nameBuffer, selectedEntity->name().c_str());
					lastSelected = selectedEntity;
				}

				if (ImGui::InputText("##entity_name", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					selectedEntity->setName(nameBuffer);
				}
			}

		}
		ImGui::End();
	}

	std::unordered_map<std::type_index, SerializedMemberDrawer> Editor::s_inspectorDrawers =
	{
		{
			std::type_index(typeid(float)), [](const std::string& name, void* value, std::function<void()>* callbackOnChange)
			{
			    DRAW_MEMBER_NAME(name);
				if (ImGui::DragFloat(labelCopy.c_str(), static_cast<float*>(value), 0.1f))
				{
				    if (callbackOnChange != nullptr) (*callbackOnChange)();
				};
			}
		},
		{
			std::type_index(typeid(glm::vec3)), [](const std::string& name, void* value, std::function<void()>* callbackOnChange)
			{
				DRAW_MEMBER_NAME(name);
				if (ImGui::DragFloat3(labelCopy.c_str(), &static_cast<glm::vec3*>(value)->x, 0.1f))
				{
                    if (callbackOnChange != nullptr) (*callbackOnChange)();
				}
			}
		},
		{
			std::type_index(typeid(std::string)), [](const std::string& name, void* value, std::function<void()>* callbackOnChange)
			{
				DRAW_MEMBER_NAME(name);
				std::string* str = static_cast<std::string*>(value);

				static std::unordered_map<void*, std::array<char, 256>> buffers;

				auto& buffer = buffers[value];

				if (std::strcmp(buffer.data(), str->c_str()) != 0)
				{
    				std::strncpy(buffer.data(), str->c_str(), buffer.size());
    				buffer[sizeof(buffer)-1] = '\0';
				}
				if (ImGui::InputText(labelCopy.c_str(), buffer.data(), buffer.size(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
				    *str = buffer.data();
					if (callbackOnChange != nullptr) (*callbackOnChange)();
				}
			}
		}
	};

	void Editor::UpdateInspector()
	{
		if (selectedEntity == nullptr) return;
		ImGui::Begin("Inspector");

		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text(selectedEntity->name().c_str());
		ImGui::SetWindowFontScale(1.0f);
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			ImGui::OpenPopup("addComponent");
		}

		if (ImGui::BeginPopup("addComponent"))
		{
			ImGui::Text("Add a component:");
			ImGui::Separator();

			for (auto& [cmpName, factory] : Component::componentRegistry)
			{
				if (cmpName == "Transform" || cmpName == "Component") continue;
				if (ImGui::Selectable(cmpName.c_str()))
				{
					factory.addComponent(selectedEntity);
				}
			}
			ImGui::EndPopup();
		}

		Scene* scene = SceneManager::ActiveScene();
		if (scene != nullptr)
		{
			ImGui::PushID(selectedEntity->id());
			for (Component* com : selectedEntity->getComponents())
			{
				if (ImGui::CollapsingHeader(com->name().c_str()))
				{
					com->drawSerializedMembers();
				}
			}
			ImGui::PopID();
		}
		ImGui::End();
	}

	void Editor::DrawMemberInInspector(const std::string& name, const std::type_index& type, void* value, std::function<void()>* callbackOnChange)
	{
	    if (s_inspectorDrawers.find(type) != s_inspectorDrawers.end())	s_inspectorDrawers[type](name, value, callbackOnChange);
		else std::cerr << "Error: Couldn't find matching function in s_inspectorDrawers for a variable of type " << type.name() << " \n";
	}

}
