#include "CameraMove.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../GameObject/Player.h"

int CameraMove::shakeCount = 2;
bool CameraMove::viewShakeLeft = true;
bool CameraMove::viewShakeRight;

bool CameraMove::viewShake = false;
bool CameraMove::viewSet = true;

Vector2f CameraMove::view = {0, 0};
float CameraMove::shakeSpeed = 0.f;
float CameraMove::shakeXPos = 0.f;

void CameraMove::CameraMoving()
{
	
}

void CameraMove::CameraShake(float dt)
{
	Scene* currScene = SCENE_MGR->GetCurrentScene();
	if (shakeCount == 0 || viewShake == false)
	{
		auto player = currScene->FindGameObj("PLAYER");
		currScene->SetWorldViewCenter(player->GetPos());
		return;
	}

	if (viewSet)
	{
		view = currScene->GetWorldViewCenter();
		viewSet = false;
	}

	if (viewShakeLeft == true)
	{
		currScene->SetWorldViewCenter(view.x - (dt * shakeSpeed), view.y);
		if (view.x <= view.x + shakeXPos)
		{
			viewShakeLeft = false;
			viewShakeRight = true;
		}
	}
	if (viewShakeRight == true)
	{
		currScene->SetWorldViewCenter(view.x + (dt * shakeSpeed), view.y);
		if (view.x >= view.x)
		{
			viewShakeLeft = true;
			viewShakeRight = false;
			shakeCount--;
		}

		if (shakeCount == 0)
		{
			shakeCount = 2;
			viewShake = false;
			viewSet = true;
		}
	}
}
