#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

enum class Tag
{
	NONE,
	PLAYER,
	MONSTER,
	COLLIDER,
	GROUND,
	BACKGROUND,
	TRAP,
	BREAKABLE,
};

class Object
{
protected:
	int id;
	string name;
	int layer;
	Tag tag;

	bool enabled;
	bool isVisible;

	Vector2f position;
	Vector2f direction;
	float rotation;
	Vector2f scale;
	
	float gravity;
	bool gravityApply;
	
	static int objCount;

	RectangleShape hitbox;
	bool isDevMode;

public:
	Object();
	virtual ~Object();

	int GetObjId() const;

	virtual void SetActive(bool active);
	virtual bool GetActive() const;

	virtual void Init();
	virtual void Release();

	virtual void Reset();

	virtual void SetPos(const Vector2f& pos);
	virtual void SetPos(const float& x, const float& y);
	virtual const Vector2f& GetPos() const;
	virtual void Translate(const Vector2f& delta);

	void SetGravity(float g) { gravity = g; }
	void SetGravityApply(bool apply) { gravityApply = apply; }
	bool GetGravityApply() const { return gravityApply; }

	virtual void SetRotation(float rotation) { this->rotation = rotation; }

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	void SetName(string name) { this->name = name; }
	const string& GetName() const { return name; }

	void SetDirection(const Vector2f& dir) { direction = dir; }
	const Vector2f& GetDirection() const { return direction; }

	virtual void SetDevMode(bool devMode) { isDevMode = devMode; }
	virtual bool GetDevMode() const { return isDevMode; }
	virtual void SwitchDevMode() { isDevMode = !isDevMode; }

	virtual void SetHitBox(const FloatRect rect);
	virtual RectangleShape GetHitBox();

	FloatRect GetHitBounds() const { return hitbox.getGlobalBounds(); }

	void SetHidden();
	void SetVisible();
	bool GetIsVisible();

	void SetLayer(int layer);
	int Getlayer();

	void SetTag(Tag tag);
	Tag GetTag();
	bool CompareTag(Tag tag);
	
};