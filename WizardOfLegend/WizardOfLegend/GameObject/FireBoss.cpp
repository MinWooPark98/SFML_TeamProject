#include "FireBoss.h"

void FireBoss::Init()
{
	Enemy::Init();

	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossClear"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownFireball"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownThrowing"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftAttackReady"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftFireball"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftFly"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftLand"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftStern"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftStomp"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftThirdAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossLeftThrowing"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightAttackReady"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightFireball"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightFly"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightLand"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightStern"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightStomp"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightThirdAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossRightThrowing"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpFireball"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpThirdAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossUpThrowing"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("FireBossDownThirdAttack"));

	//SetPaletteIndex(44);
	//SetpaletteSize(9);
	//SetColorTable("graphics/ArcherColorIndex.png");

	SetPos({200, 200});
	SetSpeed(200.f);
	SetMoveScale(10000.f);
	SetAttackScale(10000.f);
	SetMonsterType(MonsterType::MiddleBoss);
	SetMaxHp(1);
	SetCurHp(GetMaxHp());
}

void FireBoss::Update(float dt)
{
	SpriteObj::Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::Key::L))
	{
		animation.Play("FireBossDownThirdAttack");
	}

	animation.Update(dt);
}

void FireBoss::Draw(RenderWindow& window)
{
	Enemy::Draw(window);
	window.draw(sprite);
}

void FireBoss::SetState(States newState)
{
	//Enemy::SetState(newState);
}

void FireBoss::UpdateAttack(float dt)
{
	//Enemy::UpdateAttack(dt);
}
