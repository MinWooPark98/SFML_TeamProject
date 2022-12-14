#include "Player.h"
#include "../Framework/Animator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Framework.h"
#include "../Scene/SceneMgr.h"
#include "SkillSet.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/StatTable.h"
#include "../Ui/PlayUiMgr.h"
#include "../Scene/PlayScene.h"
#include "../Ui/ShowDamage.h"
#include "../Framework/CameraMove.h"
#include "../DataTable/PlatinumTable.h"
#include "../GameObject/HitSpark.h"
#include "Item/ItemMgr.h"

Player::Player()
	:currState(States::None), isBackHand(false), animator(nullptr), attackDmg(20),
	walkingSpeed(0.f), runningSpeed(0.f), accelTime(2.f), accelTimer(0.f), dashDuration(0.35f), dashTimer(0.f), jumpDuration(0.5f), jumpTimer(0.f), jumpDistance(0.f), jumpOriginY(0.f), lastDir(1.f, 0.f), dashDir(1.f, 0.f), currSkillSet(nullptr), skillToolMode(false), maxHp(525), curHp(525), hitDuration(0.2f), hitTimer(0.f), damageTake(0.f), evasionRate(0.f), criticalRate(0.f), criticalRatio(0.f), fallDuration(1.f), fallTimer(0.f), fallingScale({ 1.f, 1.f }), itemMgr(nullptr)
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
		currSkillSet = nullptr;
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
			auto angle = Utils::Angle(dashDir);
			if (angle > -135.f && angle < -45.f)
				animator->Play("DashUp");
			else if (angle > 45.f && angle < 135.f)
				animator->Play("DashDown");
			else if (angle >= -45.f && angle <= 45.f)
				animator->Play("DashRight");
			else
				animator->Play("DashLeft");
		}
		SOUND_MGR->Play("sounds/AirJet.wav");
		break;
	case States::Slide:
		if (Utils::EqualFloat(dashDir.x, 0.f))
			dashDir.y > 0.f ? animator->Play("SlideDown") : animator->Play("SlideUp");
		else
			dashDir.x > 0.f ? animator->Play("SlideRight") : animator->Play("SlideLeft");

		isRunning = true;
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
	case States::Jump:
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
		if (currState == States::Jump)
			lastDir.y >= 0.f ? animator->Play("JumpSlamDown") : animator->Play("JumpSlamUp");
		else
			lastDir.y >= 0.f ? animator->Play("GroundSlamDown") : animator->Play("GroundSlamUp");
		break;
	case States::GroundSlamEnd:
			lastDir.y >= 0.f ? animator->Play("GroundSlamDownEnd") : animator->Play("GroundSlamUpEnd");
		break;
	case States::Hit:
		{
			auto angle = Utils::Angle(lastDir);
			if (angle > -135.f && angle <= -45.f)
				animator->Play("HurtUp");
			else if (angle > 45.f && angle <= 135.f)
				animator->Play("HurtDown");
			else if (angle > -45.f && angle <= 45.f)
				animator->Play("HurtRight");
			else
				animator->Play("HurtLeft");
		}
		break;
	case States::Fall:
		{
			auto angle = Utils::Angle(lastDir);
			if (angle > -135.f && angle <= -45.f)
				animator->Play("HurtUp");
			else if (angle > 45.f && angle <= 135.f)
				animator->Play("HurtDown");
			else if (angle > -45.f && angle <= 45.f)
				animator->Play("HurtRight");
			else
				animator->Play("HurtLeft");
		}
		SOUND_MGR->Play("sounds/Fall.wav");
		break;
	case States::Die:
		animator->Play("Die");
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
	animator->SetSpeed(1.5f);
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
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("GroundSlamDownEnd"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("GroundSlamUpEnd"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("HurtRight"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("HurtLeft"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("HurtDown"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("HurtUp"));
	animator->AddClip(*RESOURCE_MGR->GetAnimationClip("Die"));
	{
		vector<string> clipIds = { "SlideRight", "SlideLeft", "SlideDown", "SlideUp", "GroundSlamDownEnd", "GroundSlamUpEnd" };
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
		vector<string> clipIds = { "BackHandRight", "BackHandLeft", "BackHandDown", "BackHandUp", "ForeHandRight", "ForeHandLeft", "ForeHandDown", "ForeHandUp", "PBAoERight", "PBAoELeft", "PBAoEDown", "PBAoEUp", "GroundSlamDown", "GroundSlamUp", "JumpSlamDown", "JumpSlamUp" };
		for (int i = 0; i < clipIds.size(); ++i)
		{
			AnimationEvent ev;
			ev.clipId = clipIds[i];
			ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
			ev.onEvent = bind(&Player::FinishAction, this);
			animator->AddEvent(ev);
		}
	}
	{
		AnimationEvent ev;
		ev.clipId = "Die";
		ev.frame = RESOURCE_MGR->GetAnimationClip(ev.clipId)->GetFrameCount() - 1;
		ev.onEvent = bind(&SceneMgr::ChangeScene, SCENE_MGR, Scenes::Title);
		animator->AddEvent(ev);
	}
	animator->SetTarget(&sprite);
	SetState(States::Idle);

	for (int i = 0; i < 6; ++i)
	{
		SkillSet* newSkillSet = new SkillSet();
		newSkillSet->SetSubject(this, Skill::SubjectType::Player);
		skillSets.push_back(newSkillSet);
	}

	UseShader();
	SetSpriteShader();
	SetSpritePalette(64, "graphics/WizardPalette.png");
	SetPaletteColor(63);	// index �ٲ��־� �� ���� -> �Ӽ� ���� ���� �߰�

	hitboxSize = FloatRect(0.f, 0.f, 13.f, 25.f);
	SetHitBox(hitboxSize);
	SetHitBoxOrigin(Origins::MC);
	SetLowHitBox({ 0.f, 0.f, 8.f, 5.f }, Color::White);
	SetLowHitBoxOrigin(Origins::MC);

	shadow.setFillColor({ 0, 0, 0, 100 });
	shadow.setRadius(7.5f);
	Utils::SetOrigin(shadow, Origins::MC);
	shadow.setScale({ 1.f, 0.6f });

	itemMgr = new ItemMgr();
	itemMgr->SetPlayer(this);
	itemMgr->Apply();

	//SetCurHp(maxHp);
}

void Player::Update(float dt)
{
	SetLastPosition(GetPos());

	SpriteObj::Update(dt);
	shadow.setPosition(lowhitbox.getPosition());
	animator->Update(dt);
	itemMgr->Update(dt);

	if (currState == States::Die)
		return;

	auto& windowSize = FRAMEWORK->GetWindowSize();

	if (currState == States::Idle || currState == States::Run)
	{
		direction.x = 0.f;
		direction.y = 0.f;
		if (InputMgr::GetStackedOrder().empty())
		{
			direction.x += Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) ? 1 : 0;
			direction.x += Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A) ? -1 : 0;
			direction.y += Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S) ? 1 : 0;
			direction.y += Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) ? -1 : 0;

			if (!skillToolMode || InputMgr::GetMousePos().x < windowSize.x * 0.7f)
			{
				for (auto mouseDown : InputMgr::GetMouseDownList())
				{
					switch (mouseDown)
					{
					case Mouse::Left:
						SetCurrSkillSet(skillSets[0]);
						skillSets[0]->Restart();
						break;
					case Mouse::Right:
						SetCurrSkillSet(skillSets[2]);
						skillSets[2]->Restart();
						break;
					default:
						break;
					}
				}
			}

			for (auto keyDown : InputMgr::GetKeyDownList())
			{
				switch (keyDown)
				{
				case Keyboard::Space:
					SetCurrSkillSet(skillSets[1]);
					skillSets[1]->Restart();
					SetState(States::Dash);
					break;
				case Keyboard::Q:
					SetCurrSkillSet(skillSets[3]);
					skillSets[3]->Restart();
					break;
				case Keyboard::E:
					SetCurrSkillSet(skillSets[4]);
					skillSets[4]->Restart();
					break;
				case Keyboard::R:
					SetCurrSkillSet(skillSets[5]);
					skillSets[5]->Restart();
					break;
				default:
					break;
				}
			}
		}
		direction = Utils::Normalize(direction);
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
	case Player::States::Jump:
		UpdateJump(dt);
		break;
	case Player::States::Wait:
		UpdateWait(dt);
		break;
	case Player::States::Hit:
		UpdateHit(dt);
		break;
	case Player::States::Fall:
		UpdateFall(dt);
		break;
	default:
		break;
	}

	if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
	{
		lastDir = direction;
		dashDir = lastDir;
	}
	else if (!Utils::EqualFloat(lastDir.x, 0.f))
		dashDir = Utils::Normalize({ lastDir.x, 0.f });

	for (auto skillSet : skillSets)
	{
		skillSet->Update(dt);
	}

	Scene* currScene = SCENE_MGR->GetCurrentScene();
	if (currScene->GetType() != Scenes::Play || currState == States::Fall)
		return;
	vector<map<Object::ObjTypes, list<Object*>>>& collisionList = ((PlayScene*)currScene)->GetCollisionList();
	for (int i = 0; i < collisionList.size(); ++i)
	{
		if (collisionList[i][Object::ObjTypes::Player].empty())
			continue;
		for (auto& cliff : collisionList[i][Object::ObjTypes::Cliff])
		{
			if (cliff->GetHitBounds().intersects(GetLowHitBounds()))
			{
				if (currState != States::Hit && currState != States::Dash)
					SetPos(lastPosition);
				return;
			}
		}
	}
	lastStandingPos = position;
}

void Player::Draw(RenderWindow& window)
{
	window.draw(shadow);
	for (auto skill : skillSets)
	{
		skill->Draw(window);
	}
	SpriteObj::Draw(window);
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

	moveSoundTimer -= dt;

	if (moveSoundTimer <= 0.f)
	{
		SOUND_MGR->Play("sounds/PlayerFootstep.wav");
		moveSoundTimer = 0.4f;
	}

	if (accelTimer >= accelTime)
	{
		if (isRunning)
		{
			SOUND_MGR->Play("sounds/RunSpeed.wav");
			isRunning = false;
		}
		Translate(direction * runningSpeed * dt);
	}
	else
		Translate(direction * walkingSpeed * dt);

	if (Utils::EqualFloat(direction.x, lastDir.x) && Utils::EqualFloat(direction.y, lastDir.y))
		return;

	if (Utils::EqualFloat(direction.x, 0.f))
		direction.y > 0.f ? animator->Play("RunDown") : animator->Play("RunUp");
	else if(direction.x * lastDir.x <= 0.f)
		direction.x > 0.f ? animator->Play("RunRight") : animator->Play("RunLeft");
}

void Player::UpdateDash(float dt)
{
	dashTimer += dt;
	Translate(dashDir * runningSpeed * dt);
	if (dashTimer >= dashDuration)
	{
		dashTimer = 0.f;
		if (IsStanding())
			FinishAction();
	}
}

void Player::UpdateJump(float dt)
{
	Vector2f moving = direction * jumpDistance * dt / jumpDuration;
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
		FinishAction();
	}
}

void Player::UpdateWait(float dt)
{
	if (currSkillSet->GetCurrSkill()->GetDoing())
		return;
	if (!currSkillSet->Do())
	{
		if (currSkillSet->GetCurrSkill()->GetSetting()->playerAction == SkillAction::GroundSlam)
			SetState(States::GroundSlamEnd);
	}
}

void Player::Reset()
{
	SpriteObj::Reset();
	for (auto skillSet : skillSets)
	{
		skillSet->ResetSkills();
	}
	isBackHand = false;
	accelTimer = 0.f;
	dashTimer = 0.f;
	jumpTimer = 0.f;
	jumpDistance = 0.f;
	jumpOriginY = 0.f;
	direction = { 1.f, 0.f };
	lastDir = { 1.f, 0.f };
	dashDir = { 1.f, 0.f };
	SetState(States::Idle);
	currSkillSet = nullptr;
	curHp = maxHp;
	hitTimer = 0.f;
	fallTimer = 0.f;
	fallingScale = { 1.f, 1.f };
	for (auto extraSkillSet : extraSkillSets)
	{
		delete extraSkillSet;
	}
	extraSkillSets.clear();
}

void Player::UpdateHit(float dt)
{
	hitTimer += dt;
	Translate(-lastDir * walkingSpeed * 0.5f * dt);
	if (hitTimer >= hitDuration)
	{
		hitTimer = 0.f;
		if(IsStanding())
			SetState(States::Idle);
	}
}

void Player::UpdateFall(float dt)
{
	fallingScale.x -= dt * 0.5f;
	fallingScale.y -= dt * 0.5f;
	sprite.setScale(fallingScale);
	fallTimer += dt;
	if (fallTimer >= fallDuration)
	{
		fallingScale = { 1.f, 1.f };
		fallTimer = 0.f;
		sprite.setScale(1.f, 1.f);
		SetPos(lastStandingPos);
		curHp -= maxHp * 0.05f;
		if (curHp <= 0)
		{
			curHp = 0;
			SetState(States::Die);
			return;
		}
		SetState(States::Idle);
		SOUND_MGR->Play("sounds/FallRecover.wav");
	}
}

void Player::SetSpeed(float speed)
{
	walkingSpeed = speed;
	runningSpeed = speed * 1.5f;
}

bool Player::IsStanding()
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

void Player::Action(Skill* currSkill)
{
	SkillAction action = currSkill->GetSetting()->playerAction;
	if (action != SkillAction::Dash)
	{
		auto& mousePos = SCENE_MGR->GetCurrentScene()->GetObjMousePos();
		direction = Utils::Normalize(mousePos - position);
		if (!Utils::EqualFloat(direction.x, 0.f) || !Utils::EqualFloat(direction.y, 0.f))
			lastDir = direction;
		if (action == SkillAction::Jump)
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
	case SkillAction::Jump:
		SetState(States::Jump);
		break;
	case SkillAction::GroundSlam:
		SetState(States::GroundSlam);
		break;
	default:
		break;
	}
}                                                                                                                                                                                                                                                                               

void Player::FinishAction()
{
	if (currSkillSet != nullptr)
	{
		Skill* currSkill = currSkillSet->GetCurrSkill();
		if (currSkill != nullptr)
		{
			if (currSkill->GetSetting()->stopMoving == Skill::StopMoving::Immovable)
			{
				SetState(States::Wait);
				return;
			}
			if (currSkillSet->Do())
				return;
		}
	}
	switch (currState)
	{
	case Player::States::Dash:
		SetState(States::Slide);
		break;
	case Player::States::GroundSlam:
		SetState(States::GroundSlamEnd);
		break;
	default:
		SetState(States::Idle);
		break;
	}
}

void Player::SetSkillSet(int idx, const string& skillSetName, bool isPlayScene)
{
	skillSets[idx]->Set(skillSetName);

	auto currScene = SCENE_MGR->GetCurrentScene();
	if (isPlayScene || currScene->GetType() == Scenes::Play)
		((PlayUiMgr*)SCENE_MGR->GetScene(Scenes::Play)->GetUiMgr())->SetSkillIcon(idx, skillSets[idx]->GetIconDir());
}

void Player::OnHit(const Vector2f& atkDir, int dmg)
{
	if (Utils::RandomRange(0.f, 1.f) < evasionRate)
		return;

	if (currState == States::Fall)
		return;

	dmg = dmg * damageTake;
	PlayScene* playScene = (PlayScene*)SCENE_MGR->GetCurrentScene();
	auto showDamage = playScene->GetShowDamage()->Get();
	showDamage->ShowDamageFire(position, dmg);
	auto hitSpark = playScene->GetPlayerHitSpark()->Get();
	hitSpark->PlayerHitSparkFire(position);
	curHp -= dmg;
	CameraMove::SetShakeSpeed(1000.f);
	CameraMove::SetShakeViewX(50.f);
	CameraMove::SetViewShake(true);
	if (curHp <= 0)
	{
		curHp = 0;
		SetState(States::Die);
		return;
	}
	direction = -atkDir;
	lastDir = direction;
	dashDir = direction;
	SOUND_MGR->Play("sounds/ImpactPhysicalLight.wav");
	SetState(States::Hit);
}

void Player::AddExtraSkillSet(const string& skillSetName)
{
	SkillSet* newSkillSet = new SkillSet();
	newSkillSet->SetSubject(this, Skill::SubjectType::Player);
	newSkillSet->Set(skillSetName);
	extraSkillSets.push_back(newSkillSet);
}

void Player::ExchangeSkillSet(int idx, const string& skillSetName, bool isPlayScene)
{
	for (auto skillSet : extraSkillSets)
	{
		if (skillSet->GetSkillSetName() == skillSetName)
		{
			SkillSet* temp = skillSet;
			skillSet = skillSets[idx];
			skillSets[idx] = temp;
		}
	}

	auto currScene = SCENE_MGR->GetCurrentScene();
	if (isPlayScene || currScene->GetType() == Scenes::Play)
		((PlayUiMgr*)SCENE_MGR->GetScene(Scenes::Play)->GetUiMgr())->SetSkillIcon(idx, skillSets[idx]->GetIconDir());
}

void Player::SavePlatinum()
{
	rapidcsv::Document doc("tables/platinumTable.csv", rapidcsv::LabelParams(0, -1));

	doc.SetCell(1, 0, platinum);
	doc.Save("tables/platinumTable.csv");
}

void Player::LoadPlatinum()
{
	PlatinumTable* platinumTable = DATATABLE_MGR->Get<PlatinumTable>(DataTable::Types::Platinum);
	platinumTable->Load();
	auto& p = platinumTable->Get("Platinum");
	SetPlatinum(p.platinum);
}