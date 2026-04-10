#include "ComponentsInitializer.h"
#include "GlobalLight.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Sandbox.h"

namespace RuamEngine
{
    void ComponentsInitializer::InitComponents()
    {
        Transform::TransformRegister();
        GlobalLight::GlobalLightRegister();
        ModelRenderer::ModelRendererRegister();
        Sandbox::SandboxRegister();
    }
}
