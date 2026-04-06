#include "ComponentsInitializer.h"
#include "Camera.h"
#include "GlobalLight.h"
#include "ModelRenderer.h"
#include "Transform.h"
namespace RuamEngine
{
    void ComponentsInitializer::InitComponents()
    {
        Transform::TransformRegister();
        Camera::CameraRegister();
        GlobalLight::GlobalLightRegister();
        ModelRenderer::ModelRendererRegister();
    }
}
