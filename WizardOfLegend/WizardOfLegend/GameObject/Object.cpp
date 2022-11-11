#include "Object.h"

int Object::objCount = 0;

Object::Object()
    :isDevMode(false), rotation(0.f), enabled(true), gravity(0.f), gravityApply(false), direction(0.f, 0.f), isVisible(true), layer(0), tag(Tag::NONE)
{

    id = ++objCount;
}

Object::~Object()
{
    Release();
}

int Object::GetObjId() const
{
    return id;
}

void Object::SetActive(bool active)
{
    enabled = active;
}

bool Object::GetActive() const
{
    return enabled;
}

void Object::Init()
{
    Reset();
}

void Object::Release()
{
}

void Object::Reset()
{
    enabled = true;
}

void Object::SetPos(const Vector2f& pos)
{
    position = pos;
    //hitbox.setPosition(position);
}
void Object::SetPos(const float& x, const float& y)
{
    position.x = x;
    position.y = y;
    //hitbox.setPosition(position);
}

const Vector2f& Object::GetPos() const
{
    return position;
}

void Object::Update(float dt)
{
    if (gravityApply)
        direction.y += dt * gravity;
    hitbox.setPosition(position);
}


void Object::Draw(RenderWindow& window)
{
    if (isDevMode)
    {
        window.draw(hitbox);
    }
}

void Object::SetHitBox(const FloatRect rect)
{
    hitbox.setSize({ rect.width,rect.height });
    hitbox.setFillColor({ 255, 0, 0, 255 });
}

RectangleShape Object::GetHitBox()
{
    return hitbox;
}

void Object::SetHidden()
{
    isVisible = false;
}

void Object::SetVisible()
{
    isVisible = true;
}

bool Object::GetIsVisible()
{
    return false;
}

void Object::SetLayer(int layer)
{
    this->layer = layer;
}

int Object::Getlayer()
{
    return layer;
}

void Object::SetTag(Tag tag)
{
    this->tag = tag;
}

Tag Object::GetTag()
{
    return tag;
}

bool Object::CompareTag(Tag tag)
{
    if (this->tag == tag)
        return true;
    return false;
}


void Object::Translate(const Vector2f& delta)
{
    SetPos({ position.x + delta.x, position.y + delta.y });
}

