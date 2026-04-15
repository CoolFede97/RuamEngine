#pragma once

#include "Camera.h"
#include "FrameBuffer.h"
#include <functional>
#include <string>
#include <typeindex>
#include <unordered_map>
namespace RuamEngine
{
    using SerializedMemberDrawer = std::function<void(const std::string&, void*, std::function<void()>*)>;

	class Entity;
	class EditorCamera;

	class Editor
	{
	public:
		static void UpdateHierarchy();
		static void UpdateInspector();
		static void UpdateSceneManager(); // Doesn't update anything from the class SceneManager itself. It refers to the UI.
		static void UpdateViewport(FrameBuffer* fb);
		static void UpdateCameraTransform();
		static void SetCameraTransform(CameraTransform cameraTransform);

		static void DrawAddComponentButton();
		static void DrawCreateSceneButton();

		static void DrawEntityFamily(Entity* entity);
		static void DrawMemberInInspector(const std::string& name, const std::type_index& type, void* value, std::function<void()>* callbackOnChange);

		static EditorCamera& Camera() { return s_camera; };

		#define DRAW_MEMBER_NAME(name)  \
    		std::string label = name + ": ";    \
            std::string labelCopy = "##"+name;  \
            ImGui::Text("%s",label.c_str());    \
            ImGui::SameLine();

		static Entity* s_selectedEntity;
		static EditorCamera s_camera;
		static std::unordered_map<std::type_index, SerializedMemberDrawer> s_inspectorDrawers;
	};
}
