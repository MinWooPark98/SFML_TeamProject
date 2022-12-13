#include "Summoner.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/StatTable.h"
#include "../Scene/PlayScene.h"
#include "../Scene/SceneMgr.h"
#include "../GameObject/Gold.h"
#include "../GameObject/ChaosFragments.h"
#include "../DataTable/PropertyTable.h"

Summoner::Summoner()
{
}

Summoner::~Summoner()
{
}

void Summoner::Init()
{
	Enemy::Init();

	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerLeftAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerLeftCast"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerLeftDead"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerLeftHurt"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerLeftIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerLeftMove"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerRightAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerRightCast"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerRightDead"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerRightHurt"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerRightIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("SummonerRightMove"));

	SetPaletteIndex(43);
	SetpaletteSize(9);
	SetColorTable("graphics/SummonerColorIndex.png");

	SetMoveScale(500.f);
	SetAttackScale(400.f);
	SetMonsterType(MonsterType::Normal);

	spawn->SetPos(GetPos());
	SetCardColor(2);

	auto statTable = DATATABLE_MGR->Get<StatTable>(DataTable::Types::Stat);
	auto& stat = statTable->Get("Summoner");
	SetDamage(stat.attackDmg);
	SetMaxHp(stat.maxHp);
	SetSpeed(stat.speed);
	SetCurHp(GetMaxHp());

	SetHitBox({ 20.f, 20.f, 20.f, 30.f }, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
	SetLowHitBox({ 20.f, 20.f, 15.f, 5.f }, Color::White);
	SetLowHitBoxOrigin(Origins::MC);

	for (int i = 0; i < 5; i++)
	{
		fires.push_back(new SpriteObj());
		fires[i]->UseShader();
		fires[i]->SetTexture(*RESOURCE_MGR->GetTexture("graphics/NewFire_0.png"));
		fires[i]->SetOrigin(Origins::MC);
		fires[i]->SetHitBox((FloatRect)fires[i]->GetTextureRect());
		fires[i]->SetHitBoxOrigin(Origins::MC);

		fires[i]->SetSpriteShader();
		fires[i]->SetSpritePalette(5, "graphics/FireColorIndex.png");
		fires[i]->SetPaletteColor(2);
		
		fires[i]->SetActive(false);
	}

	for (int i = 0; i < 5; i++)
	{
		fireAnimations.push_back(new Animator());
		fireAnimations[i]->SetTarget(&fires[i]->GetSprite());
		fireAnimations[i]->AddClip(*RESOURCE_MGR->GetAnimationClip("FireAnimation"));
	}

	auto propertyTable = DATATABLE_MGR->Get<PropertyTable>(DataTable::Types::MonsterProperty);
	auto& p = propertyTable->Get("Summoner");
	SetProperty(p.goldProbability, p.goldDropNumber, p.minGold, p.maxGold, p.platinumProbability, p.platinumDropNumber);
}

void Summoner::Reset()
{
}

void Summoner::Update(float dt)
{
	Enemy::Update(dt);
	if (isAlive)
	{
		if (!isSpawn)
		{
			spawnAnimation.Play("MonsterSpawnCard");
			animation.Play("SummonerLeftIdle");
			isSpawn = true;
		}
		else if (isActionStart)
		{
			if (curState != States::Hit)
			{
				if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale() + 1.f && curState != States::Attack)
				{
					NormalMonsterMove(dt);
					
					moveSoundTimer -= dt;
					if (moveSoundTimer <= 0.f)
					{
						SOUND_MGR->Play("sounds/LightFootstep.wav");
						moveSoundTimer = 0.4f;
					}
				}
			}

			if (curHp <= 0 && isAlive)
			{
				dieTimer = 1.f;
				SetState(States::Die);
				
				PlayScene* playScene = (PlayScene*)SCENE_MGR->GetCurrentScene();
				Drop(playScene);

				isAlive = false;
			}

			if (!Utils::EqualFloat(direction.x, 0.f))
			{
				lastDir = direction;
			}
		}
	}
	UpdateCollision(dt);

	animation.Update(dt);
	for (int i = 0; i < fireAnimations.size(); i++)
		fireAnimations[i]->Update(dt);
}

void Summoner::Draw(RenderWindow& window)
{
	for (int i = 0; i < fires.size(); i++)
	{
		if (fires[i]->GetActive())
		{
			fires[i]->Draw(window);
			if (isDevMode)
			{
				window.draw(fires[i]->GetHitBox());
			}
		}
	}

	Enemy::Draw(window);

	if (isAlive)
		window.draw(sprite, &shader);
	else
	{
		if (dieTimer >= 0.f)
			window.draw(sprite, &shader);
	}
}

void Summoner::SetState(States newState)
{
	if (curState == newState)
		return;

	curState = newState;

	float angle = 0.f;

	switch (curState)
	{
	case States::LeftIdle:
		animation.Play("SummonerLeftIdle");
		break;
	case States::RightIdle:
		animation.Play("SummonerRightIdle");
		break;
	case States::LeftMove:
		animation.Play("SummonerLeftMove");
		break;
	case States::RightMove:
		animation.Play("SummonerRightMove");
		break;
	case States::Attack:
		lastDir.x < 0.f ? animation.Play("SummonerLeftCast") : animation.Play("SummonerRightCast");
		break;
	case States::Hit:
		if (!isAttack)
		{
			for (int i = 0; i < fires.size(); i++)
			{
				if (fires[i]->GetActive())
					fires[i]->SetActive(false);
			}
		}
		isAttack = false;
		attackTimer = -0.3f;
		fireSet = true;
		lastDir.x < 0.f ? animation.Play("SummonerLeftHurt") : animation.Play("SummonerRightHurt");
		break;
	case States::Die:
		lastDir.x < 0.f ? animation.Play("SummonerLeftDead") : animation.Play("SummonerRightDead");
		SOUND_MGR->Play("sounds/EnemyDead.wav");
		break;
	}
}

void Summoner::UpdateAttack(float dt)
{
	if (curHp <= 0)
		return;

	attackTimer += dt;

	if (attackTimer <= attackStartTimer && attackTimer >= 0.f)
	{
		if (fireSet)
		{
			playerLastPos = player->GetPos();

			for (int i = 0; i < fires.size(); i++)
			{
				float angle = atan2(-(playerLastPos.y - GetPos().y), playerLastPos.x - GetPos().x) * 180.f / M_PI;
				angle = angle + ((i - 2) * 25.f);
				angle = angle * (M_PI / 180);
				Vector2f shot = { cos(angle), -sin(angle) };
				fires[i]->SetPos(GetPos() + Utils::Normalize((playerLastPos - GetPos())) + (shot * 30.f));

				fires[i]->SetActive(true);
				fireAnimations[i]->Play("FireAnimation");
			}
			SOUND_MGR->Play("sounds/FireballCast.wav");
			fireSet = false;
			isShot = false;
		}
	}
	else if (attackTimer >= attackStartTimer)
	{
		if (!isAttack)
		{
			if (playerLastPos.x >= GetPos().x)
				animation.Play("SummonerRightAttack");
			if (playerLastPos.x < GetPos().x)
				animation.Play("SummonerLeftAttack");

			SOUND_MGR->Play("sounds/FireBlast.wav");
		}

		isAttack = true;
		isShot = true;
	}

	if (attackTimer >= attackStartTimer + 1.f)
	{
		if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale())
		{
			if (lastDir.x < 0.f)
				SetState(States::LeftMove);
			if (lastDir.x > 0.f)
				SetState(States::RightMove);
		}
		else
		{
			direction.x = 0;

			if (lastDir.x < 0.f)
				SetState(States::LeftIdle);
			if (lastDir.x > 0.f)
				SetState(States::RightIdle);

			return;
		}

		attackTimer = 0.f;
		isAttack = false;
		fireSet = true;
	}
}

void Summoner::UpdateCollision(float dt)
{
	Scene* currScene = SCENE_MGR->GetCurrentScene();
	if (currScene->GetType() != Scenes::Play || curState == States::Fall || curState == States::Hit)
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

	auto& collList = ((PlayScene*)SCENE_MGR->GetCurrentScene())->GetCollisionList();
	for (int i = 0; i < collisionList.size(); i++)
	{
		if (collList[i][Object::ObjTypes::Player].empty())
			continue;

		for (auto& coll : collList[i][Object::ObjTypes::Wall])
		{
			for (int i = 0; i < fires.size(); i++)
			{
				if (fires[i]->GetHitBounds().intersects(coll->GetHitBounds()))
				{
					fires[i]->SetActive(false);
				}
			}
		}
	}

	if (attackTimer >= attackStartTimer)
	{
		for (int i = 0; i < fires.size(); i++)
		{
			if (fires[i]->GetActive())
			{
				if (Utils::OBB(player->GetHitBox(), fires[i]->GetHitBox()))
				{
					player->OnHit(direction, GetDamage());
					fires[i]->SetActive(false);
				}
			}
		}
	}

	if (isShot)
	{
		for (int i = 0; i < fires.size(); i++)
		{
			if (fires[i]->GetActive())
			{
				float angle = atan2(-(playerLastPos.y - GetPos().y), playerLastPos.x - GetPos().x) * 180.f / M_PI;
				angle = angle + ((i - 2) * 12.5f);
				angle = angle * (M_PI / 180);
				Vector2f shot = { cos(angle), -sin(angle) };

				fires[i]->Translate(dt * fireSpeed * shot);
				fires[i]->GetHitBox().setPosition(fires[i]->GetPos());
			}
		}
	}
}
