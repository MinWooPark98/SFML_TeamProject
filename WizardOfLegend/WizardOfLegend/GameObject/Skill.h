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
		string skillName;						// ��ų �̸�
		Element element;						// ���� �Ӽ�
		AttackType attackType;					// �ܹ���, ������, ������  
		int attackCntLim;						// �ܹ��� = 1ȸ, ������ = ��ų ��� �� ���� �ߵ�, ������ = ���� �ִ� Ƚ��(��ų �ߵ� �� ��� �ߵ�)
		float attackInterval;					// ���� ����
		float distance;							// ����ڷκ����� �Ÿ�
		Projectile::AttackShape attackShape;	// range, rotate, wave
		float amplitude;						// ����  / shape_range ���� 0����, wave�� �� 0�̸� ����, Ŭ ���� ���� �ִ� �
		float frequency;						// �󸶳� ���� �����ϴ°�	/ shape_wave�� ���� ��ȿ
		Projectile::MoveType moveType;			// ���ƿ��°� / shape_range�� ���� 0
		Player::SkillAction playerAction;		// �÷��̾� ���� ����(kick, jumpSmash ��)
		float skillDelay;						// ��ų ������(��Ÿ�� �� ���� �ð�)
		float dmgRatio;							// ���ݷ� ��� ��ų ������ ����
		Projectile::DamageType dmgType;			// ���� Ÿ������
		float dmgDelay;							// ������ ���� ������
		float duration;							// ��ų ���� �ð�
		float speed;							// shape_range �� ������, ��ų �̵� �ӵ�
		vector<string> animClipName;			// ��ų �ִϸ��̼� Ŭ�� string

		Set(string skillName, Element element, AttackType attackType, int attackCntLim, float attackInterval, float distance, Projectile::AttackShape attackShape, float amplitude, float frequency, Projectile::MoveType moveType, Player::SkillAction playerAction, float skillDelay, float dmgRatio, Projectile::DamageType dmgType, float dmgDelay, float duration, float speed, vector<string> animClipName)
			:skillName(skillName), element(element), attackType(attackType), attackCntLim(attackCntLim), attackInterval(attackInterval), distance(distance), attackShape(attackShape), amplitude(amplitude), frequency(frequency), moveType(moveType), playerAction(playerAction), skillDelay(skillDelay), dmgRatio(dmgRatio), dmgType(dmgType), dmgDelay(dmgDelay), duration(duration), speed(speed), animClipName(animClipName) {}
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
