#include "Skill.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/SkillTable.h"
#include "../Scene/SceneMgr.h"
#include "Projectile.h"
#include "CastingCircle.h"
#include "../Framework/SoundMgr.h"
#include "FinalBoss.h"
#include "../Scene/PlayScene.h"
#include "../FrameWork/RayCast.h"

Skill::Skill()
	:subject(nullptr), setting(nullptr), subType(SubjectType::None), isDoing(false), distance(0.f), attackCnt(0), attackTimer(0.f), skillTimer(0.f)
{
}

Skill::~Skill()
{
}

void Skill::Reset()
{
	for (auto projectile : projectiles)
	{
		projectile->SetActive(false);
	}
	projectiles.clear();
	for (auto circle : castingCircles)
	{
		circle->SetActive(false);
	}
	castingCircles.clear();
	Reprepare();
}

void Skill::Reprepare()
{
	isDoing = false;
	attackCnt = 0;
	attackTimer = 0.f;
	skillTimer = 0.f;
}

void Skill::SetSkill(const String& skillName)
{
	Reset();
	SkillTable* table = DATATABLE_MGR->Get<SkillTable>(DataTable::Types::Skill);
	if (setting == nullptr)
		setting = new Set(table->Get(skillName));
	else
		*setting = table->Get(skillName);
}

void Skill::SetSkill(const Set& set)
{
	Reset();
	if (setting == nullptr)
		setting = new Set(set);
	else
		*setting = set;
}

void Skill::SetSubject(Object* sub, SubjectType type)
{
	subject = sub;
	subType = type;
}

void Skill::Do()
{
	if (setting == nullptr || subject == nullptr || attackCnt >= setting->attackCntLim)
		return;
	Projectile* obj = SCENE_MGR->GetCurrentScene()->GetProjectiles()->Get();
	obj->SetSubjectType(subType);
	obj->SetAtkShape(setting->attackShape);
	obj->SetFrequency(setting->frequency);
	obj->SetWaveType(setting->waveType);
	obj->SetFallingHeight(setting->fallingHeight);
	obj->SetRangeType(setting->rangeType);
	obj->SetDelay(setting->skillDelay);
	obj->SetDmgType(setting->dmgType);
	obj->SetAtkDelay(setting->dmgDelay);
	obj->SetMovingDuration(setting->duration);
	obj->SetSpeed(setting->speed);
	if(setting->attackShape != AttackShape::Range)
		obj->SetDistance(setting->distance);
	obj->SetAnimClip(setting->animClipName);
	switch (subType)
	{
	case Skill::SubjectType::Player:
		if(!(isDoing && (setting->attackType == AttackType::Multiple || setting->playerAction != Player::SkillAction::NormalSpell)))
			((Player*)subject)->Action(this);
		obj->SetAtkDmg(setting->dmgRatio * ((Player*)subject)->GetAtkDmg());
		switch (setting->attackShape)
		{
		case AttackShape::Surrounded:
			obj->SetDirection({ 0.f, 0.f });
			obj->SetPos(subject->GetPos());
			break;
		case AttackShape::Range:
			{
				if (setting->rangeType == RangeType::Default)
				{
					obj->SetDirection({ 0.f, 0.f });
					obj->SetDistance(0.f);
					obj->SetPos(subject->GetPos());
					break;
				}
				if (!isDoing)
				{
					auto currScene = SCENE_MGR->GetCurrentScene();
					auto mouseVec = currScene->GetObjMousePos() - subject->GetPos();
					auto mouseDistance = Utils::Magnitude(mouseVec);
					skillDir = Utils::Normalize(mouseVec);
					distance = mouseDistance <= setting->distance ? mouseDistance : setting->distance;
					startPos = subject->GetPos() + skillDir * distance;
					RayCast ray(subject->GetPos(), skillDir, distance);
					ray.SetObjType(Object::ObjTypes::Wall);
					ray.Update();
					if (ray.RayHit())
						startPos = ray.GetHittingPoint();
				}
				obj->SetDirection(skillDir);
				obj->SetDistance(distance);
				obj->SetPos(setting->rangeType == RangeType::FromAbovePlayer ? subject->GetPos() : startPos);
				Vector2f translation = Utils::RandAreaPoint() * setting->amplitude;
				if (setting->rangeType == RangeType::FromAbovePlayer && setting->attackType == AttackType::Multiple)
					translation *= setting->frequency;
				if (isDoing)
				{
					obj->Translate(translation);
					if (setting->rangeType == RangeType::VerticalDescent)
					{
						for (auto circle : castingCircles)
							circle->SetTimer(0.f);
						break;
					}
				}
				CastingCircle* circle = SCENE_MGR->GetCurrentScene()->GetCastingCircles()->Get();
				circle->SetSubjectType(subType);
				circle->SetPos(startPos);
				circle->SetDuration(setting->duration);
				circle->Do();
				circle->SetSize({ setting->amplitude * 2.f, setting->amplitude * 2.f });
				castingCircles.push_back(circle);
				if (setting->rangeType == RangeType::FromAbovePlayer)
				{
					obj->SetFallingHeight(setting->fallingHeight);
					obj->SetPos(startPos);
					if(setting->attackType != AttackType::Single)
						circle->SetColor({ 255, 255, 255, 0 });
					if (isDoing)
					{
						obj->Translate(translation);
						circle->Translate(translation);
					}
				}
			}
			break;
		case AttackShape::Rotate:
			if (isDoing)
				obj->SetAngle(projectiles.back()->GetAngle() + 360.f / setting->attackCntLim);
			startPos = subject->GetPos();
			obj->SetStartPos(startPos);
			break;
		case AttackShape::Wave:
			if (!(setting->attackType == AttackType::Multiple && isDoing))
			{
				skillDir = Utils::Normalize(SCENE_MGR->GetCurrentScene()->GetObjMousePos() - subject->GetPos());
				startPos = subject->GetPos();
			}
			if (((Player*)subject)->GetBackHand())
				obj->SetReverse(true);
			obj->SetStartPos(startPos);
			obj->SetDirection(skillDir);
			obj->SetAmplitude(setting->amplitude);
			break;
		}
		break;
	case Skill::SubjectType::Enemy:
		switch (setting->attackShape)
		{
		case AttackShape::Range:
			obj->SetDirection(skillDir);
			startPos = subject->GetPos() + skillDir * setting->distance;
			obj->SetPos(startPos);
			break;
		case AttackShape::Rotate:
			if (isDoing)
				obj->SetAngle(projectiles.back()->GetAngle() + 360.f / setting->attackCntLim);
			startPos = subject->GetPos();
			obj->SetStartPos(startPos);
			break;
		case AttackShape::Wave:
			if (!(setting->attackType == AttackType::Multiple && isDoing))
			{
				startPos = subject->GetPos() + skillDir * setting->distance;
			}
			obj->SetStartPos(startPos);
			obj->SetDirection(skillDir);
			obj->SetAmplitude(setting->amplitude);
			break;
		}
		obj->SetAtkDmg(setting->dmgRatio * ((Enemy*)subject)->GetDamage());
		break;
	case SubjectType::FinalBoss:
		{
			auto& playerPos = SCENE_MGR->GetCurrentScene()->FindGameObj("player")->GetPos();
			if (!(isDoing && (setting->attackType == AttackType::Multiple || setting->playerAction != Player::SkillAction::NormalSpell)))
				((FinalBoss*)subject)->Action(this);
			obj->SetAtkDmg(setting->dmgRatio * ((FinalBoss*)subject)->GetAtkDmg());
			switch (setting->attackShape)
			{
			case AttackShape::Surrounded:
				obj->SetDirection({ 0.f, 0.f });
				obj->SetPos(subject->GetPos());
				break;
			case AttackShape::Range:
				{
					if (setting->rangeType == RangeType::Default)
					{
						obj->SetDirection({ 0.f, 0.f });
						obj->SetDistance(0.f);
						obj->SetPos(subject->GetPos());
						break;
					}
					if (!isDoing)
					{
						auto dir = playerPos - subject->GetPos();
						auto dist = Utils::Magnitude(dir);
						skillDir = Utils::Normalize(dir);
						distance = dist <= setting->distance ? dist : setting->distance;
						startPos = subject->GetPos() + skillDir * distance;
					}
					obj->SetDirection(skillDir);
					obj->SetDistance(distance);
					obj->SetPos(setting->rangeType == RangeType::FromAbovePlayer ? subject->GetPos() : startPos);
					Vector2f translation = Utils::RandAreaPoint() * setting->amplitude;
					if (setting->rangeType == RangeType::FromAbovePlayer && setting->attackType == AttackType::Multiple)
						translation *= setting->frequency;
					if (isDoing)
					{
						obj->Translate(translation);
						if (setting->rangeType == RangeType::VerticalDescent)
						{
							for (auto circle : castingCircles)
								circle->SetTimer(0.f);
							break;
						}
					}
					CastingCircle* circle = SCENE_MGR->GetCurrentScene()->GetCastingCircles()->Get();
					circle->SetSubjectType(subType);
					circle->SetPos(startPos);
					circle->SetDuration(setting->duration);
					circle->Do();
					circle->SetSize({ setting->amplitude * 2.f, setting->amplitude * 2.f });
					castingCircles.push_back(circle);
					if (setting->rangeType == RangeType::FromAbovePlayer)
					{
						obj->SetFallingHeight(setting->fallingHeight);
						obj->SetPos(startPos);
						if (setting->attackType != AttackType::Single)
							circle->SetColor({ 255, 255, 255, 0 });
						if (isDoing)
						{
							obj->Translate(translation);
							circle->Translate(translation);
						}
					}
					break;
				}
				break;
			case AttackShape::Rotate:
				if (isDoing)
					obj->SetAngle(projectiles.back()->GetAngle() + 360.f / setting->attackCntLim);
				startPos = subject->GetPos();
				obj->SetStartPos(startPos);
				break;
			case AttackShape::Wave:
				if (!(setting->attackType == AttackType::Multiple && isDoing))
				{
					skillDir = Utils::Normalize(playerPos - subject->GetPos());
					startPos = subject->GetPos();
				}
				if (((FinalBoss*)subject)->GetBackHand())
					obj->SetReverse(true);
				obj->SetStartPos(startPos);
				obj->SetDirection(skillDir);
				obj->SetAmplitude(setting->amplitude);
				break;
			}
		}
		break;
	default:
		break;
	}
	isDoing = true;
	obj->SetMoving(true);
	projectiles.push_back(obj);
	if(!setting->soundName[0].empty())
		SOUND_MGR->Play(setting->soundName[0]);
	++attackCnt;
}

void Skill::Update(float dt)
{
	if (setting == nullptr || subject == nullptr)
		return; 
	if(attackCnt != 0 && !isDoing)
		skillTimer += dt;

	switch (setting->attackType)
	{
	case AttackType::SaveAttacks:
		if (attackCnt > 0 && skillTimer >= setting->skillCoolDown / setting->attackCntLim)
		{
			--attackCnt;
			skillTimer -= setting->skillCoolDown / setting->attackCntLim;
		}
		break;
	default:
		if (skillTimer >= setting->skillCoolDown)
		{
			attackCnt = 0;
			skillTimer = 0.f;
		}
		break;
	}

	if (isDoing)
	{
		switch (setting->attackType)
		{
		case AttackType::Multiple:
		case AttackType::SaveAttacks:
			{
				attackTimer += dt;
				if (attackCnt < setting->attackCntLim)
				{
					if (setting->attackShape != AttackShape::Rotate && attackTimer < setting->attackInterval)
						break;
					Do();
					attackTimer = 0.f;
				}
				else
				{
					isDoing = false;
				}
			}
			break;
		default:
			isDoing = false;
			break;
		}
	}
	{
		auto it = projectiles.begin();
		while (it != projectiles.end())
		{
			if (setting->attackShape == AttackShape::Surrounded)
				(*it)->SetPos(subject->GetPos());
			if (setting->attackShape == AttackShape::Rotate)
				(*it)->SetStartPos(subject->GetPos());

			if (!(*it)->GetMoving())
			{
				(*it)->SetActive(false);
				if (!setting->soundName[1].empty())
					SOUND_MGR->Play(setting->soundName[1]);
				it = projectiles.erase(it);
				continue;
			}
			++it;
		}
	}

	if (projectiles.empty())
	{
		bool isVerticalDescent = setting->rangeType == RangeType::VerticalDescent;
		auto it = castingCircles.begin();
		while (it != castingCircles.end())
		{
			if(isVerticalDescent)
				(*it)->SetActive(false);
			if (!(*it)->GetActive())
			{
				it = castingCircles.erase(it);
				continue;
			}
			++it;
		}
	}
}

void Skill::Draw(RenderWindow& window)
{
	for (auto circle : castingCircles)
	{
		if (circle->GetActive())
			circle->Draw(window);
	}
	for (auto projectile : projectiles)
	{
		if (projectile->GetActive())
			projectile->Draw(window);
	}
}

void Skill::Set::Reset()
{
	skillName.clear();
	element = Element::Fire;
	attackType = AttackType::Single;
	attackCntLim = 0;
	attackInterval = 0.f;
	distance = 0.f;
	attackShape = AttackShape::None;
	amplitude = 0.f;
	frequency = 0.f;
	waveType = WaveType::OneWay;
	playerAction = Player::SkillAction::NormalSpell;
	skillDelay = 0.f;
	skillCoolDown = 0.f;
	dmgRatio = 0.f;
	dmgType = DamageType::Once;
	dmgDelay = 0.f;
	duration = 0.f;
	speed = 0.f;
	for (auto& name : animClipName)
	{
		name.clear();
	}
}
