#pragma once
#include "Scene.h"
#include "../GameObject/Button.h"

class FireBoss;
class Player;
class Sector;
class PlayScene : public Scene
{
protected:
	FireBoss* fireBoss;
	Player* player;
	IntRect mapSize;

	vector<Sector> room;
	vector<map<Object::ObjTypes, list<Object*>>> collisionList;

	list<int> playerRooms;

public:
	PlayScene();
	virtual ~PlayScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	virtual void Enter() override;
	virtual void Exit() override;

	void SpownEnemy(int i);

	vector<map<Object::ObjTypes, list<Object*>>>& GetCollisionList() { return collisionList; }
	void OnCollisionWall(int roomVec, Object* obj);
};

