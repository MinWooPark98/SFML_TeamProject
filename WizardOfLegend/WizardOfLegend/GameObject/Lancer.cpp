#include "Lancer.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"

Lancer::Lancer()
	: curState(States::None), speed(0.f), lastDir(1.f, 0.f), SpireDir(0, 0), curHp(100)
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

	SetPos({ 500.f, 500.f });
	SetScale({ 3, 3 });
	SetState(States::Idle);

	shader.loadFromFile("shaders/palette.frag", Shader::Fragment);
	texColorTable.loadFromFile("graphics/LancerColorIndex.png");
	SetColor(3);
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
	Move(dt);
	
	if (curHp > 0)
	{
		if (InputMgr::GetKeyDown(Keyboard::Key::G))
		{
			SetState(States::Idle);
		}
		if (InputMgr::GetKeyDown(Keyboard::Key::A))
		{
			SetState(States::Attack);
		}
		if (InputMgr::GetKeyDown(Keyboard::Key::S))
		{
			SetState(States::Move);
		}
		if (InputMgr::GetKeyDown(Keyboard::Key::F))
		{
			SetState(States::Hit);
		}
		if (InputMgr::GetKeyDown(Keyboard::Key::D))
		{
			SetState(States::Die);
		}
	}
	animation.Update(dt);
}

void Lancer::Draw(RenderWindow& window)
{
	Object::Draw(window);
	window.draw(sprite, &shader);
}

void Lancer::Die()
{
	this->SetActive(false);
}

void Lancer::SetState(States newState) // 플레이어 매개변수로 받기, 플레이어 위치에 따라서 공격 방향 달라짐
{
	if (curState == newState)
		return;

	curState = newState;
	
	switch (curState)
	{
	case Lancer::States::Idle:
		lastDir.x < 0.f ? animation.Play("LancerLeftIdle") : animation.Play("LancerRightIdle");
		break;
	case Lancer::States::Move:
		lastDir.x < 0.f ? animation.Play("LancerLeftMove") : animation.Play("LancerRightMove");
		break;
	case Lancer::States::Attack:
		// 플레이어 벡터, 렌서 벡터 비교해서 공격 위치 잡기
		switch (attackPos)
		{
		case 1:
			animation.Play("LancerLeftAttack");
			break;
		case 2:
			animation.Play("LancerRightAttack");
			break;
		case 3:
			animation.Play("LancerUpAttack");
			break;
		case 4:
			animation.Play("LancerDownAttack");
			break;
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

void Lancer::Move(float dt)
{
	//vector2f pos = player.GetPos();
	//movedir = player.dir - GetDirection();
	//SetPos(movedir * dt * speed);


	if (InputMgr::GetKeyDown(Keyboard::Key::Left))
	{
		attackPos = 1;
		lastDir.x = -1;
		lastDir.y = 0;
		SetState(States::None);
		SetState(States::Move);
	}
	else if (InputMgr::GetKeyDown(Keyboard::Key::Right))
	{
		attackPos = 2;
		lastDir.x = 1;
		lastDir.y = 0;
		SetState(States::None);
		SetState(States::Move);
	}

	else if (InputMgr::GetKeyDown(Keyboard::Key::Up))
	{
		attackPos = 3;
		//lastDir.x = 0;
		lastDir.y = -1;
		SetState(States::None);
		SetState(States::Move);
	}
	else if (InputMgr::GetKeyDown(Keyboard::Key::Down))
	{
		attackPos = 4;
		//lastDir.x = 0;
		lastDir.y = 1;
		SetState(States::None);
		SetState(States::Move);
	}
	else
		speed = 100.f;

	Translate({ dt * speed * lastDir.x, dt * speed * lastDir.y });
}

void Lancer::SpearPos(const Vector2f& lancerPos)
{
	spear->SetPos(lancerPos);
}

void Lancer::SetColor(int index)
{
	paletteIndex = (paletteIndex - index) % paletteSize;
	shader.setUniform("colorTable", texColorTable);
	shader.setUniform("paletteIndex", (float)paletteIndex / paletteSize);
}