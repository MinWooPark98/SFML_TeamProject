#include "Stable.h"

Stable::Stable()
	: SpriteObj()
{
}

Stable::~Stable()
{
}

bool Stable::IsCollidable()
{
	return collidable;
}

bool Stable::IsInteractable()
{
	return interactable;
}

void Stable::SetInteractable(bool is)
{
	interactable = is;
}

Interaction_Type Stable::GetInteractionType()
{
	return type;
}

void Stable::Interaction(Player& player)
{
	return;
}

int Stable::GetIndexTotal()
{
	return index_total;
}

void Stable::Init()
{
	collidable = false;
	interactable = false;
	type = Interaction_Type::NONE;
}

void Stable::Draw(RenderWindow& window)
{
	window.draw(this->sprite);
}

void Stable::Update(float dt)
{
}

bool Stable::Collision(SpriteObj* otherObj)
{
	return false;
}

void Stable::Release()
{
}

InvisibleWall::InvisibleWall(float x, float y)
{
	SetTag(Tag::GROUND);
	SetSize(10.f, 1000.f);
	collidable = true;
	interactable = false;
	type = Interaction_Type::NONE;

	wallShape.setPosition(x, y);
}

InvisibleWall::~InvisibleWall()
{
}

void InvisibleWall::SetSize(Vector2f size)
{
	wallShape.setSize(size);
}

void InvisibleWall::SetSize(float x, float y)
{
	wallShape.setSize(Vector2f(x, y));
}
