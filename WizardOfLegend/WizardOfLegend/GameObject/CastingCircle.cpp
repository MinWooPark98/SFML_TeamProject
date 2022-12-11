#include "CastingCircle.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "FinalBoss.h"
#include "../GameObject/Heal.h"
#include "Dummy.h"

CastingCircle::CastingCircle()
	:animator(nullptr), duration(0.f), timer(0.f), dmgType(Skill::DamageType::Once), attackDmg(0), atkDelay(0.f), atkTimer(0.f), subType(Skill::SubjectType::None)
{
}

CastingCircle::~CastingCircle()
{
}

void CastingCircle::Init()
{
	SpriteObj::Init();
	animator = new Animator();
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("CastingCircle"));
	animator->SetTarget(&sprite);
}

void CastingCircle::Release()
{
	SpriteObj::Release();
	if (animator != nullptr)
		delete animator;
	animator = nullptr;
}

void CastingCircle::Reset()
{
	SpriteObj::Reset();
	timer = 0.f;
	atkTimer = 0.f;
	switch (subType)
	{
	case Skill::SubjectType::Player:
		sprite.setColor({ 255, 255, 255, 255 });
		break;
	case Skill::SubjectType::Enemy:
		sprite.setColor({ 255, 0, 0, 255 });
		break;
	case Skill::SubjectType::FinalBoss:
		sprite.setColor({ 255, 0, 0, 255 });
		break;
	default:
		sprite.setColor({ 255, 255, 255, 255 });
		break;
	}
	damagedObjs.clear();
}

void CastingCircle::Update(float dt)
{
	animator->Update(dt);
	auto spriteBnd = sprite.getGlobalBounds();
	SetHitBox(spriteBnd);
	SetHitBoxOrigin(Origins::MC);
	SpriteObj::Update(dt);
	timer += dt;
	if (timer >= duration)
	{
		if (dmgType == Skill::DamageType::Once)
		{
			Scene* currScene = SCENE_MGR->GetCurrentScene();
			if (currScene->GetType() == Scenes::Play)
			{
				vector<map<Object::ObjTypes, list<Object*>>>& collisionList = ((PlayScene*)currScene)->GetCollisionList();
				switch (subType)
				{
				case Skill::SubjectType::Player:
					for (int i = 0; i < collisionList.size(); ++i)
					{
						if (collisionList[i][Object::ObjTypes::Player].empty())
							continue;
						for (auto& enemy : collisionList[i][Object::ObjTypes::Enemy])
						{
							if (!enemy->GetActive() || !((Enemy*)enemy)->GetIsAlive())
								continue;
							if (GetHitBounds().intersects(enemy->GetHitBounds()))
								((Enemy*)enemy)->OnHit(direction, attackDmg);
						}
						for (auto& boss : collisionList[i][Object::ObjTypes::FinalBoss])
						{
							if (!boss->GetActive() || ((FinalBoss*)boss)->GetState() == FinalBoss::States::Die)
								continue;
							if (GetHitBounds().intersects(boss->GetHitBounds()))
								((FinalBoss*)boss)->OnHit(direction, attackDmg);
						}
						for (auto& etc : collisionList[i][Object::ObjTypes::Dummy])
						{
							if (GetHitBounds().intersects(((Dummy*)etc)->GetHitBounds()))
							{
								((Dummy*)etc)->OnHit(direction, attackDmg);
								damagedObjs.push_back(etc);
							}
						}
						for (auto& etc : collisionList[i][Object::ObjTypes::BrokenObject])
						{
							if (GetHitBounds().intersects(((Heal*)etc)->GetHitBounds()))
							{
								((Heal*)etc)->OnHit(attackDmg, 60);
								damagedObjs.push_back(etc);
							}
						}
					}
					break;
				case Skill::SubjectType::Enemy:
				case Skill::SubjectType::FinalBoss:
					for (int i = 0; i < collisionList.size(); ++i)
					{
						if (collisionList[i][Object::ObjTypes::Player].empty())
							continue;
						for (auto& player : collisionList[i][Object::ObjTypes::Player])
						{
							if (!player->GetActive() || ((Player*)player)->GetState() == Player::States::Die)
								continue;
							if (GetHitBounds().intersects(player->GetHitBounds()))
								((Player*)player)->OnHit(direction, attackDmg);
						}
					}
					break;
				default:
					break;
				}
			}
		}
		SetActive(false);
	}

	if (dmgType == Skill::DamageType::Periodic)
	{
		Scene* currScene = SCENE_MGR->GetCurrentScene();
		if (currScene->GetType() != Scenes::Play)
			return;
		vector<map<Object::ObjTypes, list<Object*>>>& collisionList = ((PlayScene*)currScene)->GetCollisionList();
		switch (subType)
		{
		case Skill::SubjectType::Player:
			for (int i = 0; i < collisionList.size(); ++i)
			{
				if (collisionList[i][Object::ObjTypes::Player].empty())
					continue;
				for (auto& enemy : collisionList[i][Object::ObjTypes::Enemy])
				{
					if (!enemy->GetActive() || !((Enemy*)enemy)->GetIsAlive())
						continue;
					if (GetHitBounds().intersects(enemy->GetHitBounds()) && find(damagedObjs.begin(), damagedObjs.end(), enemy) == damagedObjs.end())
					{
						((Enemy*)enemy)->OnHit(direction, attackDmg);
						damagedObjs.push_back(enemy);
					}
				}
				for (auto& boss : collisionList[i][Object::ObjTypes::FinalBoss])
				{
					if (!boss->GetActive() || ((FinalBoss*)boss)->GetState() == FinalBoss::States::Die)
						continue;
					if (GetHitBounds().intersects(boss->GetHitBounds()) && find(damagedObjs.begin(), damagedObjs.end(), boss) == damagedObjs.end())
					{
						((FinalBoss*)boss)->OnHit(direction, attackDmg);
						damagedObjs.push_back(boss);
					}
				}
				for (auto& etc : collisionList[i][Object::ObjTypes::Dummy])
				{
					if (GetHitBounds().intersects(etc->GetHitBounds()) && find(damagedObjs.begin(), damagedObjs.end(), etc) == damagedObjs.end())
					{
						((Dummy*)etc)->OnHit(direction, attackDmg);
						damagedObjs.push_back(etc);
					}
				}
				for (auto& etc : collisionList[i][Object::ObjTypes::BrokenObject])
				{
					if (GetHitBounds().intersects(etc->GetHitBounds()) && find(damagedObjs.begin(), damagedObjs.end(), etc) == damagedObjs.end())
					{
						((Heal*)etc)->OnHit(attackDmg, 60);
						damagedObjs.push_back(etc);
					}
				}
			}
			break;
		case Skill::SubjectType::Enemy:
		case Skill::SubjectType::FinalBoss:
			for (int i = 0; i < collisionList.size(); ++i)
			{
				if (collisionList[i][Object::ObjTypes::Player].empty())
					continue;
				for (auto& player : collisionList[i][Object::ObjTypes::Player])
				{
					if (!player->GetActive() ||((Player*)player)->GetState() == Player::States::Die)
						continue;
					if (GetHitBounds().intersects(player->GetHitBounds()) && find(damagedObjs.begin(), damagedObjs.end(), player) == damagedObjs.end())
					{
						((Player*)player)->OnHit(direction, attackDmg);
						damagedObjs.push_back(player);
					}
				}
			}
			break;
		default:
			break;
		}
		atkTimer += dt;
		if (atkTimer >= atkDelay)
		{
			atkTimer = 0.f;
			damagedObjs.clear();
		}
	}
}

void CastingCircle::Do()
{
	if(animator != nullptr)
		animator->Play("CastingCircle");
}

void CastingCircle::SetSubjectType(Skill::SubjectType type)
{
	subType = type;
	switch (type)
	{
	case Skill::SubjectType::Player:
		sprite.setColor({ 255, 255, 255, 255 });
		break;
	case Skill::SubjectType::Enemy:
		sprite.setColor({ 255, 0, 0, 255 });
		break;
	case Skill::SubjectType::FinalBoss:
		sprite.setColor({ 255, 0, 0, 255 });
		break;
	default:
		break;
	}
}
