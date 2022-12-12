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
	Ending,
	Dev,
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
		Middle,
		Wall,
		Object,
		Sector,
		Cliff,
		SpawnArea,
		Count,
	};

protected:
	Scenes type;

	map<LayerType, map<int, vector<Object*>>> objList;

	ObjectPool<Projectile>* projectiles;
	ObjectPool<CastingCircle>* circles;

	bool isPause;
	Vector2f objMousePos;
	Vector2f uiMousePos;

	View worldView;
	View uiView;

	UiMgr* uiMgr;

	vector<vector<Object*>> drawSortObjs;

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
	ObjectPool<Projectile>* GetProjectiles() { return projectiles; }
	ObjectPool<CastingCircle>* GetCastingCircles() { return circles; }

	Scenes GetType() { return type; }

	Vector2f GetWorldViewCenter() const { return worldView.getCenter(); }
	Vector2f GetUiViewCenter() const { return uiView.getCenter(); }
	void SetWorldViewCenter(float x, float y) { worldView.setCenter({x, y}); }
	void SetUiViewCenter(float x, float y) { uiView.setCenter({x, y}); }

	void DrawSort(vector<Object*>& drawSortObjs);
};