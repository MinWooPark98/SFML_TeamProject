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

	// �ؽ�Ʈ �� �� �޾ƿͼ� int�� ������ ����, ��ȭ �� ������ --, 0 �Ǹ� ��ȭ �� �ǰ�

public:
	MessageUi();
	virtual ~MessageUi();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void UiEnabled(bool set);
	void SetTexts(vector<string> strings);
	void SetNpcName(string name);
	void SetNpcImage(string imageName); // ���⼭ npc �̹��� ũ�� ����
	void SetPlayerImage(string imageName); // ���⼭ ũ��, �ø�, �ȷ�Ʈ ����

	void SetIsTalk(bool set) { isTalk = set; UiEnabled(set); };
};