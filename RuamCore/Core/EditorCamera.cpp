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
	void EditorCamera::updateCameraTransform()
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

       	m_transform.rot += static_cast<glm::vec3>(mouseRotation * m_rotationSpeed);
    	m_transform.rot.x = std::clamp(m_transform.rot.x, -89.0f, 89.0f);

    	Vec3 horizontalDirection = {0, 0, 0};
    	if (Input::GetKey(KeyCode::W_Key)) horizontalDirection.z += 1;
    	if (Input::GetKey(KeyCode::S_Key)) horizontalDirection.z -= 1;
    	if (Input::GetKey(KeyCode::A_Key)) horizontalDirection.x -= 1;
    	if (Input::GetKey(KeyCode::D_Key)) horizontalDirection.x += 1;

    	Vec3 direction = Vec3::GetDirectionFromEuler(m_transform.rot);

    	Vec3 left = Vec3::Up().CrossProduct(direction).Normalized();
    	m_transform.pos += static_cast<glm::vec3>(direction * horizontalDirection.z * RuamTime::DeltaTime() * m_speed);
    	m_transform.pos += static_cast<glm::vec3>(left * -horizontalDirection.x * RuamTime::DeltaTime() * m_speed);
	}
}
