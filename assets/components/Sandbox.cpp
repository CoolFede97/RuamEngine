#include "Sandbox.h"
#include "Cursor.h"
#include "Scene.h"
namespace RuamEngine
{
    void Sandbox::update()
    {
		if (Input::GetKeyDown(KeyCode::SpaceBar_Key))
		{
            Entity* newEntity = SceneManager::ActiveScene()->createEntity("fede"+std::to_string(entityCount));
            entityCount++;
            newEntity->addComponent<ModelRenderer>()->setModel("RuamCore/Assets/Models/tree.glb");
            newEntity->transform()->setPosition(lastEntityPos+Vec3(4,0,0));
            newEntity->transform()->setParent(transform());
            lastEntityPos = newEntity->transform()->position();
		}
    // transform()->translate(-10*RuamTime::DeltaTime(),0,0);
	};

	void Sandbox::onCollisionEnter(const CollisionInfo& col)
	{
	    std::cout << "On Collision Enter called\n";
	}
	void Sandbox::onCollisionExit(const CollisionInfo& col)
	{
	}
	void Sandbox::onCollisionStay(const CollisionInfo& col)
	{
	}
    DEF_REGISTER_COMPONENT(Sandbox);
}
