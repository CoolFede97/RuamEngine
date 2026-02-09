#include "RuamEditor.h"
#include "Entity.h"
#include "ModelRenderer.h"
#include "SceneManager.h"
#include "Transform.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <typeindex>
#include <typeinfo>

namespace RuamEngine
{
	Entity* RuamEditor::selectedEntity = nullptr;
	void RuamEditor::UpdateHierarchy()
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
				if (ImGui::Button("Delete selected entity"))
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
				ImGui::TextDisabled("Press enter to apply");
			}

		}
		ImGui::End();
	}

	std::unordered_map<std::type_index, std::function<void(const std::string&, void*)>> RuamEditor::s_inspectorDrawers =
	{
		{
			std::type_index(typeid(float)), [](const std::string& name, void* value)
			{
				std::string label = name + ": ";
				std::string labelCopy = "##"+name;
				ImGui::Text("%s",label.c_str());
				ImGui::SameLine();
				ImGui::DragFloat3(labelCopy.c_str(), static_cast<float*>(value), 0.1f);
			}
		},
		{
			std::type_index(typeid(glm::vec3)), [](const std::string& name, void* value)
			{
				std::string label = name + ": ";
				std::string labelCopy = "##"+name;
				ImGui::Text("%s",label.c_str());
				ImGui::SameLine();
				ImGui::DragFloat3(labelCopy.c_str(), &static_cast<glm::vec3*>(value)->x, 0.1f);
			}
		}
	};
	void RuamEditor::UpdateInspector()
	{
		if (selectedEntity == nullptr) return;
		ImGui::Begin("Inspector");
		Scene* scene = SceneManager::ActiveScene();
		if (scene != nullptr)
		{
			ImGui::PushID(selectedEntity->id());
			if (ImGui::CollapsingHeader("Transform"))
			{
				selectedEntity->transform().forEachSerializedField([](const std::string& name, const std::type_index& type, void* value)
				{
					if (s_inspectorDrawers.find(type) != s_inspectorDrawers.end())	s_inspectorDrawers[type](name, value);
					else std::cerr << "Error: Couldn't find matching function in s_inspectorDrawers for a variable of type " << type.name() << " \n";
				});
			}
			if (ImGui::CollapsingHeader("Model Renderer"))
			{
				DrawOnInspector(selectedEntity->getComponent<ModelRenderer>());
			}
			ImGui::PopID();

		}
		ImGui::End();
	}

	// void RuamEditor::DrawOnInspector(const std::string& name, const std::type_info& type, void* value)
	// {
	// }


	void RuamEditor::DrawOnInspector(Transform& transform)
	{
		ImGui::Text("Pos: ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Pos", &transform.position().x, 0.1f);

		ImGui::Text("Rot: ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rot", &transform.rotation().x, 0.1f);

		ImGui::Text("Sca: ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Sca", &transform.scale().x, 0.1f);
	}
	void RuamEditor::DrawOnInspector(ModelRenderer* modelRenderer)
	{
		static char nameBuffer[128];
		std::strncpy(nameBuffer, modelRenderer->m_meshPath.c_str(), sizeof(nameBuffer));
		nameBuffer[sizeof(nameBuffer) - 1] = '\0';

		ImGui::Text("Model path: ");

		ImGui::SameLine();

		if (ImGui::InputText("",nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			modelRenderer->setModel(nameBuffer);
		}
	}
}
