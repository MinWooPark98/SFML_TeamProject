#include "HeavyBombingArcher.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlayScene.h"

void HeavyBombingArcher::Init()
{
	Archer::Init();
	arrow->SetScale({1, 1});
	weapon->SetScale({1.5, 1.5});
	archerAttackArm->SetScale({ 1.5, 1.5 });
	archerPullArm->SetScale({1.5, 1.5});
	arrow->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ArrowLarge.png"));
	arrow->SetHitBox((FloatRect)arrow->GetSprite().getTextureRect());
	arrow->SetOrigin(Origins::MC);
	arrowSpeed = 400.f;
	arrowDir.setSize({ 1, 400 });
	SetMonsterType(MonsterType::StageBoss);

	SetMoveScale(400.f);
	SetAttackScale(300.f);
	SetMoveAttackScale(200.f);
	SetEscapeScale(100.f);
	SetNormalAttackCount(2);
	
	for (int i = 0; i < smollArrowCount; i++)
		smollArrow.push_back(new SpriteObj());
	
	for (auto it : smollArrow)
	{
		it->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Arrow.png"));
		it->SetHitBox({20, 20, 10, 40}, Color::Red);
		it->SetScale({1.5, 1.5});
		it->SetOrigin(Origins::MC);
	}


	spawn->SetScale({ 1.5, 1.5 });
	spawn->SetPos(GetPos());
	SetCardColor(1);

	SetMaxHp(500);
	SetCurHp(500);
	SetDamage(20);

	SetHitBox({ 20.f, 20.f, 20.f, 40.f }, Color::Red);
	hitbox.setOrigin(GetHitBox().getSize().x * 0.5f, GetHitBox().getSize().y * 0.5f);
}

void HeavyBombingArcher::Update(float dt)
{
	Archer::Update(dt);

	if (attackDelay <= 0.f)
		count = 0;

	for (auto& arrows : smollArrow)
	{
		arrows->GetHitBox().setPosition(arrows->GetPos());
	}
}

void HeavyBombingArcher::Draw(RenderWindow& window)
{
	Archer::Draw(window);

	if (curState == States::MoveAttack)
	{
		for (auto it : smollArrow)
		{
			if (it->GetActive())
			{
				if (isDevMode)
					window.draw(it->GetHitBox());

				it->Draw(window);
			}
		}

		arrowDir.setScale({ 1, 1 });
	}
	else
		arrowDir.setScale({ 1.5, 1 });
}

void HeavyBombingArcher::UpdateAttack(float dt)
{
	if (attackDelay <= 0.f)
	{
		if (curState == States::Attack)
			SetNormalAttackCount(GetNormalAttackCount() - 1);
		else
			SetNormalAttackCount(2);
	}
	else if (attackDelay >= attackStart + 0.2f)
	{
		archerAttackArm->SetPos(weapon->GetPos());
		archerPullArm->SetPos(weapon->GetPos() + Utils::Normalize((playerLastPos - GetPos())) * -10.f);
	}

	Archer::UpdateAttack(dt);

	if (attackDelay <= 0.f)
	{
		if (Utils::Distance(playerLastPos, GetPos()) <= GetEscapeScale())
			pattern = Pattern::EscapeAttack;

		else if (Utils::Distance(playerLastPos, GetPos()) <= GetMoveAttackScale())
			pattern = Pattern::MovingAttack;

		lastPos = GetPos();
	}

	if (curState == States::MoveAttack)
	{
		if (attackDelay >= attackStart + 0.2f)
		{
			for (auto it : smollArrow)
			{
				it->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);
				it->SetPos(GetPos());
				it->GetHitBox().setRotation(it->GetSprite().getRotation());
				it->SetActive(true);
			}
		}
		else if (attackDelay <= attackStart)
		{
			if (pattern == Pattern::MovingAttack || pattern == Pattern::EscapeAttack)
			{
				for (int i = smollArrow.size() - 1; i > count; i--)
				{
					smollArrow[i]->SetPos(GetPos());
					smollArrow[i]->SetActive(true);
				}

				weapon->SetPos(GetPos() - Utils::Normalize((playerLastPos - lastPos)) * -3.f);
				archerAttackArm->SetPos(weapon->GetPos());
				archerPullArm->SetPos(weapon->GetPos() + Utils::Normalize((playerLastPos - lastPos)) * -10.f);
				move = Utils::Normalize(playerLastPos - lastPos);

				if (pattern == Pattern::EscapeAttack)
				{
					Translate({ dt * speed * move * -1.f });
				}
				else
				{
					// 일자로 안감 ㅎㅎ 무조건 대각선
					if (playerLastPos.y > lastPos.y)
						move = { move.x * 0.7f, 1 };
					else
						move = { move.x * 0.7f, -1 };

					Translate({ dt * speed * move });
				}
			}

			smollArrowDelay -= dt;

			auto shot = Utils::Normalize(playerLastPos - lastPos);
			for (int i = 0; i <= count; ++i)
			{
				smollArrow[i]->Translate({ dt * arrowSpeed * shot * 2.f });

				if (Utils::OBB(player->GetHitBox(), smollArrow[i]->GetHitBox()))
				{
					if (isAttack)
					{
						player->SetCurHp(player->GetCurHp() - GetDamage());
						smollArrow[i]->SetActive(false);
						isAttack = false;
					}
				}
			}

			if (smollArrowDelay <= 0.f && count < 4)
			{
				smollArrowDelay = 0.1f;
				isAttack = true;
				SOUND_MGR->Play("sounds/ArcherAttackRelease.wav");
				SOUND_MGR->Play("sounds/ArcherArrow.wav");
				count++;
			}
		}
	}

	auto& collisionList = ((PlayScene*)SCENE_MGR->GetCurrentScene())->GetCollisionList();

	for (int i = 0; i < collisionList.size(); i++)
	{
		if (collisionList[i][Object::ObjTypes::Player].empty())
			continue;

		for (auto& coll : collisionList[i][Object::ObjTypes::Wall])
		{
			for (int a = 0; a < smollArrow.size(); a++)
			{
				if (smollArrow[a]->GetHitBounds().intersects(coll->GetHitBounds()))
				{
					smollArrow[a]->SetActive(false);
				}
			}
		}
	}
}

void HeavyBombingArcher::SetState(States newState)
{
	Archer::SetState(newState);

	if (newState == States::MoveAttack)
		smollArrowDelay = 0.1f;
}