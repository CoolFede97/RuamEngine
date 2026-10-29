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
	    std::cout << "Hello, I am " << entity()->name() << " and I collided with " << col.other->name() << "!\n";
	}
	void Sandbox::onCollisionExit(const CollisionInfo& col)
	{
	    std::cout << "Hello, I am " << entity()->name() << " and I stopped colliding with " << col.other->name() << "!\n";
	}
	void Sandbox::onCollisionStay(const CollisionInfo& col)
	{
	    std::cout << "This shouldn't be called!\n";
	}
    DEF_REGISTER_COMPONENT(Sandbox);
}
