#pragma once
#include <string>

using namespace std;

class Player;

class Item
{
public:
	enum class Types
	{
		Relic,
		Hood,
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

protected:
	Types type;

	Player* player;

public:
	Item(Types type);
	virtual ~Item();

	void SetPlayer(Player* player) { this->player = player; }

	virtual void Update(float dt) {}
};

