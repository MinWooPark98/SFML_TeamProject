#include "Dummy.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../Ui/ShowDamage.h"
#include "../Framework/CameraMove.h"
#include "../Framework/SoundMgr.h"
#include "../Framework/ResourceMgr.h"

Dummy::Dummy()
	: speed(50.f), resetTimer(0.f), curState(State::Normal)
{
}

void Dummy::Init()
{
	SpriteObj::Init();
	SetTexture(*RESOURCE_MGR->GetTexture("graphics/TrainingDummy.png"));
	SetHitBox({20, 20, 15, 30}, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
}

void Dummy::Update(float dt)
{
	SpriteObj::Update(dt);

	switch (curState)
	{
	case Dummy::State::Normal:
		break;
	case Dummy::State::Hit:
		UpdateHit(dt);
		break;
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
