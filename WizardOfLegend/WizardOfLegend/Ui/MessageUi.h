#pragma once
#include "../GameObject/SpriteObj.h"

class TextObj;
class MessageUi : public SpriteObj
{
protected:
	vector<SpriteObj*> massageImages;
	TextObj* text;
	TextObj* npcName;

	SpriteObj* spaceBarImage;
	int spaceBright = 0;

	float enabledTime = 0.1f;
	bool isTalk = false;

	Vector2f windowSize;
	RectangleShape playerShader;

	vector<string> names;
	vector<string> tables;

public:
	MessageUi();
	virtual ~MessageUi();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Release() override;

	void UiEnabled(bool set);
	void SetTexts(const string& string);
	void SetNpcName(const string& name);
	void SetNpcImage(const string& imageName); // 여기서 npc 이미지 크기 적용
	void SetPlayerImage(int playerPaletteColor);

	void SetIsTalk(bool set) { isTalk = set; UiEnabled(set); };
	bool GetisTalk() const { return isTalk; };

	void MessageUiOn();
	void Talk();

	void SetTalkTable(string npcName ,vector<string> npcTalk);
};