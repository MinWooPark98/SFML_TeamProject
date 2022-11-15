#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "../GameObject/Object.h"
#include "../Framework/ObjectPool.h"

using namespace sf;
using namespace std;

class Projectile;

enum class Scenes
{
	MapTool,
	SkillTool,
	Play,
};

class UiMgr;
class Scene
{
public:
	enum class Layer
	{
		// 추가
		Count,
	};

protected:
	Scenes type;
	vector<list<Object*>*> layOut;
	list<Object*> objList;

	ObjectPool<Projectile>* projectiles;

	bool isPause;
	Vector2f objMousePos;
	Vector2f uiMousePos;

	View worldView;
	View uiView;
	UiMgr* uiMgr;

public:
	Scene(Scenes type);
	virtual ~Scene();

	virtual void Init(); //생성
	virtual void Release();	 //해제

	virtual void Reset() {}

	virtual void Enter();
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

	void AddGameObject(Object* obj);
	void AddGameObjectFirst(Object* obj);
	void DelGameObject(Object* obj);
	Object* FindGameObj(string name);
	UiMgr* GetUiMgr() { return uiMgr; }
	const Vector2f& GetObjMousePos() const { return objMousePos; }
	const Vector2f& GetUiMousePos() const { return uiMousePos; }
	vector<list<Object*>*>& GetLayout() { return layOut; }
	ObjectPool<Projectile>* GetProjectiles() { return projectiles; }
};

