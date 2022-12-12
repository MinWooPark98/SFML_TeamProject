#pragma once
#include "SpriteObj.h"
#include "Interactive/Goods.h"

class Store : public SpriteObj
{
protected:
	Goods::Payment payment;
	Goods::Types goodsType;

	vector<Goods*> goodsList;

public:
	Store(Goods::Payment payment, Goods::Types goodsType);
	virtual ~Store();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void SetPos(const Vector2f& pos) override;
	virtual void SwitchDevMode() override;
};

