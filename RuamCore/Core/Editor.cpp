#include "Editor.h"
#include "Entity.h"
#include "Component.h"
#include "EventManager.h"
#include "KeyCode.h"
#include "SaveSystem.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Transform.h"

#include "imgui.h"
#include <typeindex>
#include <utility>
#include <cstring>

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
				if (entity->transform().m_parent) continue;
				DrawEntityFamily(entity);
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

	void Editor::DrawEntityFamily(Entity *entity)
	{
		std::list<Transform*> childrenTransform = entity->transform().children();

		bool selected = (selectedEntity == entity);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                        	ImGuiTreeNodeFlags_OpenOnDoubleClick  |
                            ImGuiTreeNodeFlags_SpanAvailWidth;

		if (selected) flags |= ImGuiTreeNodeFlags_Selected;
		if (childrenTransform.empty()) flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		bool opened = ImGui::TreeNodeEx(entity->name().c_str(), flags);

		if (ImGui::IsItemClicked()) selectedEntity = entity;

		if (opened && !childrenTransform.empty())
		{
			for (Transform* transform : childrenTransform)
			{
				DrawEntityFamily(transform->entity());
			}
			ImGui::TreePop();
		}
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

		DrawAddComponentButton();

		Scene* scene = SceneManager::ActiveScene();
		if (scene != nullptr)
		{
			for (Component* com : selectedEntity->getComponents())
			{
				ImGui::PushID(com->id());
				bool opened = ImGui::CollapsingHeader(com->name().c_str(), ImGuiTreeNodeFlags_AllowItemOverlap);
				ImGui::SameLine();
				if (ImGui::Button("-"))	com->destroy();
				if (opened)
				{
					com->drawSerializedMembers();
				}
				ImGui::PopID();
			}
		}
		ImGui::End();
	}

	void Editor::DrawAddComponentButton()
	{
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
	}

	void Editor::DrawMemberInInspector(const std::string& name, const std::type_index& type, void* value, std::function<void()>* callbackOnChange)
	{
	    if (s_inspectorDrawers.find(type) != s_inspectorDrawers.end())	s_inspectorDrawers[type](name, value, callbackOnChange);
		else std::cerr << "Error: Couldn't find matching function in s_inspectorDrawers for a variable of type " << type.name() << " \n";
	}

	void Editor::UpdateSceneManager()
	{
		ImGui::Begin("Scenes");

		for (std::string sceneName : SceneManager::Scenes())
		{
		    ImGui::PushID(sceneName.c_str());

		    ImGui::Selectable(sceneName.c_str());
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				SceneManager::EnqueueSceneChange(sceneName);
			}
			else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
			    ImGui::OpenPopup("SceneOptions");
			}
			if (ImGui::BeginPopupContextItem("SceneOptions"))
			{
    			if (Input::GetKeyDown(KeyCode::Escape_Key))
    			{
    				ImGui::CloseCurrentPopup();
    			}
			    if (ImGui::Button("Delete"))
				{
				    std::cout << "Scene deleted: " << sceneName << "\n";
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		DrawCreateSceneButton();
	}

	void Editor::DrawCreateSceneButton()
	{
		static bool nameError = false; // True if the user tries to create a scene with a name that is already used
		static char newSceneName[128] = "newScene";
		if (ImGui::Button("+\n"))
		{
			nameError = false;
			std::strncpy(newSceneName, "newScene", sizeof(newSceneName));
			newSceneName[sizeof(newSceneName) - 1] = '\0';
			ImGui::OpenPopup("CreateNewScene");
		}
		if (ImGui::BeginPopupModal("CreateNewScene"))
		{
			if (Input::GetKeyDown(KeyCode::Escape_Key))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::Text("Insert scene's name:");
			bool enterPressed = ImGui::InputText("##sceneName", newSceneName, IM_ARRAYSIZE(newSceneName), ImGuiInputTextFlags_EnterReturnsTrue);

			ImGui::Separator();

			if (enterPressed)
			{
				if (SceneManager::CheckIfSceneAlreadyExists(newSceneName))
				{
					nameError = true;
				}
				else
				{
					if (SceneManager::Scenes().size()>0) SaveSystem::SaveCurrentScene();
					SceneSPtr newScene = SceneManager::CreateDefaultScene(newSceneName);
					SaveSystem::SaveScene(newScene.get());
					SceneManager::UpdateScenes();
					SceneManager::EnqueueSceneChange(newScene->name());
					std::cout << "Scene created\n";
					ImGui::CloseCurrentPopup();
				}
			}
			if (nameError)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name already used!");
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}
