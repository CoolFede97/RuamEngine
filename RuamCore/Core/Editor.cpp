#include "Editor.h"
#include "Camera.h"
#include "Cursor.h"
#include "Entity.h"
#include "Component.h"
#include "GameCamera.h"
#include "KeyCode.h"
#include "SaveSystem.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Transform.h"
#include "EditorCamera.h"

#include "imgui.h"
#include <typeindex>
#include <utility>
#include <cstring>

namespace RuamEngine
{
	Entity* Editor::s_selectedEntity = nullptr;
	EditorCamera Editor::s_camera;

	void Editor::UpdateHierarchy()
	{
		ImGui::Begin("Hierarchy");
		Scene* scene = SceneManager::ActiveScene();
		if (scene != nullptr)
		{
            static bool nameError = false; // True if the user tries to create an entity with a name that is already used
 			static char newEntityName[128] = "newEntity";

			if (ImGui::Button("Create Entity"))
			{
			    nameError = false;
				std::strncpy(newEntityName, "newEntity", sizeof(newEntityName));
				newEntityName[sizeof(newEntityName) - 1] = '\0';
			    ImGui::OpenPopup("CreateEntity");
			}

			ImGui::SetNextWindowSize(ImVec2(187, 95), ImGuiCond_Always);
			if (ImGui::BeginPopupModal("CreateEntity",NULL, ImGuiWindowFlags_NoResize))
			{
    			if (Input::GetKeyDown(KeyCode::Escape_Key) || Input::GetMouseButtonDown(MouseCode::Mouse_Right))
    			{
    				ImGui::CloseCurrentPopup();
    			}
    		    if (ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();
                ImGui::SetNextItemWidth(-3.0f);
			    bool enterPressed = ImGui::InputText("##entityName", newEntityName, IM_ARRAYSIZE(newEntityName), ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::SetItemDefaultFocus();
				if (enterPressed)
				{
				    bool nameAlreadyUsed = SceneManager::ActiveScene()->getEntityByName(newEntityName)!=nullptr;
				    if (nameAlreadyUsed) nameError = true;
                    else
                    {
                        SceneManager::ActiveScene()->createEntity(newEntityName);
                        ImGui::CloseCurrentPopup();
                    };
				}
				if (nameError)
				{
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name already used!");
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			if (s_selectedEntity != nullptr)
			{
				if (Input::GetKeyDown(KeyCode::Delete_Key))
				{
					s_selectedEntity->destroy();
					s_selectedEntity = nullptr;
				}
			}

			ImGui::Separator();

			std::list<Entity*> entities = scene->getEntities();
			for (Entity* entity : entities)
			{
				if (entity->transform().m_parent) continue;
				DrawEntityFamily(entity);
			}
		}
		ImGui::End();
	}

	void Editor::DrawEntityFamily(Entity *entity)
	{
		std::list<Transform*> childrenTransform = entity->transform().children();

		bool selected = (s_selectedEntity == entity);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                        	ImGuiTreeNodeFlags_OpenOnDoubleClick  |
                            ImGuiTreeNodeFlags_SpanAvailWidth;

		if (selected) flags |= ImGuiTreeNodeFlags_Selected;
		if (childrenTransform.empty()) flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		ImGui::PushID(entity->name().c_str());

		bool opened = ImGui::TreeNodeEx(entity->name().c_str(), flags);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
            s_selectedEntity = entity;
		    ImGui::OpenPopup("EntityOptions");
		}
		if (ImGui::BeginPopupContextItem("EntityOptions"))
		{
		    if (ImGui::Button("Delete"))
			{
			    std::cout << "Entity deleted: " << entity->name() << "\n";
				if (s_selectedEntity == entity) s_selectedEntity = nullptr;
				entity->destroy();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (ImGui::IsItemClicked()) s_selectedEntity = entity;

		if (opened && !childrenTransform.empty())
		{
			for (Transform* transform : childrenTransform)
			{
				DrawEntityFamily(transform->entity());
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
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
		if (s_selectedEntity == nullptr) return;
		ImGui::Begin("Inspector");

		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text(s_selectedEntity->name().c_str());
		ImGui::SetWindowFontScale(1.0f);
		ImGui::SameLine();

		DrawAddComponentButton();

		Scene* scene = SceneManager::ActiveScene();
		if (scene != nullptr)
		{
			for (Component* com : s_selectedEntity->getComponents())
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

			if (ImGui::Button("Create component"))
			{
			    ImGui::OpenPopup("CreateNewComponent");
			}
			bool componentAdded = false;

			if (ImGui::BeginPopup("CreateNewComponent"))
			{
			    static char newComponentName[128] = "";
				if (ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();

				bool enterPressed = ImGui::InputText("Name", newComponentName, IM_ARRAYSIZE(newComponentName), ImGuiInputTextFlags_EnterReturnsTrue);
				if (enterPressed)
				{
				    std::cout << "TODO: Component created\n";
					newComponentName[0] = '\0';
					ImGui::CloseCurrentPopup();
					componentAdded = true;
				}

				ImGui::EndPopup();
			}

			for (auto& [cmpName, factory] : Component::componentRegistry)
			{
				if (cmpName == "Transform" || cmpName == "Component") continue;
				if (ImGui::Selectable(cmpName.c_str()))
				{
					factory.addComponent(s_selectedEntity);
					componentAdded = true;
				}
			}
			if (componentAdded) ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}

	void Editor::DrawMemberInInspector(const std::string& name, const std::type_index& type, void* value, std::function<void()>* callbackOnChange)
	{
	    if (s_inspectorDrawers.find(type) != s_inspectorDrawers.end())	s_inspectorDrawers[type](name, value, callbackOnChange);
		else std::cerr << "Error: Couldn't find matching function in s_inspectorDrawers for a variable of type " << type.name() << " \n";
	}

	// EditorCamera& Editor::Camera() { return s_camera; }

	void Editor::UpdateSceneManager()
	{
		ImGui::Begin("Scenes");

		for (std::string sceneName : SceneManager::Scenes())
		{
		    ImGui::PushID(sceneName.c_str());

			bool isSelected = sceneName == SceneManager::ActiveScene()->name();

		    ImGui::Selectable(sceneName.c_str(), isSelected);
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
			    if (ImGui::Button("Delete"))
				{
				    std::cout << "Scene deleted: " << sceneName << "\n";
					SaveSystem::EraseScene(sceneName);
					SceneManager::ResetActiveScene();
					SceneManager::RemoveScene(sceneName);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		DrawCreateSceneButton();
	}

	void Editor::UpdateViewport(FrameBuffer* fb, const char* windowName, bool editorCamera)
	{
    	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin(windowName);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        float aspectRatio = viewportPanelSize.x / viewportPanelSize.y;

        if (editorCamera)
        {
            if (Camera().aspectRatio()!=aspectRatio) Camera().setAspectRatio(aspectRatio);
        }
        else if (GameCamera::GetMainCamera() && GameCamera::GetMainCamera()->aspectRatio()!=aspectRatio)
        {
            for (Entity* entity : SceneManager::ActiveScene()->getEntities())
            {
                for (GameCamera* cameraCmp : entity->getComponentsOfType<GameCamera>())
                {
                    cameraCmp->setAspectRatio(aspectRatio);
                }
            }
        }

        if (fb->width() != viewportPanelSize.x || fb->height() != viewportPanelSize.y)
        {
            fb->rescale(viewportPanelSize.x, viewportPanelSize.y);
        }

        unsigned int textureID = fb->texture();
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)),
                    ImVec2{ (float)fb->width(), (float)fb->height() },
                    ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
        ImGui::PopStyleVar();
	}

	void Editor::UpdateCameraTransform()
	{
	    s_camera.updateCameraTransform();
	}
	void Editor::SetCameraTransform(CameraTransform cameraTransform)
	{
	    s_camera.setTransform(cameraTransform);
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
			if (Input::GetKeyDown(KeyCode::Escape_Key) || Input::GetMouseButtonDown(MouseCode::Mouse_Right))
			{
				ImGui::CloseCurrentPopup();
			}
		    if (ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();

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
