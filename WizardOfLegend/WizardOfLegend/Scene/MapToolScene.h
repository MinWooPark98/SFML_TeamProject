#pragma once
#define WIDTHCNT 150
#define HEIGHTCNT 150
#include "../Scene/Scene.h"

class Button;
class UiMgr;
class DrawObj;
class Sector;
class Cliff;

class MapToolScene : public Scene
{
private:

	map<int, vector<Button*>> grids;
	map<LayerType, map<int, map<int, DrawObj*>>> gridObjs; //Tile, Object, Enermy
	vector<ObjectData> saveObjs;
	Vector2f initMousePos;
	string type;

	LayerType nowType;
	DrawObj* nowDraw;
	bool isNowSectorDraw = false;
	bool isNowCliffDraw = false;
	bool isNowObjDraw = false;

	DrawObj* player;
	Vector2i playerPos;

	//RectangleShape* sector;
	//vector<RectangleShape*> sectors;
	Sector* sector;
	Cliff* cliff;
	map<LayerType, map<int, map<int, Sector*>>> sectors;
	map<LayerType, map<int, map<int, Cliff*>>> cliffs;
	int sectorI;
	int sectorJ;

public:
	MapToolScene();
	virtual ~MapToolScene();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Release() override;
	void SetType(string t);

	void Save();
	void Load(string path);
};