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
		Count,
	};
	enum class AttackType
	{
		Single,
		Multiple,
		SaveAttacks,
	};
	
	struct Set
	{
		string skillName;						// ��ų �̸�
		Element element;						// ���� �Ӽ�
		AttackType attackType;					// �ܹ���, ������, ������  
		int attackCntLim;						// �ܹ��� = 1ȸ, ������ = ��ų ��� �� ���� �ߵ�, ������ = ���� �ִ� Ƚ��(��ų �� ��� �ߵ�) / shape_rotate�� ���� �Ұ�
		float attackInterval;					// ���� ����	/ shape_rotate�� ���� ������
		float distance;							// ����ڷκ����� �Ÿ�
		Projectile::AttackShape attackShape;	// range, rotate, wave
		float amplitude;						// ����  / shape_wave�� ���� ��ȿ, wave�� �� 0�̸� ����, Ŭ ���� ���� �ִ� �
		float frequency;						// �󸶳� ���� �����ϴ°�	/ shape_wave�� ���� ��ȿ
		Projectile::MoveType moveType;			// ���ƿ��°� / shape_wave�� ���� ��ȿ
		Player::SkillAction playerAction;		// �÷��̾� ���� ����(kick, jumpSmash ��)
		float skillDelay;						// ��ų �� ������
		float skillCoolDown;					// ��ų ������(��Ÿ�� �� ���� �ð�)
		float dmgRatio;							// ���ݷ� ��� ��ų ������ ����
		Projectile::DamageType dmgType;			// ���� Ÿ������
		float dmgDelay;							// ������ ���� ������
		float duration;							// ��ų ���� �ð�
		float speed;							// shape_range �� ������, shpae_rotate�� �� ������ �ݽð� ����, ��ų �̵� �ӵ�
		vector<string> animClipName;			// ��ų �ִϸ��̼� Ŭ�� string

		Set(){}
		Set(string skillName, Element element, AttackType attackType, int attackCntLim, float attackInterval, float distance, Projectile::AttackShape attackShape, float amplitude, float frequency, Projectile::MoveType moveType, Player::SkillAction playerAction, float skillDelay, float skillCoolDown, float dmgRatio, Projectile::DamageType dmgType, float dmgDelay, float duration, float speed, vector<string> animClipName)
			:skillName(skillName), element(element), attackType(attackType), attackCntLim(attackCntLim), attackInterval(attackInterval), distance(distance), attackShape(attackShape), amplitude(amplitude), frequency(frequency), moveType(moveType), playerAction(playerAction), skillDelay(skillDelay), skillCoolDown(skillCoolDown), dmgRatio(dmgRatio), dmgType(dmgType), dmgDelay(dmgDelay), duration(duration), speed(speed), animClipName(animClipName) {}
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

	bool isDoing;
	Vector2f skillDir;
	Vector2f startPos;

	Set* setting;
	int attackCnt;
	float attackTimer;
	float skillTimer;

public:
	Skill();
	~Skill();

	void SetSkill(const String& skillName);
	void SetSkill(const Set& set);
	void SetSubject(Object* sub, SubjectType type);
	void Do();
	void Update(float dt);
	void Draw(RenderWindow& window);

	Set* GetSetting() { return setting; }
};
