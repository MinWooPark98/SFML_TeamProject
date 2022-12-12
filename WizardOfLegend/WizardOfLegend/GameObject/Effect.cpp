#include "Effect.h"

Effect::Effect()
{
}

Effect::~Effect()
{
}

void Effect::Init()
{
	SpriteObj::Init();
}

void Effect::Reset()
{
	SpriteObj::Reset();
}

void Effect::Update(float dt)
{
	SpriteObj::Update(dt);
	
	if (animation != nullptr)
		animation->Update(dt);
}

void Effect::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
}
