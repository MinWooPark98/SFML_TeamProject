#pragma once
#include "Goods.h"

class GoodsSkill : public Goods
{
protected:
	SpriteObj card;
	SpriteObj skillIcon;

	string skillKey;

public:
	GoodsSkill(Payment payment);
	virtual ~GoodsSkill();

	virtual void Init();
	virtual void Draw(RenderWindow& window);

	void SetInfo(const string& name);
	virtual void Saled() override;

	virtual void SetPos(const Vector2f& pos) override;
};

