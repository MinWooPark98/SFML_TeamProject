#include "Collider.h"

Collider::Collider()
{
}

Collider::Collider(Vector2f size, Vector2f pos)
    :Collider()
{
    hitbox.setSize(size);
    position = pos;
    hitbox.setPosition(position);
}

Collider::~Collider()
{
}

bool Collider::Collision(SpriteObj* otherObj)
{
    return true;
}

void Collider::Init()
{
    SetTag(Tag::COLLIDER);
    SetName("collider");
    SetLayer(-1);
}

void Collider::Release()
{
}

void Collider::Update(float dt)
{
}

void Collider::Draw(RenderWindow& window)
{
    window.draw(hitbox);
}

RectangleShape& Collider::GetShape()
{
    return hitbox;
}

void Collider::SetPos(const Vector2f& pos)
{
    position = pos;
    hitbox.setPosition(position);
}

void Collider::SetPos(const float& x,const float& y)
{
    position.x = x;
    position.y = y;
    hitbox.setPosition(position);
}
