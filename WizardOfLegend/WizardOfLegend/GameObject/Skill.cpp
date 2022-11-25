#include "Skill.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/SkillTable.h"
#include "../Scene/SceneMgr.h"

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
	if(setting->attackShape != Projectile::AttackShape::Range)
		obj->SetDistance(setting->distance);
	obj->SetAnimClip(setting->animClipName);
	switch (subType)
	{
	case Skill::SubjectType::Player:
		if(!(isDoing && (setting->attackType == AttackType::Multiple || setting->playerAction != Player::SkillAction::NormalSpell)))
			((Player*)subject)->Action();
		switch (setting->attackShape)
		{
		case Projectile::AttackShape::Range:
			if (!isDoing)
			{
				auto mouseVec = SCENE_MGR->GetCurrentScene()->GetObjMousePos() - subject->GetPos();
				auto mouseDistance = Utils::Magnitude(mouseVec);
				skillDir = Utils::Normalize(mouseVec);
				distance = mouseDistance <= setting->distance ? mouseDistance : setting->distance;
				startPos = subject->GetPos() + skillDir * setting->distance;
			}
			obj->SetDirection(skillDir);
			obj->SetDistance(distance);
			obj->SetPos(setting->rangeType == Projectile::RangeType::AbovePlayer ? subject->GetPos() : startPos);
			if (isDoing)
				obj->Translate(Utils::RandAreaPoint() * setting->amplitude);
			break;
		case Projectile::AttackShape::Rotate:
			if (isDoing)
				obj->SetAngle(projectiles.back()->GetAngle() + 360.f / setting->attackCntLim);
			startPos = subject->GetPos();
			obj->SetStartPos(startPos);
			break;
		case Projectile::AttackShape::Wave:
			if (!(setting->attackType == AttackType::Multiple && isDoing))
			{
				skillDir = Utils::Normalize(SCENE_MGR->GetCurrentScene()->GetObjMousePos() - subject->GetPos());
				startPos = subject->GetPos() + skillDir * setting->distance;
			}
			if (((Player*)subject)->GetBackHand())
				obj->SetReverse(true);
			obj->SetStartPos(startPos);
			obj->SetDirection(skillDir);
			obj->SetAmplitude(setting->amplitude);
			break;
		}
		obj->SetAtkDmg(setting->dmgRatio * ((Player*)subject)->GetAtkDmg());
		break;
	case Skill::SubjectType::Enemy:
		switch (setting->attackShape)
		{
		case Projectile::AttackShape::Range:
			obj->SetDirection(skillDir);
			startPos = subject->GetPos() + skillDir * setting->distance;
			obj->SetPos(startPos);
			break;
		case Projectile::AttackShape::Rotate:
			if (isDoing)
				obj->SetAngle(projectiles.back()->GetAngle() + 360.f / setting->attackCntLim);
			startPos = subject->GetPos();
			obj->SetStartPos(startPos);
			break;
		case Projectile::AttackShape::Wave:
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
	default:
		break;
	}
	isDoing = true;
	obj->SetMoving(true);
	projectiles.push_back(obj);
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
					if (setting->attackShape != Projectile::AttackShape::Rotate && attackTimer < setting->attackInterval)
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
	auto it = projectiles.begin();
	while(it != projectiles.end())
	{
		if (setting->attackShape == Projectile::AttackShape::Rotate)
			(*it)->SetStartPos(subject->GetPos());

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

void Skill::Set::Reset()
{
	skillName.clear();
	element = Element::Fire;
	attackType = AttackType::Single;
	attackCntLim = 0;
	attackInterval = 0.f;
	distance = 0.f;
	attackShape = Projectile::AttackShape::None;
	amplitude = 0.f;
	frequency = 0.f;
	waveType = Projectile::WaveType::OneWay;
	playerAction = Player::SkillAction::NormalSpell;
	skillDelay = 0.f;
	skillCoolDown = 0.f;
	dmgRatio = 0.f;
	dmgType = Projectile::DamageType::Once;
	dmgDelay = 0.f;
	duration = 0.f;
	speed = 0.f;
	for (auto& name : animClipName)
	{
		name.clear();
	}
}
