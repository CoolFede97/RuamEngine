#include "Sandbox.h"
#include "Scene.h"
namespace RuamEngine
{
    void Sandbox::update()
    {
		if (Input::GetKeyDown(KeyCode::SpaceBar_Key))
		{
            Entity* newEntity = SceneManager::ActiveScene()->createEntity("fede"+std::to_string(entityCount));
            entityCount++;
            newEntity->addComponent<ModelRenderer>()->setModel("RuamCore/Assets/Models/Fede.obj");
            newEntity->transform()->setPosition(lastEntityPos+Vec3(4,0,0));
            lastEntityPos = newEntity->transform()->position();
            SceneManager::ActiveScene()->getEntityByName("fede")->transform()->addChild(newEntity->transform());
		}
    // transform()->position().z-=10*RuamTime::DeltaTime();
	};
    DEF_REGISTER_COMPONENT(Sandbox);
}
