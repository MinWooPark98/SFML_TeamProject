#pragma once
#include "../GameObject/Object.h"
#include "../GameObject/Item/Hood.h"
#include "../Framework/ObjectPool.h"

class SpriteObj;
class TextObj;

class WardrobeUi : public Object
{
public:
	struct Collection
	{
		SpriteObj* backImg;
		SpriteObj* image;
		bool unlocked;
		Hood::HoodInfo info;
	};
protected:
	SpriteObj* frame;
	SpriteObj* display;
	vector<string> actions;
	int actionIdx;
	float angle;
	float currAngle;

	SpriteObj* panel;
	vector<Collection> collections;
	int vecIdx;
	int lastIdx;

	TextObj* itemName;
	TextObj* itemIntro;

	//vector<pair<SpriteObj*, TextObj*>> keyInfos;

	bool isMoving;
	float moveSpeed;

public:
	WardrobeUi();
	virtual ~WardrobeUi();

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
	void FinishRoate();
	void ChangeHood();

	void CollectionHighLightOn();
	void CollectionHighLightOff();
};

