#pragma once
#include "../GameObject/Object.h"
#include "../GameObject/Item/Item.h"

class KeyButton;
class SpriteObj;
class TextObj;

class ItemBoxUi : public Object
{
protected:
	SpriteObj* frame;
	SpriteObj* panel;

	vector<vector<vector<pair<KeyButton*, pair<bool, Item::Info>>>>> collections;
	int currPage;
	int currRow;
	int currColumn;
	bool indexChanged;

	KeyButton* currPlayerItem;

	TextObj* itemName;
	TextObj* itemIntro;

	vector<pair<SpriteObj*, TextObj*>> keyInfos;

	bool isMoving;
	float moveSpeed;

public:
	ItemBoxUi();
	virtual ~ItemBoxUi();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void Reposition();
	virtual void SetPos(const Vector2f& pos) override;
	virtual void Translate(const Vector2f& delta) override;
	virtual void SetActive(bool active) override;
	void Reappear();
	void Disappear();

	void SetItemInfo();
	void SelectItem();

	void CollectionHighLightOn();
	void CollectionHighLightOff();
};

