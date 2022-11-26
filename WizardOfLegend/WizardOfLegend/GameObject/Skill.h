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
	};
	
	struct Set
	{
		string skillName;						// ��ų �̸�
		Element element;						// ���� �Ӽ�
		AttackType attackType;					// �ܹ���, ������, ������  
		int attackCntLim;						// �ܹ��� = 1ȸ, ������ = ��ų ��� �� ���� �ߵ�, ������ = ���� �ִ� Ƚ��(��ų �� ��� �ߵ�) / shape_rotate�� ���� �Ұ�
		float attackInterval;					// ���� ����	/ shape_rotate�� ���� ������
		float distance;							// ����ڷκ����� �Ÿ�
		AttackShape attackShape;				// range, rotate, wave
		float amplitude;						// ����  / shape_wave, range�� ���� ��ȿ, wave�� �� 0�̸� ����, Ŭ ���� ���� �ִ� �
		float frequency;						// �󸶳� ���� �����ϴ°�	/ shape_wave�� ���� ��ȿ
		WaveType waveType;						// ���ƿ��°� / shape_wave�� ���� ��ȿ
		float fallingHeight;					// shape_range �� ���� ��ȿ
		RangeType rangeType;					// shape_range �� ���� ��ȿ
		Player::SkillAction playerAction;		// �÷��̾� ���� ����(kick, jumpSmash ��)
		float skillDelay;						// ��ų �� ������
		float skillCoolDown;					// ��ų ������(��Ÿ�� �� ���� �ð�)
		float dmgRatio;							// ���ݷ� ��� ��ų ������ ����
		DamageType dmgType;						// ���� Ÿ������
		float dmgDelay;							// ������ ���� ������
		float duration;							// ��ų ���� �ð�
		float speed;							// shape_range �� ������, shpae_rotate�� �� ������ �ݽð� ����, ��ų �̵� �ӵ�
		vector<string> animClipName;			// ��ų �ִϸ��̼� Ŭ�� string

		Set(){}
		Set(string skillName, Element element, AttackType attackType, int attackCntLim, float attackInterval, float distance, AttackShape attackShape, float amplitude, float frequency, WaveType waveType, float fallingHeight, RangeType rangeType, Player::SkillAction playerAction, float skillDelay, float skillCoolDown, float dmgRatio, DamageType dmgType, float dmgDelay, float duration, float speed, vector<string> animClipName)
			:skillName(skillName), element(element), attackType(attackType), attackCntLim(attackCntLim), attackInterval(attackInterval), distance(distance), attackShape(attackShape), amplitude(amplitude), frequency(frequency), waveType(waveType), fallingHeight(fallingHeight), rangeType(rangeType), playerAction(playerAction), skillDelay(skillDelay), skillCoolDown(skillCoolDown), dmgRatio(dmgRatio), dmgType(dmgType), dmgDelay(dmgDelay), duration(duration), speed(speed), animClipName(animClipName) {}
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
