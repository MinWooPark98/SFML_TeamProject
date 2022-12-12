#include "Interactive.h"
#include "../../Framework/Animator.h"
#include "../Player.h"
#include "../../Framework/InputMgr.h"
#include "../../Framework/ResourceMgr.h"

Interactive::Interactive()
	:animator(nullptr), player(nullptr), approached(false), interactKey(nullptr)
{
}

Interactive::~Interactive()
{
}

void Interactive::Init()
{
	SpriteObj::Init();
	interactKey = new SpriteObj();
	interactKey->Init();
	interactKey->SetTexture(*RESOURCE_MGR->GetTexture("graphics/F.png"));
	interactKey->SetScale({ 0.6f, 0.6f });
	interactKey->SetOrigin(Origins::MC);
	interactKey->SetActive(false);
}

void Interactive::Update(float dt)
{
	SpriteObj::Update(dt);
	if (animator != nullptr)
		animator->Update(dt);
	if (player != nullptr)
	{
		if (!approached)
		{
			if (GetHitBounds().intersects(player->GetHitBounds()))
			{
				approached = true;
				Approach();
			}
		}
		else
		{
			if (!GetHitBounds().intersects(player->GetHitBounds()))
			{
				approached = false;
				ApproachEnd();
				return;
			}

			auto stackedOrder = InputMgr::GetStackedOrder();
			if (stackedOrder.empty() && InputMgr::GetKeyDown(Keyboard::F) && Interact != nullptr)
				Interact();
		}
	}
}

void Interactive::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
	if (interactKey != nullptr && interactKey->GetActive())
		interactKey->Draw(window);
}

void Interactive::SetPos(const Vector2f& pos)
{
	SpriteObj::SetPos(pos);
	if (interactKey != nullptr)
		interactKey->SetPos(pos + interactKeyPos);
}

void Interactive::Approach()
{
	DefaultApproach();
	if (ApproachFunc != nullptr)
		ApproachFunc();
}

void Interactive::ApproachEnd()
{
	DefaultApproachEnd();
	if (ApproachEndFunc != nullptr)
		ApproachEndFunc();
}

void Interactive::DefaultApproach()
{
	interactKey->SetActive(true);
}

void Interactive::DefaultApproachEnd()
{
	interactKey->SetActive(false);
}
