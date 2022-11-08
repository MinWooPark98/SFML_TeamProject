#include "Player.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "Skill.h"
#include "../Framework/Framework.h"

Player::Player()
	:currState(States::None), isBackHand(false), animator(nullptr), walkingSpeed(200.f), runningSpeed(300.f), accelTime(2.f), accelTimer(0.f)
{
}

Player::~Player()
{
}

void Player::SetState(States state)
{
	if (currState == state)
		return;
	switch (state)
	{
	case Player::States::Idle:
		if (Utils::EqualFloat(lastDir.x, 0.f))
			lastDir.y > 0.f ? animator->Play("IdleDown") : animator->Play("IdleUp");
		else
			lastDir.x > 0.f ? animator->Play("IdleRight") : animator->Play("IdleLeft"); 
		break;
	case Player::States::Run:
		accelTimer = 0.f;
		if (Utils::EqualFloat(direction.x, 0.f))
			direction.y > 0.f ? animator->Play("RunDown") : animator->Play("RunUp");
		else
			direction.x > 0.f ? animator->Play("RunRight") : animator->Play("RunLeft");
		break;
	case Player::States::Slide:
		if (Utils::EqualFloat(lastDir.x, 0.f))
			lastDir.y > 0.f ? animator->Play("SlideDown") : animator->Play("SlideUp");
		else
			lastDir.x > 0.f ? animator->Play("SlideRight") : animator->Play("SlideLeft");
		break;
	case Player::States::Skill:
		/*if (skills[0] == nullptr)
			return;*/
		// �Ϲ����� ��ų ��� �� ĳ���� ��� / ���� �����̳� �ٸ� ���� ���� ��ų ������ �����ؼ� �ش� �ִϸ��̼� �� ���� ����ϵ��� �ؾ� ��
		if (Utils::EqualFloat(lastDir.x, 0.f))
		{
			if(isBackHand)
				lastDir.y > 0.f ? animator->Play("BackHandDown") : animator->Play("BackHandUp");
			else
				lastDir.y > 0.f ? animator->Play("ForeHandDown") : animator->Play("ForeHandUp");
		}
		else
		{
			if (isBackHand)
				lastDir.x > 0.f ? animator->Play("BackHandRight") : animator->Play("BackHandLeft");
			else
				lastDir.x > 0.f ? animator->Play("ForeHandRight") : animator->Play("ForeHandLeft");
		}
		break;
	default:
		break;
	}
	currState = state;
}

void Player::Init()
{
	SpriteObj::Init();
	SetPos(Vector2f(FRAMEWORK->GetWindowSize()) * 0.25f);

	animator = new Animator();
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("IdleDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("IdleRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("IdleLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("IdleUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("BackHandDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("BackHandRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("BackHandLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("BackHandUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("ForeHandDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("ForeHandRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("ForeHandLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("ForeHandUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("PBAoEDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("PBAoERight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("PBAoELeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("PBAoEUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("RunDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("RunRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("RunLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("RunUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SlideRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SlideLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SlideDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("SlideUp"));
	{
		vector<string> clipIds = { "SlideRight", "SlideLeft", "SlideDown", "SlideUp" };
		for (int i = 0; i < clipIds.size(); ++i)
		{
			AnimationEvent ev;
			ev.clipId = clipIds[i];
			ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
			ev.onEvent = bind(&Player::SetState, this, States::Idle);
			animator->AddEvent(ev);
		}
	}
	animator->SetTarget(&sprite);
	SetState(States::Idle);

	lastDir = { 0.f, 1.f };
	skills.assign(6, nullptr);
}

void Player::Update(float dt)
{
	SpriteObj::Update(dt);
	animator->Update(dt);
	direction.x = 0.f;
	direction.y = 0.f;
	direction.x += Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) ? 1 : 0;
	direction.x += Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A) ? -1 : 0;
	direction.y += Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S) ? 1 : 0;
	direction.y += Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) ? -1 : 0;
	direction = Utils::Normalize(direction);
	
	if (currState == States::Idle || currState == States::Run)
	{
		for (auto mouseDown : InputMgr::GetMouseDownList())
		{
			switch (mouseDown)
			{
			case Mouse::Left:
				skills[0]->Do();
				SetState(States::Skill);
				break;
			case Mouse::Right:
				skills[1]->Do();
				SetState(States::Skill);
				break;
			default:
				break;
			}
		}
	}
	if (currState == States::Idle || currState == States::Run)
	{
		for (auto keyDown : InputMgr::GetKeyDownList())
		{
			switch (keyDown)
			{
			case Keyboard::Space:
				skills[2]->Do();
				SetState(States::Skill);
				break;
			case Keyboard::Q:
				skills[3]->Do();
				SetState(States::Skill);
				break;
			case Keyboard::E:
				skills[4]->Do();
				SetState(States::Skill);
				break;
			case Keyboard::F:
				skills[5]->Do();
				SetState(States::Skill);
				break;
			default:
				break;
			}
		}
	}

	switch (currState)
	{
	case Player::States::Idle:
		UpdateIdle(dt);
		break;
	case Player::States::Run:
		UpdateRun(dt);
		break;
	case Player::States::Skill:
		UpdateSkill(dt);
		break;
	default:
		break;
	}

	if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
		lastDir = direction;

	for (auto skill : skills)
	{
		// ���� �߰�
		if (skill != nullptr)
		{
			// ��ų ������Ʈ -> ����ü �߻� �� player ���� ���ƿ͵� ��ų�� ������Ʈ �� �� �ֵ���
		}
	}
}

void Player::UpdateIdle(float dt)
{
	if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
		SetState(States::Run);
}

void Player::UpdateRun(float dt)
{
	accelTimer += dt;
	if (Utils::EqualFloat(direction.x, 0.f) && Utils::EqualFloat(direction.y, 0.f))
	{
		if (accelTimer >= accelTime)
			SetState(States::Slide);
		else
			SetState(States::Idle);
		return;
	}

	if (accelTimer >= accelTime)
		Translate(direction * runningSpeed * dt);
	else
		Translate(direction * walkingSpeed * dt);

	if (Utils::EqualFloat(direction.x, lastDir.x) && Utils::EqualFloat(direction.y, lastDir.y))
		return;

	if (Utils::EqualFloat(direction.x, 0.f))
		direction.y > 0.f ? animator->Play("RunDown") : animator->Play("RunUp");
	else
		direction.x > 0.f ? animator->Play("RunRight") : animator->Play("RunLeft");
}

void Player::UpdateSkill(float dt)
{
	// skill ��� ���� �� Ȯ�� �� SetState(States::Idle);
}
