#pragma once
#include "Object.h"
#include "../Framework/ObjectPool.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class DrawObj;

class SpawnArea : public Object
{
protected:
	Shape* spawnArea;
	string type;
	string path;
	EditorObjs data;

	Vector2i startPos;
	Vector2i endPos;

	DrawObj* nowDraw;

public:
	SpawnArea();
	~SpawnArea();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Release() override;

	void UpdateNowDraw(float dt, DrawObj* nowDraw);
	void SetType(string type) { this->type = type; }
	void SetPath(string path) { this->path = path; }
	string GetPath() { return path; }
	string GetType() { return type; }
	EditorObjs GetData() { return data; }
	void SetData(EditorObjs d) { data = d; }
	virtual void SetPos(const Vector2f& pos);

	virtual void SetFillColor(Color color) { spawnArea->setFillColor(color); }
	virtual void SetOutlineColor(Color color) { spawnArea->setOutlineColor(color); }
	virtual void SetOutlineThickness(float t) { spawnArea->setOutlineThickness(t); }

	void SetSize(const Vector2f& size) { ((RectangleShape*)spawnArea)->setSize(size); }
	const Vector2f& GetSize() const { return ((RectangleShape*)spawnArea)->getSize(); }

	Shape* GetSpawnAreaShape() const { return spawnArea; };



};



