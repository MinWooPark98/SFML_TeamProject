#include "Player.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "Skill.h"
#include "../Framework/Framework.h"
#include "../Scene/SceneMgr.h"

Player::Player()
	:currState(States::None), isBackHand(false), animator(nullptr), paletteIdx(64), paletteSize(64), attackDmg(20.f),
	walkingSpeed(200.f), runningSpeed(300.f), accelTime(2.f), accelTimer(0.f), dashDuration(0.2f), dashTimer(0.f), currSkill(nullptr), skillToolMode(false)
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
		{
			auto angle = Utils::Angle(lastDir);
			if (angle > -135.f && angle < -45.f)
				animator->Play("IdleUp");
			else if (angle >= -45.f && angle <= 45.f)
				animator->Play("IdleRight");
			else if (angle > 45.f && angle < 135.f)
				animator->Play("IdleDown");
			else
				animator->Play("IdleLeft");
		}
		break;
		/*if (Utils::EqualFloat(lastDir.x, 0.f))
			lastDir.y > 0.f ? animator->Play("IdleDown") : animator->Play("IdleUp");
		else
			lastDir.x > 0.f ? animator->Play("IdleRight") : animator->Play("IdleLeft"); 
		break;*/
	case Player::States::Run:
		accelTimer = 0.f;
		if (Utils::EqualFloat(direction.x, 0.f))
			direction.y > 0.f ? animator->Play("RunDown") : animator->Play("RunUp");
		else
			direction.x > 0.f ? animator->Play("RunRight") : animator->Play("RunLeft");
		break;
	case Player::States::Dash:
		{
			auto angle = Utils::Angle(lastDir);
			if (angle > -135.f && angle < -45.f)
				animator->Play("DashUp");
			else if (angle >= -45.f && angle <= 45.f)
				animator->Play("DashRight");
			else if (angle > 45.f && angle < 135.f)
				animator->Play("DashDown");
			else
				animator->Play("DashLeft");
		}
		break;
	case Player::States::Slide:
		if (Utils::EqualFloat(lastDir.x, 0.f))
			lastDir.y > 0.f ? animator->Play("SlideDown") : animator->Play("SlideUp");
		else
			lastDir.x > 0.f ? animator->Play("SlideRight") : animator->Play("SlideLeft");
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
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("DashDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("DashRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("DashLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("DashUp"));
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
	{
		vector<string> clipIds = { "BackHandRight", "BackHandLeft", "BackHandDown", "BackHandUp", "ForeHandRight", "ForeHandLeft", "ForeHandDown", "ForeHandUp" };
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
	for (int i = 0; i < 6; ++i)
	{
		Skill* newSkill = new Skill();
		newSkill->SetSubject(this, Skill::SubjectType::Player);
		skills.push_back(newSkill);
	}

	playerShader.loadFromFile("shaders/palette.frag", Shader::Fragment);
	playerShader.setUniform("colorTable", *RESOURCE_MGR->GetTexture("graphics/WizardPalette.png"));
	playerShader.setUniform("paletteIndex", (float)paletteIdx / paletteSize);	// index 바꿔주어 색 변경 -> 속성 변경 추후 추가
}

void Player::Update(float dt)
{
	SpriteObj::Update(dt);
	animator->Update(dt);

	auto& windowSize = FRAMEWORK->GetWindowSize();
	
	if (currState == States::Idle || currState == States::Run)
	{
		direction.x = 0.f;
		direction.y = 0.f;
		if (!skillToolMode || InputMgr::GetMousePos().x < windowSize.x * 0.7f)
		{
			direction.x += Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) ? 1 : 0;
			direction.x += Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A) ? -1 : 0;
			direction.y += Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S) ? 1 : 0;
			direction.y += Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) ? -1 : 0;
		}
		direction = Utils::Normalize(direction);

		if (!skillToolMode || InputMgr::GetMousePos().x < windowSize.x * 0.7f)
		{
			for (auto mouseDown : InputMgr::GetMouseDownList())
			{
				switch (mouseDown)
				{
				case Mouse::Left:
					SetCurrSkill(skills[0]);
					skills[0]->Do();
					break;
				case Mouse::Right:
					SetCurrSkill(skills[1]);
					skills[1]->Do();
					break;
				default:
					break;
				}
			}
		}
	}
	if (currState == States::Idle || currState == States::Run)
	{
		if (!skillToolMode || InputMgr::GetMousePos().x < windowSize.x * 0.7f)
		{
			for (auto keyDown : InputMgr::GetKeyDownList())
			{
				switch (keyDown)
				{
				case Keyboard::Space:
					SetCurrSkill(skills[2]);
					skills[2]->Do();
					SetState(States::Dash);
					break;
				case Keyboard::Q:
					SetCurrSkill(skills[3]);
					skills[3]->Do();
					break;
				case Keyboard::E:
					SetCurrSkill(skills[4]);
					skills[4]->Do();
					break;
				case Keyboard::R:
					SetCurrSkill(skills[5]);
					skills[5]->Do();
					break;
				default:
					break;
				}
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
	case Player::States::Dash:
		UpdateDash(dt);
		break;
	case Player::States::Skill:
		UpdateSkill(dt);
		break;
	default:
		break;
	}

	if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
		lastDir = direction;
	else if (!Utils::EqualFloat(lastDir.x, 0.f))
		lastDir = Utils::Normalize({ lastDir.x, 0.f });

	for (auto skill : skills)
	{
		skill->Update(dt);
	}
}

void Player::Draw(RenderWindow& window)
{
	Object::Draw(window);
	window.draw(sprite, &playerShader);
	for (auto skill : skills)
	{
		skill->Draw(window);
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

void Player::UpdateDash(float dt)
{
	dashTimer += dt;
	Translate(lastDir * runningSpeed * dt);
	if (dashTimer >= dashDuration)
	{
		dashTimer = 0.f;
		SetState(States::Slide);
	}
}

void Player::UpdateSkill(float dt)
{

}

void Player::Action()
{
	SkillAction action = currSkill->GetSetting()->playerAction;
	if (action != SkillAction::Dash)
	{
		SetState(States::Skill);
		auto& mousePos = SCENE_MGR->GetCurrentScene()->GetObjMousePos();
		direction = mousePos - position;
		if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
			lastDir = direction;
	}
	switch (currSkill->GetSetting()->playerAction)
	{
	case SkillAction::NormalSpell:
		{
			auto angle = Utils::Angle(direction);
			if (angle > -135.f && angle <= -45.f)
				isBackHand ? animator->Play("BackHandUp") : animator->Play("ForeHandUp");
			else if (angle > -45.f && angle <= 45.f)
				isBackHand ? animator->Play("BackHandRight") : animator->Play("ForeHandRight");
			else if (angle > 45.f && angle <= 135.f)
				isBackHand ? animator->Play("BackHandDown") : animator->Play("ForeHandDown");
			else
				isBackHand ? animator->Play("BackHandLeft") : animator->Play("ForeHandLeft");
			isBackHand = !isBackHand;
		}
		break;
	case SkillAction::PBAoE:
		// 애니메이션 재생
		break;
	case SkillAction::JumpSlash:
		// 애니메이션 재생 및 이동
		break;
	default:
		break;
	}
}
