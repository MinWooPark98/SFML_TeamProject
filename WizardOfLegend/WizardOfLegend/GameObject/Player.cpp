#include "Player.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "Skill.h"
#include "../Framework/Framework.h"
#include "../Scene/SceneMgr.h"

Player::Player()
	:currState(States::None), isBackHand(false), animator(nullptr), paletteIdx(64), paletteSize(64), attackDmg(20.f),
	walkingSpeed(200.f), runningSpeed(300.f), accelTime(2.f), accelTimer(0.f), dashDuration(0.25f), dashTimer(0.f), jumpDuration(0.75f), jumpTimer(0.f), jumpDistance(0.f), jumpOriginY(0.f), currSkill(nullptr), skillToolMode(false)
{
}

Player::~Player()
{
}

void Player::SetState(States state)
{
	if (state != States::NormalSpell && currState == state)
		return;
	switch (state)
	{
	case States::Idle:
		{
			auto angle = Utils::Angle(lastDir);
			if (angle > -135.f && angle < -45.f)
				animator->Play("IdleUp");
			else if (angle > 45.f && angle < 135.f)
				animator->Play("IdleDown");
			else if (angle >= -45.f && angle <= 45.f)
				animator->Play("IdleRight");
			else
				animator->Play("IdleLeft");
		}
		break;
	case States::Run:
		accelTimer = 0.f;
		if (Utils::EqualFloat(direction.x, 0.f))
			direction.y > 0.f ? animator->Play("RunDown") : animator->Play("RunUp");
		else
			direction.x > 0.f ? animator->Play("RunRight") : animator->Play("RunLeft");
		break;
	case States::Dash:
		{
			auto angle = Utils::Angle(lastDir);
			if (angle > -135.f && angle < -45.f)
				animator->Play("DashUp");
			else if (angle > 45.f && angle < 135.f)
				animator->Play("DashDown");
			else if (angle >= -45.f && angle <= 45.f)
				animator->Play("DashRight");
			else
				animator->Play("DashLeft");
		}
		break;
	case States::Slide:
		if (Utils::EqualFloat(lastDir.x, 0.f))
			lastDir.y > 0.f ? animator->Play("SlideDown") : animator->Play("SlideUp");
		else
			lastDir.x > 0.f ? animator->Play("SlideRight") : animator->Play("SlideLeft");
		break;
	case States::NormalSpell:
		{
			auto angle = Utils::Angle(direction);
			if (angle > -135.f && angle <= -45.f)
				isBackHand ? animator->Play("BackHandUp") : animator->Play("ForeHandUp");
			else if (angle > 45.f && angle <= 135.f)
				isBackHand ? animator->Play("BackHandDown") : animator->Play("ForeHandDown");
			else if (angle > -45.f && angle <= 45.f)
				isBackHand ? animator->Play("BackHandRight") : animator->Play("ForeHandRight");
			else
				isBackHand ? animator->Play("BackHandLeft") : animator->Play("ForeHandLeft");
			isBackHand = !isBackHand;
		}
		break;
	case States::PBAoE:
		{
			auto angle = Utils::Angle(direction);
			if (angle > -135.f && angle <= -45.f)
				animator->Play("PBAoEUp");
			else if (angle > 45.f && angle <= 135.f)
				animator->Play("PBAoEDown");
			else if (angle > -45.f && angle <= 45.f)
				animator->Play("PBAoERight");
			else
				animator->Play("PBAoELeft");
		}
		break;
	case States::JumpSlash:
		{
			auto angle = Utils::Angle(lastDir);
			if (angle > -135.f && angle <= -45.f)
				animator->Play("JumpUp");
			else if (angle > 45.f && angle <= 135.f)
				animator->Play("JumpDown");
			else if (angle > -45.f && angle <= 45.f)
				Utils::EqualFloat(jumpDistance, 0.f) ? animator->Play("JumpRight") : animator->Play("JumpRightAlt");
			else
				Utils::EqualFloat(jumpDistance, 0.f) ? animator->Play("JumpLeft") : animator->Play("JumpLeftAlt");
		}
		break;
	case States::GroundSlam:
		if (currState == States::JumpSlash)
			lastDir.y >= 0.f ? animator->Play("JumpSlamDown") : animator->Play("JumpSlamUp");
		else
			lastDir.y >= 0.f ? animator->Play("GroundSlamDown") : animator->Play("GroundSlamUp");
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
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("JumpRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("JumpRightAlt"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("JumpLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("JumpLeftAlt"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("JumpDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("JumpUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FallRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FallLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FallDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("FallUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("GroundSlamDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("GroundSlamUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("JumpSlamDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("JumpSlamUp"));
	{
		vector<string> clipIds = { "SlideRight", "SlideLeft", "SlideDown", "SlideUp", "BackHandRight", "BackHandLeft", "BackHandDown", "BackHandUp", "ForeHandRight", "ForeHandLeft", "ForeHandDown", "ForeHandUp", "PBAoERight", "PBAoELeft", "PBAoEDown", "PBAoEUp", "GroundSlamDown", "GroundSlamUp", "JumpSlamDown", "JumpSlamUp" };
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
	case Player::States::JumpSlash:
		UpdateJumpSlash(dt);
		break;
	default:
		break;
	}

	if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
		lastDir = direction;
	/*else if (!Utils::EqualFloat(lastDir.x, 0.f))
		lastDir = Utils::Normalize({ lastDir.x, 0.f });*/

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
	else if(Utils::EqualFloat(lastDir.x, 0.f))
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

void Player::UpdateJumpSlash(float dt)
{
	Vector2f moving = direction * jumpDistance * dt / jumpDuration;
	float distance = Utils::Magnitude(moving);
	if (jumpTimer < jumpDuration * 0.5f && jumpTimer + dt >= jumpDuration * 0.5f)
	{
		auto angle = Utils::Angle(lastDir);
		if (angle > -135.f && angle <= -45.f)
			animator->Play("FallUp");
		else if (angle > -45.f && angle <= 45.f)
			animator->Play("FallRight");
		else if (angle > 45.f && angle <= 135.f)
			animator->Play("FallDown");
		else
			animator->Play("FallLeft");
	}
	Translate(moving);
	jumpTimer += dt;
	auto jumpHeightRatio = (jumpDuration * 0.5f - fabs(jumpTimer - jumpDuration * 0.5f)) / (jumpDuration * 0.5f);
	jumpOriginY = 80.f * jumpHeightRatio;
	auto& currOrigin = animator->GetFrame().origin;
	sprite.setOrigin({ currOrigin.x, currOrigin.y + jumpOriginY});
	if (jumpTimer >= jumpDuration)
	{
		jumpTimer = 0.f;
		SetState(States::GroundSlam);
	}
}

void Player::Action()
{
	SkillAction action = currSkill->GetSetting()->playerAction;
	if (action != SkillAction::Dash)
	{
		auto& mousePos = SCENE_MGR->GetCurrentScene()->GetObjMousePos();
		direction = Utils::Normalize(mousePos - position);
		if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
			lastDir = direction;
		if (action == SkillAction::JumpSlash)
		{
			auto mouseDistance = Utils::Distance(mousePos, position);
			if (mouseDistance >= currSkill->GetSetting()->distance * 0.5f)
				jumpDistance = currSkill->GetSetting()->distance;
			else
			{
				jumpDistance = 0.f;
				direction = { 0.f, 0.f };
			}
		}
	}
	switch (action)
	{
	case SkillAction::NormalSpell:
		SetState(States::NormalSpell);
		break;
	case SkillAction::PBAoE:
		SetState(States::PBAoE);
		break;
	case SkillAction::JumpSlash:
		SetState(States::JumpSlash);
		break;
	case SkillAction::GroundSlam:
		SetState(States::GroundSlam);
		break;
	default:
		break;
	}
}
