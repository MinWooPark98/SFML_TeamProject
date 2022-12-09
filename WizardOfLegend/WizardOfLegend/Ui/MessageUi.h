#pragma once
#include "../GameObject/SpriteObj.h"

class TextObj;
class MessageUi : public SpriteObj
{
protected:
	vector<SpriteObj*> massageImages;
	vector<TextObj*> texts;
	TextObj* npcName;

	SpriteObj* spaceBarImage;
	int spaceBright = 0;

	float enabledTime = 0.1f;
	bool isTalk = false;

	Vector2f windowSize;

	Vector2f textXY;

	// 텍스트 줄 수 받아와서 int로 사이즈 지정, 대화 할 때마다 --, 0 되면 대화 안 되게

public:
	MessageUi();
	virtual ~MessageUi();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void UiEnabled(bool set);
	void SetTexts(vector<string> strings);
	void SetNpcName(string name);
	void SetNpcImage(string imageName); // 여기서 npc 이미지 크기 적용
	void SetPlayerImage(string imageName); // 여기서 크기, 플립, 팔레트 적용

	void SetIsTalk(bool set) { isTalk = set; UiEnabled(set); };
};