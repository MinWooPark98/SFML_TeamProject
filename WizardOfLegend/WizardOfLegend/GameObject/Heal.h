#pragma once
#include "../GameObject/SpriteObj.h"
#include "../Framework/Animator.h"

class Player;
class Heal : public SpriteObj
{
protected:
	Player* player;

public:
	Heal() {};
	virtual ~Heal() {};

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Reset() override;

	void OnHit(int dmg, int heal);

	void SetPlayer(Player* p) { player = p; };
};

