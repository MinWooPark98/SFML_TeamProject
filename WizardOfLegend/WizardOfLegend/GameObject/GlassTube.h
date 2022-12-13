#pragma once
#include "../GameObject/Interactive/Interactive.h"

class SpriteObj;
class NpcTalkTable;
class GlassTube : public Interactive
{
protected:
	bool isPlayerAdjacent = false;
	vector<string> messages;
	string name;

public:
	GlassTube();
	virtual ~GlassTube();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	bool GetIsPlayerAdjacent() { return isPlayerAdjacent; };
	void SetIsPlayerAdjacent(bool set) { isPlayerAdjacent = set; };

	void SetMessage(string m) { messages.push_back(m); };
	vector<string> GetMessages() const { return messages; };
	void SetNpcName(string m) { name = m; };
	string GetName() const { return name; };

	void SetGlass(NpcTalkTable* table, string image, string npcName);
};

