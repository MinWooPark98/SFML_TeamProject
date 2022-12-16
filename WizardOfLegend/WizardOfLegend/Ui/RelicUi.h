#pragma once
#include "../GameObject/Object.h"
#include "../GameObject/Item/Relic.h"

class Button2;
class SpriteObj;
class TextObj;

class RelicUi : public Object
{
protected:
	vector<pair<Relic::RelicInfo, Button2*>> display;

	SpriteObj* infoPanel;
	TextObj* relicName;
	TextObj* relicInfo;

public:
	RelicUi();
	virtual ~RelicUi();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void AddRelicUi(const Relic::RelicInfo& info);
	void ChangeRelicUi(const Relic::RelicInfo& info, int idx);

	void Reposition();
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Translate(const Vector2f& delta) override;

	void ShowInfo(int idx);
	void HideInfo();
};

