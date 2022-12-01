#pragma once
#include "Object.h"
#include "../Framework/ObjectPool.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class DrawObj;

class Cliff : public Object
{
protected:
	Shape* cliff;
	string type;
	string path;
	EditorObjs data;

	Vector2i startPos;
	Vector2i endPos;

	DrawObj* nowDraw;

	bool isPlayerInToTheRoom;

public:
	Cliff();
	~Cliff();

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

	virtual void SetFillColor(Color color) { cliff->setFillColor(color); }
	virtual void SetOutlineColor(Color color) { cliff->setOutlineColor(color); }
	virtual void SetOutlineThickness(float t) { cliff->setOutlineThickness(t); }

	void SetSize(const Vector2f& size) { ((RectangleShape*)cliff)->setSize(size); }
	const Vector2f& GetSize() const { return ((RectangleShape*)cliff)->getSize(); }
};

