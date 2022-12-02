#include "CastingCircle.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "FinalBoss.h"

CastingCircle::CastingCircle()
	:animator(nullptr), duration(0.f), timer(0.f), dmgType(Skill::DamageType::Once), attackDmg(0), isOnAtkDelay(false), atkDelay(0.f), atkTimer(0.f), subType(Skill::SubjectType::None)
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
	isOnAtkDelay = false;
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
}

void CastingCircle::Update(float dt)
{
	SpriteObj::Update(dt);
	animator->Update(dt);
	auto spriteBnd = sprite.getGlobalBounds();
	SetHitBox(spriteBnd);
	SetHitBoxOrigin(Origins::MC);
	timer += dt;
	if (timer >= duration)
	{
		if (dmgType == Skill::DamageType::Once)
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
						if (GetHitBounds().intersects(enemy->GetHitBounds()))
						{
							((Enemy*)enemy)->SetCurHp(((Enemy*)enemy)->GetCurHp() - attackDmg);
							continue;
						}
					}
					for (auto& boss : collisionList[i][Object::ObjTypes::FinalBoss])
					{
						if (GetHitBounds().intersects(boss->GetHitBounds()))
						{
							((FinalBoss*)boss)->OnHit(direction, attackDmg);
							continue;
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
						if (GetHitBounds().intersects(player->GetHitBounds()))
						{
							((Player*)player)->OnHit(direction, attackDmg);
							continue;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		SetActive(false);
	}

	if (dmgType == Skill::DamageType::Periodic)
	{
		if (!isOnAtkDelay)
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
						if (GetHitBounds().intersects(enemy->GetHitBounds()))
						{
							((Enemy*)enemy)->SetCurHp(((Enemy*)enemy)->GetCurHp() - attackDmg);
							continue;
						}
					}
					for (auto& boss : collisionList[i][Object::ObjTypes::FinalBoss])
					{
						if (GetHitBounds().intersects(boss->GetHitBounds()))
						{
							((FinalBoss*)boss)->OnHit(direction, attackDmg);
							continue;
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
						if (GetHitBounds().intersects(player->GetHitBounds()))
						{
							((Player*)player)->OnHit(direction, attackDmg);
							continue;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		else
		{
			atkTimer += dt;
			if (atkTimer >= atkDelay)
			{
				isOnAtkDelay = false;
				atkTimer = 0.f;
			}
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
