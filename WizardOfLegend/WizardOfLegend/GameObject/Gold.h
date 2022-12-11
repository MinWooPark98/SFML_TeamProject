#pragma once
#include "../GameObject/SpriteObj.h"
#include "../Framework/Animator.h"

class Player;
class Gold : public SpriteObj
{
protected:
	float moveSpeed = 200.f;
	Player* player;
	Animator goldAnimation;

	bool animationPlay = true;

public:
	Gold() {};
	virtual ~Gold() {};

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Reset() override;

	void Drop(float dt);
	void GetGold(Player* player);
	void SetGoldPos(Vector2f pos);
};

