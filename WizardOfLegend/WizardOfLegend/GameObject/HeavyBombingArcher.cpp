#include "HeavyBombingArcher.h"

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
	arrowDir.setScale({ 2, 1 });
	SetMonsterType(MonsterType::StageBoss);
	SetPos({200, 200});
	SetEscapeScale(50.f);
	SetMoveAttackScale(100.f);
	SetNormalAttackCount(2);
	
	for (int i = 0; i < smollArrowCount; i++)
		smollArrow.push_back(new SpriteObj());
	
	for (auto it : smollArrow)
	{
		it->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Arrow.png"));
		it->SetHitBox((FloatRect)it->GetTextureRect());
		it->SetScale({1.5, 1.5});
		it->SetOrigin(Origins::MC);
	}


	spawn->SetScale({ 1.5, 1.5 });
	spawn->SetPos(GetPos());
	SetCardColor(1);
}

void HeavyBombingArcher::Update(float dt)
{
	if (attackDelay <= 0.f)
		count = 0;

	if (InputMgr::GetKeyDown(Keyboard::Key::J))
		SetCurHp(0);

	Archer::Update(dt);
}

void HeavyBombingArcher::Draw(RenderWindow& window)
{
	if (curState == States::MoveAttack)
	{
		for (auto it : smollArrow)
			it->Draw(window);

		arrowDir.setScale({ 1, 1 });
	}
	else
		arrowDir.setScale({ 2, 1 });

	Archer::Draw(window);
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
			}
		}
		else if (attackDelay <= attackStart)
		{
			if (pattern == Pattern::MovingAttack || pattern == Pattern::EscapeAttack)
			{
				for (int i = smollArrow.size() - 1; i > count; i--)
					smollArrow[i]->SetPos(GetPos());

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
			}

			if (smollArrowDelay <= 0.f && count < 4)
			{
				smollArrowDelay = 0.1f;
				count++;
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