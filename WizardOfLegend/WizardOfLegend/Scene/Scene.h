#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "../GameObject/Object.h"
#include "../Framework/ObjectPool.h"

using namespace sf;
using namespace std;

class Projectile;
class CastingCircle;

enum class Scenes
{
	Title,
	Play,
	MapTool,
	SkillTool,
};

class UiMgr;
class Scene
{
public:
	enum class LayerType
	{
		None = -1,
		Back,
		Tile,
		Object,
		Sector,
		Count,
	};

protected:
	Scenes type;

	vector<list<Object*>*> layOut;
	map<LayerType, map<int, vector<Object*>>> objList;

	ObjectPool<Projectile>* projectiles;
	ObjectPool<CastingCircle>* circles;

	bool isPause;
	Vector2f objMousePos;
	Vector2f uiMousePos;

	View worldView;
	View uiView;

	UiMgr* uiMgr;

	vector<Object*> moves;
	vector<Object*> drawObjs;
	vector<Object*> alphaObj;
	bool isMap;

public:
	Scene(Scenes type);
	virtual ~Scene();

	virtual void Init(); //생성
	virtual void Release();	 //해제

	virtual void Reset() {}
	virtual void Enter() {}
	virtual void Exit();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	Texture* GetTexture(const string path) const;

	void SetPause(bool pause) { isPause = pause; }
	bool GetPause() const { return isPause; }

	void SetWorldViewCenter(const Vector2f& view) { worldView.setCenter(view.x, view.y); }
	View& GetWorldView() { return worldView; }
	View& GetUiView() { return uiView; }

	Vector2f ScreenToWorld(Vector2i screenPos);
	Vector2f ScreenToUiPosition(Vector2i screenPos);

	void AddGameObject(Object* obj, LayerType type, int num);
	Object* FindGameObj(string name);
	UiMgr* GetUiMgr() { return uiMgr; }
	const Vector2f& GetObjMousePos() const { return objMousePos; }
	const Vector2f& GetUiMousePos() const { return uiMousePos; }
	vector<list<Object*>*>& GetLayout() { return layOut; }
	ObjectPool<Projectile>* GetProjectiles() { return projectiles; }
	ObjectPool<CastingCircle>* GetCastingCircles() { return circles; }
	//virtual void LayerSort();
};
//bool sorting(Object* p1, Object* p2);