#pragma once
#include <string>

using namespace std;

class Player;

class Item
{
public:
	enum class Types
	{
		Item,
		Hood,
	};
	enum class Condition
	{
		None,
		HpLower,
		HpUpper,
		NumOfItems,
		OnHit,
		WhileDashing,
	};
	struct Values
	{
		float speed;
		float atkDmg;
		float dmg;
		float maxHp;
		float evasionRate;
		float criticalRate;
		float criticalRatio;

		Values() {}
		Values(float speed, float atkDmg, float dmg, float maxHp, float evasionRate, float criticalRate, float criticalRatio) : speed(speed), atkDmg(atkDmg), dmg(dmg), maxHp(maxHp), evasionRate(evasionRate), criticalRate(criticalRate), criticalRatio(criticalRatio) {}

		void Reset()
		{
			speed = 0.f;
			atkDmg = 0.f;
			dmg = 0.f;
			maxHp = 0.f;
			evasionRate = 0.f;
			criticalRate = 0.f;
			criticalRatio = 0.f;
		}

		Values operator+(const Values& val) const
		{
			Values temp(speed + val.speed, atkDmg + val.atkDmg, dmg + val.dmg, maxHp + val.maxHp, evasionRate + val.evasionRate, criticalRate + val.criticalRate, criticalRatio + val.criticalRatio);
			return temp;
		}
		template <typename T>
		Values operator*(const T& val) const
		{
			Values temp(speed * val, atkDmg * val, dmg * val, maxHp * val, evasionRate * val, criticalRate * val, criticalRatio * val);
			return temp;
		}
		Values& operator+=(const Values& val)
		{
			(*this) = (*this) + val;
			return *this;
		}
	};
	struct Info
	{
		int id;
		Types type;
		string name;
		Values values;
		Condition condition;
		float conditionValue;
		float duration;
		int price;
		string iconDir;
		string intro;
	};

protected:
	Info info;
	bool applyValues;

	Player* player;

public:
	Item();
	virtual ~Item();

	void Reset();

	void SetInfo(const Info& info);
	const Info& GetInfo() { return info; }
	const Values& GetValues() { return info.values; }
	bool GetApplyValues() { return applyValues; }
	void SetPlayer(Player* player) { this->player = player; }

	void Update(float dt);

	void CheckHpLower();
	void CheckHpUpper();
	void CheckNumOfItems();
	void CheckOnHit();
	void CheckWhileDashing();
};

