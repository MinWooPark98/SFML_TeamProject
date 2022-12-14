#include "Enemy.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"
#include "../Ui/ShowDamage.h"
#include "../Framework/CameraMove.h"
#include "../GameObject/HitSpark.h"
#include "Gold.h"
#include "ChaosFragments.h"

void Enemy::Init()
{
	Release();
	SpriteObj::Init();
	sprite.setScale({0, 1});
	animation.SetTarget(&sprite);
	position = { 0, 0 };
	shader.loadFromFile("shaders/palette.frag", Shader::Fragment);
	weapon = new SpriteObj();
	type = MonsterType::None;
	curState = States::RightIdle;

	shadow.setFillColor({ 0, 0, 0, 100 });
	shadow.setRadius(7.5f);
	Utils::SetOrigin(shadow, Origins::MC);
	shadow.setScale({ 1.f, 0.6f });

	spawn = new SpriteObj();
	spawn->SetOrigin(Origins::MC);
	spawnAnimation.SetTarget(&spawn->GetSprite());
	spawnAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("MonsterSpawnCard"));
	spawnAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("MonsterCard"));
	cardShader.loadFromFile("shaders/palette.frag", Shader::Fragment);
	SetCardPalette(19, 2, "graphics/CardColorIndex.png");

	fallingScale = { 1.f, 1.f };
}

void Enemy::Update(float dt)
{ 
	SpriteObj::Update(dt);

	SetLastPosition(GetPos());
	shadow.setPosition(lowhitbox.getPosition());

	if (isActionStart)
	{
		if (type == MonsterType::Normal || type == MonsterType::StageBoss)
		{
			switch (curState)
			{
			case States::LeftIdle: case States::RightIdle:
				UpdateIdle();
				break;
			case States::LeftMove: case States::RightMove:
				UpdateMove(2.f);
				break;
			case States::Attack: case States::MoveAttack:
				UpdateAttack(dt);
				break;
			case States::Hit:
				UpdateHit(dt);
				break;
			case States::Fall:
				UpdateFall(dt);
				break;
			case States::Die:
				SetState(States::Die);
				break;
			}
		}
	}
	if (type == MonsterType::MiddleBoss)
	{
		switch (curBossState)
		{
		case Enemy::BossStates::Idle:
			break;
		case Enemy::BossStates::Move:
			UpdateMove(0.5f);
			break;
		case Enemy::BossStates::Attack:
			UpdateAttack(dt);
			break;
		case Enemy::BossStates::Hit:
			UpdateHit(dt);
			break;
		case Enemy::BossStates::Die:
			SetState(BossStates::Die);
			break;
		case Enemy::BossStates::Clear:
			SetState(BossStates::Clear);
			break;
		}
	}

	if (curState == States::Attack || curState == States::MoveAttack || curBossState == BossStates::Attack)
		attackDelay -= dt;

	if (curState == States::Die && dieTimer >= 0.f)
		dieTimer -= dt;
	if (dieTimer < 0)
		SetActive(false);

	if (type == MonsterType::Normal || type == MonsterType::StageBoss)
	{
		if (isSpawn && !isActionStart)
		{
			if (spawn->GetSprite().getScale().x <= 0.f && spawn->GetSprite().getScale().x > -0.1f)
			{
				spawn->SetTexture(*RESOURCE_MGR->GetTexture("graphics/CardSpawnUnindexed_25.png"));
				spawnAnimation.Stop();
			}

			if (type == MonsterType::Normal)
				SpawnScale(1.f, dt);
			else if (type == MonsterType::StageBoss)
				SpawnScale(1.5f, dt);
		}

		if (isActionStart)
		{
			isShader = false;
			deleteTimer -= dt;
		}
		spawnAnimation.Update(dt);
	}
}

void Enemy::SetColor(int index)
{
	int saveIndex = paletteIndex;

	paletteIndex = (paletteIndex - index) % paletteSize;
	shader.setUniform("colorTable", texColorTable);
	shader.setUniform("paletteIndex", (float)paletteIndex / paletteSize);

	paletteIndex = saveIndex;

}

void Enemy::NormalMonsterMove(float dt)
{
	if (curState != States::Hit && curState != States::Fall)
	{
		if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale())
		{
			player->GetPos().x > GetPos().x ? direction.x = 1 : direction.x = -1;
			player->GetPos().y > GetPos().y ? direction.y = 1 : direction.y = -1;
		}
		else
			direction.x = 0;

		if (curState == States::Die)
			return;

		if (type == MonsterType::Normal || type == MonsterType::StageBoss)
		{
			if (!Utils::EqualFloat(direction.x, 0.f))
			{
				auto move = Utils::Normalize(player->GetPos() - GetPos());
				Translate({ dt * speed * move });

				if (lastDir.x < 0.f)
					SetState(States::LeftMove);
				if (lastDir.x > 0.f)
					SetState(States::RightMove);

				return;
			}

			if (Utils::EqualFloat(direction.x, 0.f))
			{
				if (lastDir.x < 0.f)
					SetState(States::LeftIdle);
				if (lastDir.x > 0.f)
					SetState(States::RightIdle);

				return;
			}
		}
	}
}

void Enemy::BossMonsterMove(float dt)
{
	player->GetPos().x > GetPos().x ? direction.x = 1 : direction.x = -1;
	player->GetPos().y > GetPos().y ? direction.y = 1 : direction.y = -1;

	if (curBossState == BossStates::Die || curBossState == BossStates::Clear || curBossState == BossStates::Hit)
		return;

	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		auto move = Utils::Normalize(player->GetPos() - GetPos());
		Translate({ dt * speed * move });
		SetState(BossStates::None);
		SetState(BossStates::Move);
		return;
	}
}

void Enemy::UpdateIdle()
{
	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		if (isActionStart)
			return;
		else
		{
			if (lastDir.x > 0.f)
				SetState(States::LeftMove);
			if (lastDir.x < 0.f)
				SetState(States::RightMove);
			return;
		}
	}
}

void Enemy::UpdateMove(int attackDelay)
{
	if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale() && type == MonsterType::Normal)
	{
		if (curState != States::Fall)
		{
			SetState(States::Attack);
			this->attackDelay = attackDelay;
			return;
		}
	}

	if (type == MonsterType::StageBoss)	
	{
		if (normalAttackCount <= 0)
		{
			if (Utils::Distance(player->GetPos(), GetPos()) <= GetEscapeScale())
				SetState(States::MoveAttack);

			else if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveAttackScale())
				SetState(States::MoveAttack);

			else if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale())
				SetState(States::Attack);
		}
		else
		{
			SetState(States::Attack);
		}

		this->attackDelay = attackDelay;
		return;
	}
}

void Enemy::SpawnScale(float scale, float dt)
{
	if (spawn->GetSprite().getScale().x > -scale)
	{
		spawn->SetScale({ spawn->GetSprite().getScale().x - (dt * 7) , scale });

		if (spawn->GetSprite().getScale().x <= 0.f && sprite.getScale().x <= scale)
			sprite.setScale({ sprite.getScale().x + (dt * 7), scale });
	}
	else if (spawn->GetSprite().getScale().x <= -scale)
	{
		isActionStart = true;
		spawnAnimation.Play("MonsterCard");
	}
}

void Enemy::Draw(RenderWindow& window)
{ 
	window.draw(shadow);
	if (type != MonsterType::MiddleBoss)
	{
		Object::Draw(window);

		if (isShader)
		{
			if (spawn->GetSprite().getScale().x >= 0.f)
				window.draw(spawn->GetSprite(), &cardShader);
			else
				spawn->Draw(window);
		}
		else
		{
			if (deleteTimer >= 0.f)
				spawn->Draw(window);
		}
	}
	else
	{
		SpriteObj::Draw(window);
	}
}

void Enemy::Reset()
{
	SpriteObj::Reset();
	deleteTimer = 0.3f;
	isSpawn = false;
	isActionStart = false;
	isShader = true;
	isAlive = true;
	moveSoundTimer = 0.f;
	curState = States::RightIdle;
}

void Enemy::UpdateHit(float dt)
{
	hitTimer += dt;
	if (type == MonsterType::Normal)
		Translate(-lastDir * speed * 0.3f * dt);
	
	if (type == MonsterType::Normal || type == MonsterType::StageBoss)
	{
		if (hitTimer >= 0.5f)
		{
			hitTimer = 0.f;

			if (IsStanding())
			{
				if (lastDir.x > 0.f)
					SetState(States::LeftIdle);
				if (lastDir.x < 0.f)
					SetState(States::RightIdle);
			}
		}
	}
	if (type == MonsterType::MiddleBoss)
	{
		if (hitTimer >= 2.f)
		{
			hitTimer = 0.f;
			SetState(BossStates::Move);
		}
	}
}

void Enemy::OnHit(const Vector2f& atkDir, int dmg)
{
	if (curState == States::Fall)
		return;

	PlayScene* playScene = (PlayScene*)SCENE_MGR->GetCurrentScene();
	auto showDamage = playScene->GetShowDamage()->Get();
	showDamage->ShowDamageFire(position, dmg);
	auto hitSpark = playScene->GetEnemyHitSpark()->Get();
	hitSpark->EnemyHitSparkFire(position);
	curHp -= dmg;
	direction = -atkDir;
	lastDir = direction;
	SOUND_MGR->Play("sounds/ImpactPhysicalLight.wav");
	CameraMove::SetShakeSpeed(200.f);
	CameraMove::SetShakeViewX(10.f);
	CameraMove::SetViewShake(true);
	if (type == MonsterType::Normal)
		SetState(States::Hit);
	else if (type == MonsterType::StageBoss)
	{
		if (curState == States::MoveAttack)
			SetState(States::Hit);
	}
	else if (type == MonsterType::MiddleBoss)
	{
		if (curBossState == BossStates::Idle)
		{
			SetState(BossStates::Hit);
		}
		if (curBossState == BossStates::Idle ||
			curBossState == BossStates::Hit)
		{
			int i = Utils::RandomRange(0, 3);

			switch (i)
			{
			case 0:
				SOUND_MGR->Play("sounds/FireBossHurt0.wav");
				break;
			case 1:
				SOUND_MGR->Play("sounds/FireBossHurt1.wav");
				break;
			case 2:
				SOUND_MGR->Play("sounds/FireBossHurt2.wav");
				break;
			}
		}
	}
}

void Enemy::SetProperty(float goldPer, int goldDropNum, int minG, int maxG, float platinumPer, int platinumDropNum)
{
	goldProbability = goldPer;
	goldDropNumber = goldDropNum;
	minGold = minG;
	maxGold = maxG;
	platinumProbability = platinumPer;
	platinumDropNumber = platinumDropNum;
}

void Enemy::Drop(PlayScene* scene)
{
	float goldDrop = Utils::RandomRange(0.f, 100.f);
	if (goldDrop <= GetGoldPer())
	{
		for (int i = 0; i < GetGoldDropNum(); i++)
		{
			auto gold = scene->GetGold()->Get();
			gold->SetGold(Utils::RandomRange(GetMinGold(), GetMaxGold()));
			gold->SetGoldPos(GetPos());
		}
	}

	float platinumDrop = Utils::RandomRange(0.f, 100.f);
	if (platinumDrop <= GetPlatinumPer())
	{
		for (int i = 0; i < GetPlatinumDropNum(); i++)
		{
			auto platinum = scene->GetPlatinum()->Get();
			platinum->SetPosition(GetPos());
		}
	}

	SOUND_MGR->Play("sounds/GoldSpawn.wav");
}

void Enemy::UpdateFall(float dt)
{
	fallingScale.x -= dt * 0.7f;
	fallingScale.y -= dt * 0.7f;
	sprite.setScale(fallingScale);
	fallTimer += dt;
	if (fallTimer >= 1.f)
	{
		fallTimer = 0.f;
		curHp -= curHp;
		return;
	}
}

bool Enemy::IsStanding()
{
	auto currScene = SCENE_MGR->GetCurrentScene();
	if (currScene->GetType() != Scenes::Play)
		return true;
	vector<map<Object::ObjTypes, list<Object*>>>& collisionList = ((PlayScene*)currScene)->GetCollisionList();
	for (int i = 0; i < collisionList.size(); ++i)
	{
		if (collisionList[i][Object::ObjTypes::Player].empty())
			continue;
		for (auto& cliff : collisionList[i][Object::ObjTypes::Cliff])
		{
			if (cliff->GetHitBounds().intersects(GetLowHitBounds()))
			{
				SetState(States::Fall);
				return false;
			}
		}
	}
	return true;
}