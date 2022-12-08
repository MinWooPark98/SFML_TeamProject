#include "Heal.h"
#include "../Framework/Utils.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../GameObject/Player.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/SoundMgr.h"
#include "ShowDamage.h"

void Heal::Init()
{
	SpriteObj::Init();
}

void Heal::Update(float dt)
{
	SpriteObj::Update(dt);
	animation.Update(dt);
}

void Heal::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
}

void Heal::Reset()
{
	SpriteObj::Reset();
}

void Heal::SetHealObject(string texture, string animationClipName, Vector2f hitBoxXY)
{
	SetTexture(*RESOURCE_MGR->GetTexture(texture));
	SetHitBox({ 20.f, 20.f, hitBoxXY.x, hitBoxXY.y }, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
	animation.SetTarget(&sprite);
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip(animationClipName));

	animation.Play(animationClipName);
}

void Heal::OnHit(int dmg, int heal)
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
