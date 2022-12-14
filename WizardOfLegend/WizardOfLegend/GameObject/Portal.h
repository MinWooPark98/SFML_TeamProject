#pragma once
#include "../GameObject/SpriteObj.h"

class Scene;
class Player;
class Portal : public SpriteObj
{
public:
	enum class PortalType
	{
		None = -1,
		ChangePos,
		ChangeMap,
	};

	enum class PortalName
	{
		None = -1,
		FirstTutorial,
		FightTutorial,
		Square,
		PlayMap,
	};

protected:
	Scene* currScene;
	PortalType portalType;
	Player* player;
	PortalName portalName;

	bool isPortalCreat = false;

public:
	Portal();
	virtual ~Portal();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void ChanegePlayerPos(const Vector2f& pos);
	void SetChanegeMap(string mapName);

	void ChangeMap();
	void SetPortalName(PortalName name) { portalName = name; };
	void SetPlayer(Player* player);

	void PortalCreat() { isPortalCreat = true; };
	void PortalEnabled() { isPortalCreat = false; };
};

