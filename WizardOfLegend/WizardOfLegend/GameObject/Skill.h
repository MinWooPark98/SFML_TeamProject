#pragma once
#include "Object.h"
#include <list>
#include "Projectile.h"
#include "Player.h"

class Projectile;

class Skill
{
public:
	enum class Element
	{
		Fire,
		Air,
		Earth,
		Lightning,
		Water,
		Chaos,
	};
	enum class AttackType
	{
		Single,
		Multiple,
		SaveAttacks,
	};
	
	struct Set
	{
		string skillName;						// 스킬 이름
		Element element;						// 원소 속성
		AttackType attackType;					// 단발형, 연발형, 충전형  
		int attackCntLim;						// 단발형 = 1회, 연발형 = 스킬 사용 시 연속 발동, 충전형 = 충전 최대 횟수(스킬 시 모두 발동) / shape_rotate일 경우는 불가
		float attackInterval;					// 연사 간격	/ shape_rotate일 경우는 미적용
		float distance;							// 사용자로부터의 거리
		Projectile::AttackShape attackShape;	// range, rotate, wave
		float amplitude;						// 진폭  / shape_wave일 때만 유효, wave일 때 0이면 직선, 클 수록 많이 휘는 곡선
		float frequency;						// 얼마나 자주 진동하는가	/ shape_wave일 때만 유효
		Projectile::MoveType moveType;			// 돌아오는가 / shape_wave일 때만 유효
		Player::SkillAction playerAction;		// 플레이어 동작 변경(kick, jumpSmash 등)
		float skillDelay;						// 스킬 선 딜레이
		float skillCoolDown;					// 스킬 딜레이(쿨타임 및 충전 시간)
		float dmgRatio;							// 공격력 대비 스킬 데미지 비율
		Projectile::DamageType dmgType;			// 단일 타격인지
		float dmgDelay;							// 데미지 적용 딜레이
		float duration;							// 스킬 지속 시간
		float speed;							// shape_range 는 미적용, shpae_rotate일 때 음수면 반시계 방향, 스킬 이동 속도
		vector<string> animClipName;			// 스킬 애니메이션 클립 string

		Set(){}
		Set(string skillName, Element element, AttackType attackType, int attackCntLim, float attackInterval, float distance, Projectile::AttackShape attackShape, float amplitude, float frequency, Projectile::MoveType moveType, Player::SkillAction playerAction, float skillDelay, float skillCoolDown, float dmgRatio, Projectile::DamageType dmgType, float dmgDelay, float duration, float speed, vector<string> animClipName)
			:skillName(skillName), element(element), attackType(attackType), attackCntLim(attackCntLim), attackInterval(attackInterval), distance(distance), attackShape(attackShape), amplitude(amplitude), frequency(frequency), moveType(moveType), playerAction(playerAction), skillDelay(skillDelay), skillCoolDown(skillCoolDown), dmgRatio(dmgRatio), dmgType(dmgType), dmgDelay(dmgDelay), duration(duration), speed(speed), animClipName(animClipName) {}
	};

	enum class SubjectType
	{
		None = -1,
		Player,
		Enemy,
	};

private:
	Object* subject;
	SubjectType subType;

	list<Projectile*> projectiles;

	bool isDoing;
	Vector2f skillDir;

	Set* setting;
	int attackCnt;
	float attackTimer;
	float skillTimer;

public:
	Skill();
	~Skill();

	void SetSkill(const String& skillName);
	void SetSubject(Object* sub, SubjectType type);
	void Do();
	void Update(float dt);
	void Draw(RenderWindow& window);

	Set* GetSetting() { return setting; }
};
