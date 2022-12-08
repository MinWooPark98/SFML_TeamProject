#include "NPC.h"
#include "../../Framework/Animator.h"

NPC::NPC()
	:animator(nullptr)
{
}

NPC::~NPC()
{
}

void NPC::Update(float dt)
{
	SpriteObj::Update(dt);
	if (animator != nullptr)
		animator->Update(dt);
}
