#pragma once
#include "Object.h"
#include "../Framework/ObjectPool.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class DrawObj;

class Sector : public Object
{
protected:
	Shape* sector;
	string type;
	string path;
	EditorObjs data;

	Vector2i startPos;
	Vector2i endPos;

	DrawObj* nowDraw;

	int aliveEnemyCount;
	bool allEnemyDead = false;

public:
	Sector();
	~Sector();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	
	void UpdateNowDraw(float dt, DrawObj* nowDraw);
	void SetType(string type) { this->type = type; }
	void SetPath(string path) { this->path = path; }
	string GetPath() { return path; }
	string GetType() { return type; }
	EditorObjs GetData() { return data; }
	void SetData(EditorObjs d) { data = d; }
	virtual void SetPos(const Vector2f& pos);

	virtual void SetFillColor(Color color) { sector->setFillColor(color); }
	virtual void SetOutlineColor(Color color) { sector->setOutlineColor(color); }
	virtual void SetOutlineThickness(float t) { sector->setOutlineThickness(t); }

	void SetSize(const Vector2f& size) { ((RectangleShape*)sector)->setSize(size); }
	const Vector2f& GetSize() const { return ((RectangleShape*)sector)->getSize(); }

	Shape* GetSectorShape() const { return sector; };

	int GetAliveEnemyCount() { return aliveEnemyCount; }
	void SetAliveEnemyCount(int count) { aliveEnemyCount = count; }

	bool GetAllEnemyDead() { return allEnemyDead; }
	void SetAllEnemyDead(bool alldead) { allEnemyDead = alldead; }
};

