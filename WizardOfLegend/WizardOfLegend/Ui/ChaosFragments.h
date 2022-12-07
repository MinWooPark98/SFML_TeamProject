#pragma once
#include "../GameObject/SpriteObj.h"
#include "../Framework/Animator.h"

class Player;
class ChaosFragments : public SpriteObj
{
protected:
	float moveSpeed = 200.f;
	Player* player;
	Animator animation;

	bool animationPlay = true;

public:
	ChaosFragments() {};
	virtual ~ChaosFragments() {};

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Reset() override;

	void Drop(float dt);
	void GetChaos(Player* player);
	void SetPosition(Vector2f pos);
};

