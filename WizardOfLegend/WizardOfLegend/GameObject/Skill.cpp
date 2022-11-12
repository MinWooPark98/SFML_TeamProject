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
	if (setting == nullptr || subject == nullptr || attackCnt >= setting->attackCntLim)
		return;

	Projectile* obj = SCENE_MGR->GetCurrentScene()->GetProjectiles()->Get();
	obj->SetAtkShape(setting->attackShape);
	obj->SetFrequency(setting->frequency);
	obj->SetMoveType(setting->moveType);
	obj->SetDmgType(setting->dmgType);
	obj->SetAtkDelay(setting->dmgDelay);
	obj->SetMovingDuration(setting->duration);
	obj->SetSpeed(setting->speed);
	obj->SetAnimClip(setting->animClipName);
	projectiles.push_back(obj);
	switch (subType)
	{
	case Skill::SubjectType::Player:
		if(!(setting->attackType == AttackType::Multiple && isDoing))
			((Player*)subject)->Action();
		switch (setting->attackShape)
		{
		case Projectile::AttackShape::Range:
			skillDir = Utils::Normalize(SCENE_MGR->GetCurrentScene()->GetObjMousePos() - subject->GetPos());
			break;
		case Projectile::AttackShape::Rotate:
			break;
		case Projectile::AttackShape::Wave:
			if(!(setting->attackType == AttackType::Multiple && isDoing))
				skillDir = Utils::Normalize(SCENE_MGR->GetCurrentScene()->GetObjMousePos() - subject->GetPos());
			if (((Player*)subject)->GetBackHand())
				obj->SetReverse(true);
			obj->SetDirection(skillDir);
			obj->SetAmplitude(setting->amplitude);
			break;
		}
		obj->SetStartPos(subject->GetPos() + skillDir * setting->distance);
		obj->SetAtkDmg(setting->dmgRatio * ((Player*)subject)->GetAtkDmg());
		break;
	case Skill::SubjectType::Enemy:
		// 추후 추가
		break;
	default:
		break;
	}
	isDoing = true;
	obj->Fire();
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
		if (attackCnt > 0 && skillTimer >= setting->skillDelay / setting->attackCntLim)
		{
			--attackCnt;
			skillTimer -= setting->skillDelay / setting->attackCntLim;
		}
		break;
	default:
		if (skillTimer >= setting->skillDelay)
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
					if (attackTimer >= setting->attackInterval)
					{
						Do();
						attackTimer = 0.f;
					}
				}
				else
					isDoing = false;
			}
			break;
		default:
			isDoing = false;
			break;
		}
	}
	auto it = projectiles.begin();
	if (isDoing && it == projectiles.end())
		isDoing = false;
	while(it != projectiles.end())
	{
		(*it)->Update(dt);
		if (!(*it)->GetMoving())
		{
			(*it)->SetActive(false);
			it = projectiles.erase(it);
			continue;
		}
		++it;
	}

	if (isDoing && projectiles.empty())
	{
		switch (setting->playerAction)
		{
		case Player::SkillAction::PBAoE:
			// 플레이어 애니메이션 및 상태 종료
			break;
		case Player::SkillAction::JumpSlash:
			// 플레이어 포지션 이동, 종료
			break;
		default:
			break;
		}
	}
}

void Skill::Draw(RenderWindow& window)
{
	for (auto projectile : projectiles)
	{
		if (projectile->GetActive())
			projectile->Draw(window);
	}
}
