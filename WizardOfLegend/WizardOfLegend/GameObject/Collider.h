#pragma once
#include "SpriteObj.h"

class Collider :  public SpriteObj
{
public:
	Collider();
	Collider(Vector2f size, Vector2f pos);
	virtual ~Collider();

	virtual bool Collision(SpriteObj* otherObj);

	virtual void Init();
	virtual void Release();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	RectangleShape& GetShape();
	virtual void SetPos(const Vector2f& pos) override;
	virtual void SetPos(const float& x,const float& y) override;
};

