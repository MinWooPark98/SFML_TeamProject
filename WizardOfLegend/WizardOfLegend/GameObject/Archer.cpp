#include "Archer.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "Player.h"

Archer::Archer()
	: curState(States::None), lastDir(1.f, 0.f), bowDir(0, 0)
{
}

void Archer::Init()
{
	animation.SetTarget(&sprite);
	position = { 0, 0 };

	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightRun"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherLeftIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("ArcherRightIdle"));


	bow = new SpriteObj();
	bow->SetTexture(*RESOURCE_MGR->GetTexture("graphics/ArcherAttackRelease3.png"));
	bow->SetOrigin(Origins::MC);
	bowAnimation.SetTarget(&bow->GetSprite());
	bowAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("BowPull"));
	bowAnimation.AddClip(*RESOURCE_MGR->GetAnimationClip("BowShoot"));

	SetState(States::RightIdle);


	arrow = new SpriteObj();
	arrow->SetTexture(*RESOURCE_MGR->GetTexture("graphics/Arrow.png"));
	arrow->SetHitBox((FloatRect)arrow->GetTextureRect());
	arrow->SetOrigin(Origins::MC);
	SetArrowSpeed(400.f);
	arrowDir.setFillColor(Color::Red);
	arrowDir.setSize({2, 1080});


	shader.loadFromFile("shaders/palette.frag", Shader::Fragment);
	texColorTable.loadFromFile("graphics/ArcherColorIndex.png");
	SetColor(3);

	SetSpeed(200.f);
	SetMoveScale(500.f);
	SetAttackScale(400.f);
	SetAttackStartDelay(1.f);

	SpriteObj::Init();
}

void Archer::Release()
{
	SpriteObj::Release();
}

void Archer::Reset()
{
	SpriteObj::Reset();
}

void Archer::Update(float dt)
{
	SpriteObj::Update(dt);

	if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale() + 1.f && curState != States::Attack)
		Move(dt);

	attackDelay -= dt;

	switch (curState)
	{
	case Archer::States::LeftIdle: case Archer::States::RightIdle:
		UpdateIdle();
		break;
	case Archer::States::LeftMove: case Archer::States::RightMove:
		UpdateMove();
		break;
	case Archer::States::Attack:
		UpdateAttack(dt);
		break;
	case Archer::States::Hit:
		SetState(States::Hit);
		break;
	case Archer::States::Die:
		SetState(States::Die);
		break;
	}

	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		lastDir = direction;
	}

	animation.Update(dt);
	bowAnimation.Update(dt);
}

void Archer::Draw(RenderWindow& window)
{
	if (curState == States::Attack)
	{
		window.draw(bow->GetSprite(), &shader);

		if (attackDelay >= attackStart)
			window.draw(arrowDir);
	}
	
	Object::Draw(window);
	window.draw(sprite, &shader);
	arrow->Draw(window);
}

void Archer::SetState(States newState)
{
	if (curState == newState)
		return;

	curState = newState;

	switch (curState)
	{
	case Archer::States::LeftIdle:
		animation.Play("ArcherLeftIdle");
		break;
	case Archer::States::RightIdle:
		animation.Play("ArcherRightIdle");
		break;
	case Archer::States::LeftMove:
		animation.Play("ArcherLeftRun");
		break;
	case Archer::States::RightMove:
		animation.Play("ArcherRightRun");
		break;
	case Archer::States::Attack:
		if (player->GetPos().x >= GetPos().x)
		{
			attackDelay = 2.f;
			return;
		}
		if (player->GetPos().x < GetPos().x)
		{
			attackDelay = 2.f;
			return;
		}
		break;
	case Archer::States::Hit:
		lastDir.x < 0.f ? animation.Play("ArcherLeftHit") : animation.Play("ArcherRightHit");
		break;
	case Archer::States::Die:
		lastDir.x < 0.f ? animation.Play("ArcherLeftDie") : animation.Play("ArcherRightDie");
		break;
	}
}

void Archer::Move(float dt)
{
	if (Utils::Distance(player->GetPos(), GetPos()) <= GetMoveScale())
	{
		player->GetPos().x > GetPos().x ? direction.x = 1 : direction.x = -1;
		player->GetPos().y > GetPos().y ? direction.y = 1 : direction.y = -1;
	}
	else
		direction.x = 0;

	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		auto move = Utils::Normalize(player->GetPos() - GetPos());
		Translate({ dt * speed * move });

		if (lastDir.x < 0.f)
			SetState(States::LeftMove);
		if (lastDir.x > 0.f)
			SetState(States::RightMove);
	}

	if (Utils::EqualFloat(direction.x, 0.f))
	{
		if (lastDir.x < 0.f)
			SetState(States::LeftIdle);
		if (lastDir.x > 0.f)
			SetState(States::RightIdle);
	}
}

void Archer::SetColor(int index)
{
	paletteIndex = (paletteIndex - index) % paletteSize;
	shader.setUniform("colorTable", texColorTable);
	shader.setUniform("paletteIndex", (float)paletteIndex / paletteSize);
}

void Archer::UpdateIdle()
{
	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		if (lastDir.x > 0.f)
			SetState(States::LeftMove);
		if (lastDir.x < 0.f)
			SetState(States::RightMove);
		return;
	}
}

void Archer::UpdateMove()
{
	if (Utils::Distance(player->GetPos(), GetPos()) <= GetAttackScale())
	{
		SetState(States::Attack);
		attackDelay = 2.f;
	}
}

void Archer::UpdateAttack(float dt)
{
	if (attackDelay >= attackStart)
	{
		bow->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()));
		arrow->GetSprite().setRotation(Utils::Angle(GetPos(), player->GetPos()) + 90);
		arrowDir.setRotation(Utils::Angle(GetPos(), player->GetPos()) - 90);
		arrow->SetPos(GetPos());
		bow->SetPos(GetPos());
		arrowDir.setPosition(GetPos());

		if (player->GetPos().x >= GetPos().x)
			animation.Play("ArcherRightAttack");
		if (player->GetPos().x < GetPos().x)
			animation.Play("ArcherLeftAttack");

		playerLastPos = player->GetPos();
	}
	else
	{
		auto shot = Utils::Normalize(playerLastPos - arrow->GetPos());
		arrow->Translate({ dt * arrowSpeed * shot * 2.f});
	}

	if (attackDelay <= attackStart && bowWait)
	{
		bowAnimation.Play("BowShoot");
		bowWait = false;
	}
	else if (attackDelay >= attackStart && !bowWait)
	{
		bowAnimation.Play("BowPull");
		bowWait = true;
	}


	if (attackDelay <= 0.f)
	{
		SetState(States::None);
		SetState(States::Attack);

		if (Utils::Distance(player->GetPos(), GetPos()) > GetAttackScale() &&
			Utils::Distance(player->GetPos(), GetPos()) < GetMoveScale())
		{
			if (lastDir.x < 0.f)
				SetState(States::LeftMove);
			if (lastDir.x > 0.f)
				SetState(States::RightMove);
		}

		if (Utils::Distance(player->GetPos(), GetPos()) >= GetMoveScale())
		{
			direction.x = 0;

			if (lastDir.x < 0.f)
				SetState(States::LeftIdle);
			if (lastDir.x > 0.f)
				SetState(States::RightIdle);

			return;
		}
	}
}

void Archer::SetPlayer(Player* player)
{
	this->player = player;
}