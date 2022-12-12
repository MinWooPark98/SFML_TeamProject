#pragma once
#include "../SpriteObj.h"

class Animator;
class Player;

class Interactive : public SpriteObj
{
protected:
	Animator* animator;
	bool approached;
	SpriteObj* interactKey;
	Vector2f interactKeyScale;
	Vector2f interactKeyPos;

	Player* player;

public:
	Interactive();
	virtual ~Interactive();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void SetPos(const Vector2f& pos);
	void SetInteractKeyPos(const Vector2f& pos) { interactKeyPos = pos; }

	virtual void Approach();
	virtual void ApproachEnd();
	void DefaultApproach();
	void DefaultApproachEnd();

	void SetPlayer(Player* player) { this->player = player; }
	function<void()> ApproachFunc;
	function<void()> ApproachEndFunc;
	function<void()> Interact;
};
