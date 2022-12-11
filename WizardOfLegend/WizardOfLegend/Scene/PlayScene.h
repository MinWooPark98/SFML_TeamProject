#pragma once
#include "Scene.h"
#include "../GameObject/Button.h"

class FireBoss;
class Player;
class Sector;
class FinalBoss;
class HeavyBombingArcher;
class ShowDamage;
class Gold;
class ChaosFragments;
class Portal;
class Heal;
class HitSpark;
class GlassTube;

class PlayScene : public Scene
{
protected:
	FireBoss* fireBoss;
	FinalBoss* finalBoss;
	HeavyBombingArcher* heavyBombingArcher;
	Player* player;
	IntRect mapSize;

	vector<Sector> room;
	vector<map<Object::ObjTypes, list<Object*>>> collisionList;

	list<int> playerRooms;

	float currSpownDelay = 5.f;
	float maxSpownDelay = 5.f;

	bool hpBarSet = true;

	string mapName;

	ObjectPool<ShowDamage>* showDamages;
	ObjectPool<Gold>* golds;
	ObjectPool<ChaosFragments>* platinums;
	ObjectPool<HitSpark>* hitSparks;

	Portal* portal;
	GlassTube* glassTube;

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

	vector<map<Object::ObjTypes, list<Object*>>>& GetCollisionList() { return collisionList; }
	void OnCollisionWall(int roomVec, Object* obj);
	void SpawnEnemy(int i, float dt);
	void AllDieEnemy(int i);
	void OnCollisionETC(int roomVec, Object* obj);
	ObjectPool<ShowDamage>* GetShowDamage() { return showDamages; }
	ObjectPool<Gold>* GetGold() { return golds; }
	ObjectPool<ChaosFragments>* GetPlatinum() { return platinums; }
	ObjectPool<HitSpark>* GetHitSpark() { return hitSparks; }

	void SetMapName(const string& name) { mapName = name; }
	string GetMapName() { return mapName; };
};

