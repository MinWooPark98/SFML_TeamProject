#include "Heal.h"
#include "../Framework/Utils.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../GameObject/Player.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/SoundMgr.h"
#include "../Ui/ShowDamage.h"

void Heal::Init()
{
	SpriteObj::Init();
	SetTexture(*RESOURCE_MGR->GetTexture("graphics/Map/Object/HealthCrystal.png"));
	SetOrigin(Origins::MC);
	SetHitBox({ 20, 20, 15, 30 }, Color::Red);
	hitbox.setOrigin(hitbox.getSize().x * 0.5f, hitbox.getSize().y * 0.5f);
}

void Heal::Update(float dt)
{
	SpriteObj::Update(dt);
}

void Heal::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
}

void Heal::Reset()
{
	SpriteObj::Reset();
}

void Heal::OnHit(int dmg, int heal)
{
	if (GetActive())
	{
		PlayScene* playScene = (PlayScene*)SCENE_MGR->GetCurrentScene();
		auto showDamage = playScene->GetShowDamage()->Get();
		showDamage->ShowDamageFire(position, dmg);
		SOUND_MGR->Play("sounds/ImpactPhysicalLight.wav");

		player = (Player*)playScene->FindGameObj("PLAYER");

		if (player->GetCurHp() + heal >= player->GetMaxHp())
			player->SetCurHp(player->GetMaxHp());
		else
			player->SetCurHp(player->GetCurHp() + heal);
		SOUND_MGR->Play("sounds/HealRegen.wav");

		SetActive(false);
	}
}
