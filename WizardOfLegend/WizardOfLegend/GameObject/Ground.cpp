#include "Ground.h"

Ground::Ground()
	: Stable()
{
}

Ground::~Ground()
{
}


void Ground::Init()
{
	index_total = 3;
	SetTag(Tag::GROUND);
	collidable = true;
	interactable = false;
	type = Interaction_Type::NONE;
	imageIdx = idx;
	std::string path = "Resources/Sprite/BG/Town/town_floor_";
	std::string index;

	switch (imageIdx)
	{
	case 1:
		index = "01.png";
		break;
	case 2:
		index = "02.png";
		break;
	case 3:
		index = "03.png";
		break;
	}

	SetName("ground");
	SetTexture(path + index);
	SetSprite();
}

bool Ground::Collision(SpriteObj* otherObj)
{
	return false;
}
