#include "EditorCamera.h"
#include "Cursor.h"
#include "Input.h"
#include "KeyCode.h"
#include "Vec2.h"
#include "Vec3.h"
#include "RuamTime.h"

#include <algorithm>

namespace RuamEngine
{
    const glm::vec3 EditorCamera::s_up = glm::vec3(0.0, 1.0, 0.0);
    float EditorCamera::s_nearPlane = 0.1f;
    float EditorCamera::s_farPlane = 3000.0f;
    float EditorCamera::s_fov = 45.0f;
    float EditorCamera::s_aspectRatio = 800.0f/600.0f;

    float EditorCamera::s_speed = 50.0f;
    float EditorCamera::s_rotationSpeed = 0.1f;

    glm::vec3 EditorCamera::s_pos = {0,0,0};
    glm::vec3 EditorCamera::s_rot = {0,0,0};

    glm::mat4 EditorCamera::ProjectionMatrix()
	{
		return glm::perspective(glm::radians(s_fov), s_aspectRatio, s_nearPlane, s_farPlane);
	}
	glm::mat4 EditorCamera::ViewMatrix()
	{
		glm::vec3 eulerRadians = glm::radians(s_rot);
		glm::vec3 direction;

		float pitch = eulerRadians.x;
		float yaw = eulerRadians.y;

		direction.x = cos(pitch) * sin(yaw);
		direction.y = sin(pitch);
		direction.z = cos(pitch) * cos(yaw);

		direction = glm::normalize(direction);
		return glm::lookAt(s_pos, s_pos + direction, s_up);
	};

	void EditorCamera::UpdateCameraTransform()
	{
	    static bool firstClick = true;

	    if (!Input::GetMouseButtonDown(MouseCode::Mouse_Right))
        {
            if (Input::GetCursorMode()==CursorMode::MouseDisabled)
            {
                Input::SetCursorMode(CursorMode::MouseNormal);
            }
            firstClick = true;
            return;
        }

		if (Input::GetCursorMode() != CursorMode::MouseDisabled)
		{
		    Input::SetCursorMode(CursorMode::MouseDisabled);
		}
        Vec2 mouseRotation = Vec2(0.0f, 0.0f);
        Vec2 mouseTranslation = Input::GetMouseDeltaPix();
        if (firstClick)
        {
            mouseTranslation = {0,0};
            firstClick = false;
        }
        mouseRotation.x -= mouseTranslation.y;
        mouseRotation.y -= mouseTranslation.x;

       	s_rot += static_cast<glm::vec3>(mouseRotation * s_rotationSpeed);
    	s_rot.x = std::clamp(s_rot.x, -89.0f, 89.0f);

    	Vec3 horizontalDirection = {0, 0, 0};
    	if (Input::GetKey(KeyCode::W_Key)) horizontalDirection.z += 1;
    	if (Input::GetKey(KeyCode::S_Key)) horizontalDirection.z -= 1;
    	if (Input::GetKey(KeyCode::A_Key)) horizontalDirection.x -= 1;
    	if (Input::GetKey(KeyCode::D_Key)) horizontalDirection.x += 1;

    	Vec3 direction = Vec3::GetDirectionFromEuler(s_rot);

    	Vec3 left = Vec3::Up().CrossProduct(direction).Normalized();
    	s_pos += static_cast<glm::vec3>(direction * horizontalDirection.z * RuamTime::DeltaTime() * s_speed);
    	s_pos += static_cast<glm::vec3>(left * -horizontalDirection.x * RuamTime::DeltaTime() * s_speed);
	}
}
