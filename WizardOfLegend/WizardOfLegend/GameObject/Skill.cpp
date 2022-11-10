#include "Skill.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/SkillTable.h"
#include "../Scene/SceneMgr.h"

Skill::Skill()

	:subject(nullptr), setting(nullptr), subType(SubjectType::None), isDoing(false), attackCnt(0), attackTimer(0.f), skillTimer(0.f)
{
}

Skill::~Skill()
{
}

void Skill::SetSkill(const String& skillName)
{
	SkillTable* table = DATATABLE_MGR->Get<SkillTable>(DataTable::Types::Skill);
	setting = new Set(table->Get(skillName));
}

void Skill::SetSubject(Object* sub, SubjectType type)
{
	subject = sub;
	subType = type;
}

void Skill::Do()
{
	if (setting == nullptr || subject == nullptr)
		return;
	isDoing = true;
	Projectile* obj = SCENE_MGR->GetCurrentScene()->GetProjectiles()->Get();
	obj->SetAtkOnce(setting->dmgType);
	obj->SetAtkDelay(setting->dmgDelay);
	obj->SetMovingDuration(setting->duration);
	obj->SetSpeed(setting->speed);
	obj->SetAnimClip(setting->animClipName);
	obj->SetAngle(0.f);
	projectiles.push_back(obj);
	++attackCnt;
	switch (subType)
	{
	case Skill::SubjectType::Player:
		((Player*)subject)->Action();
		skillDir = SCENE_MGR->GetCurrentScene()->GetObjMousePos() - subject->GetPos();
		switch (setting->attackShape)
		{
		case Projectile::AttackShape::Range:
			break;
		case Projectile::AttackShape::Rotate:
			break;
		case Projectile::AttackShape::Wave:
			obj->SetDirection(skillDir);
			obj->SetAmplitude(setting->amplitude);
			break;
		}
		obj->SetPos(subject->GetPos() + skillDir * setting->distance);
		obj->SetAtkDmg(setting->dmgRatio * ((Player*)subject)->GetAtkDmg());
		break;
	case Skill::SubjectType::Enemy:
		// 추후 추가
		break;
	default:
		break;
	}
}

void Skill::Update(float dt)
{
	if (!isDoing)
		return;
	isDoing = false;
	skillTimer += dt;
	{
		switch (setting->attackType)
		{
		case AttackType::Multiple:
			if (attackCnt < setting->attackCntLim && attackTimer >= setting->attackInterval)
				Do();
			break;
		case AttackType::SaveAttacks:
			{
				attackTimer += dt;
				if (attackCnt > 0 && skillTimer >= setting->skillDelay)
				{
					--attackCnt;
					skillTimer -= setting->skillDelay;
				}
				if (attackCnt < setting->attackCntLim && attackTimer >= setting->attackInterval)
					Do();
			}
			break;
		default:
			break;
		}
	}
	auto it = projectiles.begin();
	while(it != projectiles.end())
	{
		if (!(*it)->GetMoving())
		{
			(*it)->SetActive(false);
			projectiles.erase(it);
			continue;
		}
		(*it)->Update(dt);
		isDoing = true;
		++it;
	}

	if (projectiles.empty())
	{
		switch (setting->playerAction)
		{
		case Player::SkillAction::JumpSlash:
			// 플레이어 포지션 이동, 종료
		default:
			break;
		}
	}
}
