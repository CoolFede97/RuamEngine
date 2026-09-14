#include "GameCamera.h"
#include "Entity.h"
#include "RuamEngine.h"
#include "Scene.h"
#include "SceneManager.h"

namespace RuamEngine
{
    GameCamera* GameCamera::s_mainCamera = nullptr;

    GameCamera::~GameCamera()
    {
        if (s_mainCamera)
        {
            if (s_mainCamera->id() == id() && SceneManager::ActiveScene())
            {
                EmptyMainCamera();
                for (Entity* entity : SceneManager::ActiveScene()->getEntities())
                {
                    GameCamera* gameCamera = entity->getComponent<GameCamera>();
                    if (gameCamera && gameCamera != this) gameCamera->setAsMainCamera();
                }
            }
        }
    }
	void GameCamera::setAsMainCamera()
	{
		s_mainCamera = this;
	}

	void GameCamera::EmptyMainCamera()
	{
		s_mainCamera = nullptr;
	}

	GameCamera* GameCamera::GetMainCamera()
	{
		return s_mainCamera;
	}

	void GameCamera::renderStart()
	{
	    setAsMainCamera();
	}
	void GameCamera::renderUpdate()
	{
        setTransform({transform()->position(), transform()->rotation()});
	}
	DEF_REGISTER_COMPONENT(GameCamera);
}
