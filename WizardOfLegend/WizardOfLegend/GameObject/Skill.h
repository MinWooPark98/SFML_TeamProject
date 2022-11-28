#pragma once
#include "Object.h"
#include <list>
#include "Player.h"
#include "Enemy.h"
#include "../Framework/ObjectPool.h"

class Projectile;
class CastingCircle;

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
		Count,
	};
	enum class AttackType
	{
		Single,
		Multiple,
		SaveAttacks,
	};
	enum class AttackShape
	{
		None = -1,
		Surrounded,
		Range,
		Rotate,
		Wave,
	};
	enum class WaveType
	{
		None = -1,
		OneWay,
		BackAndForth,
	};
	enum class RangeType
	{
		None = -1,
		Default,
		VerticalDescent,
		FromAbovePlayer,
	};
	enum class DamageType
	{
		Once,
		Periodic,
		NoDamage,
	};
	
	struct Set
	{
		string skillName;						// 스킬 이름
		Element element;						// 원소 속성
		AttackType attackType;					// 단발형, 연발형, 충전형  
		int attackCntLim;						// 단발형 = 1회, 연발형 = 스킬 사용 시 연속 발동, 충전형 = 충전 최대 횟수(스킬 시 모두 발동) / shape_rotate일 경우는 불가
		float attackInterval;					// 연사 간격	/ shape_rotate일 경우는 미적용
		float distance;							// 사용자로부터의 거리
		AttackShape attackShape;				// range, rotate, wave
		float amplitude;						// 진폭  / shape_wave, range일 때만 유효, wave일 때 0이면 직선, 클 수록 많이 휘는 곡선
		float frequency;						// 얼마나 자주 진동하는가	/ shape_wave일 때만 유효
		WaveType waveType;						// 돌아오는가 / shape_wave일 때만 유효
		float fallingHeight;					// shape_range 일 때만 유효
		RangeType rangeType;					// shape_range 일 때만 유효
		Player::SkillAction playerAction;		// 플레이어 동작 변경(kick, jumpSmash 등)
		float skillDelay;						// 스킬 선 딜레이
		float skillCoolDown;					// 스킬 딜레이(쿨타임 및 충전 시간)
		DamageType dmgType;						// 단일 타격인지
		float dmgRatio;							// 공격력 대비 스킬 데미지 비율
		float dmgDelay;							// 데미지 적용 딜레이
		float duration;							// 스킬 지속 시간
		float speed;							// shape_range 는 미적용, shpae_rotate일 때 음수면 반시계 방향, 스킬 이동 속도
		vector<string> animClipName;			// 스킬 애니메이션 클립 string
		vector<string> soundName;				// 스킬 시작 사운드, 종료 사운드

		Set(){}
		Set(string skillName, Element element, AttackType attackType, int attackCntLim, float attackInterval, float distance, AttackShape attackShape, float amplitude, float frequency, WaveType waveType, float fallingHeight, RangeType rangeType, Player::SkillAction playerAction, float skillDelay, float skillCoolDown, DamageType dmgType, float dmgRatio, float dmgDelay, float duration, float speed, vector<string> animClipName, vector<string> soundName)
			:skillName(skillName), element(element), attackType(attackType), attackCntLim(attackCntLim), attackInterval(attackInterval), distance(distance), attackShape(attackShape), amplitude(amplitude), frequency(frequency), waveType(waveType), fallingHeight(fallingHeight), rangeType(rangeType), playerAction(playerAction), skillDelay(skillDelay), skillCoolDown(skillCoolDown), dmgType(dmgType), dmgRatio(dmgRatio), dmgDelay(dmgDelay), duration(duration), speed(speed), animClipName(animClipName), soundName(soundName) {}
		void Reset();
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
	list<CastingCircle*> castingCircles;

	bool isDoing;
	Vector2f skillDir;
	Vector2f startPos;
	float distance;

	Set* setting;
	int attackCnt;
	float attackTimer;
	float skillTimer;

public:
	Skill();
	~Skill();

	void Reset();

	void SetSkill(const String& skillName);
	void SetSkill(const Set& set);
	void SetSubject(Object* sub, SubjectType type);
	void Do();
	void Update(float dt);
	void Draw(RenderWindow& window);

	Set* GetSetting() { return setting; }

	Vector2f GetSkillDir() const { return skillDir; };
	void SetSkillDir(const Vector2f& dir) { skillDir = dir; };
};
