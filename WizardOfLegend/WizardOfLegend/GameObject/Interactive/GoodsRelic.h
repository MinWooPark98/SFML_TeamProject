#pragma once
#include "Goods.h"
#include "../Item/Relic.h"

class GoodsRelic : public Goods
{
protected:
	SpriteObj relicDisplay;

	Relic::RelicInfo info;

public:
	GoodsRelic(Payment payment);
	virtual ~GoodsRelic();

	virtual void Init();
	virtual void Draw(RenderWindow& window);

	void SetInfo(const Relic::RelicInfo& info);
	virtual void Saled() override;

	virtual void SetPos(const Vector2f& pos) override;
};

