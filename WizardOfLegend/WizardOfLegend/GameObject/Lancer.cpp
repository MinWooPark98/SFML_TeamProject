#include "Lancer.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"

Lancer::Lancer()
	: curState(States::None), speed(300.f), lastDir(1.f, 0.f), SpireDir(0, 0), curHp(100)
{
}

void Lancer::Init()
{
	animation.SetTarget(&sprite);
	position = { 0, 0 };
	
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftMove"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightMove"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightDie"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerDownAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerUpAttack"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightHit"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerLeftIdle"));
	animation.AddClip(*RESOURCE_MGR->GetAnimationClip("LancerRightIdle"));


	spear = new SpriteObj();
	spear->SetTexture(*RESOURCE_MGR->GetTexture("graphics/LancerSpearWithArm.png"));
	spear->SetOrigin(Origins::MC);
	spear->SetScale({3, 3});

	SetScale({ 3, 3 });
	SetState(States::RightIdle);

	shader.loadFromFile("shaders/palette.frag", Shader::Fragment);
	texColorTable.loadFromFile("graphics/LancerColorIndex.png");
	SetColor(3);


	// temp player
	player = new Object();
	player->SetHitBox(FloatRect(0, 0, 50, 50));
	playerRec.setFillColor(Color::White);
	playerRec.setSize({50, 50});
	player->SetPos({1920 / 2, 1080 / 2});

	this->SetHitBox(this->GetGlobalBounds());
	this->SetHitBoxOrigin(Origins::MC);

	attackScale.setFillColor(Color::Red);
	attackScale.setRadius(this->GetSize().x * 3);
	attackScale.setOrigin(this->GetSize() * 3.f);
	
	SpriteObj::Init();
}

void Lancer::Release()
{
	SpriteObj::Release();
}

void Lancer::Reset()
{
	SpriteObj::Reset();
}

void Lancer::Update(float dt)
{
	SpriteObj::Update(dt);

	DevPlayerMove(dt);
	attackScale.setPosition(this->GetPos());

	if (Utils::Distance(player->GetPos(), GetPos()) <= 700.f && curState != States::Attack)
		Move(dt, player);

	attackDelay -= dt;

	switch (curState)
	{
	case Lancer::States::LeftIdle: case Lancer::States::RightIdle:
		UpdateIdle();
		break;
	case Lancer::States::LeftMove: case Lancer::States::RightMove:
		UpdateMove();
		break;
	case Lancer::States::Attack:
		UpdateAttack();
		break;
	case Lancer::States::Hit:
		SetState(States::Hit);
		break;
	case Lancer::States::Die:
		SetState(States::Die);
		break;
	}

	if (!Utils::EqualFloat(direction.x, 0.f))
	{
		lastDir = direction;
	}

	player->Update(dt);
	animation.Update(dt);
}

void Lancer::Draw(RenderWindow& window)
{
	window.draw(attackScale);
	window.draw(playerRec);
	player->Draw(window);
	Object::Draw(window);
	window.draw(sprite, &shader);
	spear->Draw(window);
}

void Lancer::Die()
{
	this->SetActive(false);
}

void Lancer::SetState(States newState)
{
	if (curState == newState)
		return;

	curState = newState;
	
	switch (curState)
	{
	case Lancer::States::LeftIdle:
		animation.Play("LancerLeftIdle");
		break;
	case Lancer::States::RightIdle:
		animation.Play("LancerRightIdle");
		break;
	case Lancer::States::LeftMove:
		animation.Play("LancerLeftMove");
		break;
	case Lancer::States::RightMove:
		animation.Play("LancerRightMove");
		break;
	case Lancer::States::Attack:
		if (Utils::Angle(player->GetPos(), GetPos()) >= -180 &&
			Utils::Angle(player->GetPos(), GetPos()) <= -130 ||
			Utils::Angle(player->GetPos(), GetPos()) >= 130 &&
			Utils::Angle(player->GetPos(), GetPos()) <= 180)
		{
			animation.Play("LancerRightAttack");
			attackDelay = 2.f;
			return;
		}
		if (Utils::Angle(player->GetPos(), GetPos()) <= -45 &&
			Utils::Angle(player->GetPos(), GetPos()) >= -129)
		{
			animation.Play("LancerDownAttack");
			attackDelay = 2.f;
			return;
		}
		if (Utils::Angle(player->GetPos(), GetPos()) >= -44 &&
			Utils::Angle(player->GetPos(), GetPos()) <= 45)
		{
			animation.Play("LancerLeftAttack");
			attackDelay = 2.f;
			return;
		}
		if (Utils::Angle(player->GetPos(), GetPos()) >= 46 &&
			Utils::Angle(player->GetPos(), GetPos()) <= 129)
		{
			animation.Play("LancerUpAttack");
			attackDelay = 2.f;
			return;
		}
		break;
	case Lancer::States::Hit:
		lastDir.x < 0.f ? animation.Play("LancerLeftHit") : animation.Play("LancerRightHit");
		break;
	case Lancer::States::Die:
		lastDir.x < 0.f ? animation.Play("LancerLeftDie") : animation.Play("LancerRightDie");
		break;
	}
}

void Lancer::Move(float dt, Object* player)
{
	if (Utils::EqualFloat(direction.x, 0.f))
	{
		player->GetPos().x > GetPos().x ? direction.x = 1 : direction.x = -1;
		player->GetPos().y > GetPos().y ? direction.y = 1 : direction.y = -1;

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

void Lancer::SetColor(int index)
{
	paletteIndex = (paletteIndex - index) % paletteSize;
	shader.setUniform("colorTable", texColorTable);
	shader.setUniform("paletteIndex", (float)paletteIndex / paletteSize);
}

void Lancer::DevPlayerMove(float dt)
{
	if (InputMgr::GetKeyDown(Keyboard::Left))
	{
		playerDir.x = -1;
		playerDir.y = 0;
		playerSpeed = 500.f;
	}
	else if (InputMgr::GetKeyDown(Keyboard::Right))
	{
		playerDir.x = 1;
		playerDir.y = 0;
		playerSpeed = 500.f;
	}
	else
		direction.x = 0;

	if (InputMgr::GetKeyDown(Keyboard::Up))
	{
		playerDir.y = -1;
		playerDir.x = 0;
		playerSpeed = 500.f;
	}
	if (InputMgr::GetKeyDown(Keyboard::Down))
	{
		playerDir.y = 1;
		playerDir.x = 0;
		playerSpeed = 500.f;
	}

	if (InputMgr::GetKeyUp(Keyboard::Left) ||
		InputMgr::GetKeyUp(Keyboard::Right) ||
		InputMgr::GetKeyUp(Keyboard::Up) ||
		InputMgr::GetKeyUp(Keyboard::Down))
	{
		playerSpeed = 0.f;
	}

	player->SetDirection(playerDir);
	player->Translate({ dt * playerSpeed * playerDir.x, dt * playerSpeed * playerDir.y });
	playerRec.setPosition(player->GetPos());
}

void Lancer::UpdateIdle()
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

void Lancer::UpdateMove()
{
	if (Utils::Distance(player->GetPos(), GetPos()) <= 200.f)
	{
		SetState(States::Attack);
		attackDelay = 2.f;
	}

	if (Utils::EqualFloat(direction.x, 0.f))
	{
		if (lastDir.x < 0.f)
			SetState(States::LeftIdle);
		if (lastDir.x > 0.f)
			SetState(States::RightIdle);
		return;
	}

	if (!Utils::EqualFloat(direction.x, lastDir.x))
	{
		if (lastDir.x < 0.f)
			SetState(States::LeftMove);
		if (lastDir.x > 0.f)
			SetState(States::RightMove);
	}
}
void Lancer::UpdateAttack()
{
	if (attackDelay <= 0.f)
	{
		SetState(States::None);
		SetState(States::Attack);
		if (Utils::Distance(player->GetPos(), GetPos()) >= 200.f)
		{
			if (lastDir.x < 0.f)
				SetState(States::LeftMove);
			if (lastDir.x > 0.f)
				SetState(States::RightMove);
		}
	}
}

void Lancer::SpearSet(float dt, Object* player)
{
	spear->SetPos(GetPos());

	if (spearActive)
		spear->SetActive(true);
	else
		spear->SetActive(false);
}