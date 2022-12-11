#include "Dummy.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../Ui/ShowDamage.h"
#include "../Framework/CameraMove.h"
#include "../Framework/SoundMgr.h"
#include "../Framework/ResourceMgr.h"
#include "../GameObject/HitSpark.h"

Dummy::Dummy()
	: speed(50.f), resetTimer(0.f), curState(State::Normal)
{
}

void Dummy::Init()
{
	SpriteObj::Init();
	SetTexture(*RESOURCE_MGR->GetTexture("graphics/TrainingDummy.png"));
	SetOrigin(Origins::MC);
	SetHitBox({ 20, 20, 15, 30 }, Color::Red);
	hitbox.setOrigin(hitbox.getSize().x * 0.5f, hitbox.getSize().y * 0.5f);
	SetLowHitBox({20, 20, 15, 5}, Color::White);
	SetLowHitBoxOrigin(Origins::MC);
}

void Dummy::Update(float dt)
{
	SetLastPosition(GetPos());

	SpriteObj::Update(dt);

	switch (curState)
	{
	case Dummy::State::Normal:
		break;
	case Dummy::State::Hit:
		UpdateHit(dt);
		break;
	}

	Scene* currScene = SCENE_MGR->GetCurrentScene();
	if (currScene->GetType() != Scenes::Play)
		return;
	vector<map<Object::ObjTypes, list<Object*>>>& collisionList = ((PlayScene*)currScene)->GetCollisionList();
	for (int i = 0; i < collisionList.size(); ++i)
	{
		if (collisionList[i][Object::ObjTypes::Player].empty())
			continue;
		for (auto& cliff : collisionList[i][Object::ObjTypes::Cliff])
		{
			if (cliff->GetHitBounds().intersects(GetLowHitBounds()))
			{
				SetPos(lastPosition);
			}
		}
	}
}

void Dummy::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
}

void Dummy::OnHit(const Vector2f& atkDir, int dmg)
{
	PlayScene* playScene = (PlayScene*)SCENE_MGR->GetCurrentScene();
	auto showDamage = playScene->GetShowDamage()->Get();
	showDamage->ShowDamageFire(position, dmg);
	auto hitSpark = playScene->GetHitSpark()->Get();
	hitSpark->EnemyHitSparkFire(position);
	direction = -atkDir;
	lastDir = direction;
	SOUND_MGR->Play("sounds/ImpactPhysicalLight.wav");
	CameraMove::SetShakeSpeed(100.f);
	CameraMove::SetShakeViewX(5.f);
	CameraMove::SetViewShake(true);
	SetState(State::Hit);
}

void Dummy::UpdateHit(float dt)
{
	resetTimer += dt;

	if (resetTimer <= 0.3f)
	{
		Translate(-lastDir * speed * dt);
	}
	else if (resetTimer >= 2.f)
	{
		SetPos(initPos);
		SetState(State::Normal);
		resetTimer = 0.f;
	}
}

void Dummy::SetState(State s)
{
	resetTimer = 0.f;
	curState = s;
}
