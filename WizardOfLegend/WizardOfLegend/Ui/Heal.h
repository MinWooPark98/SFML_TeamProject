#pragma once
#include "../GameObject/SpriteObj.h"
#include "../Framework/Animator.h"

class Player;
class Heal : public SpriteObj
{
protected:
	Animator animation;
	Player* player;

public:
	Heal() {};
	virtual ~Heal() {};

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Reset() override;

	void SetHealObject(string texture, string animationClipName, Vector2f hitBoxXY);
	void OnHit(int dmg, int heal);
};

