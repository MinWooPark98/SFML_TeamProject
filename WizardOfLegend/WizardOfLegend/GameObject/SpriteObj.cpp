#include "SpriteObj.h"
#include "../Scene/SceneMgr.h"

SpriteObj::SpriteObj()
{
}

SpriteObj::~SpriteObj()
{
}

void SpriteObj::Init()
{
}

void SpriteObj::Update(float dt)
{
    Object::Update(dt);
}

void SpriteObj::Draw(RenderWindow& window)
{
    if (spriteShader != nullptr)
        window.draw(sprite, &(*spriteShader));
    else
        window.draw(sprite);
    Object::Draw(window);
}

void SpriteObj::SetTexture(const Texture& tex)
{
    sprite.setTexture(tex, true);
}

void SpriteObj::SetOrigin(Origins origin)
{
    Object::SetOrigin(origin);
    Utils::SetOrigin(sprite, origin);
}

void SpriteObj::SetColor(Color color)
{
    sprite.setColor(color);
}
Color SpriteObj::GetColor()
{
    return Color();
}
Vector2f SpriteObj::GetSize() const
{
    FloatRect rect = sprite.getLocalBounds();

    return Vector2f(rect.width, rect.height);
}

void SpriteObj::SetPos(const Vector2f& pos)
{
    Object::SetPos(pos);
    sprite.setPosition(position);
}

void SpriteObj::SetTextureRect(const IntRect& rect)
{
    sprite.setTextureRect(rect);
}
void SpriteObj::SetSize(Vector2f size)
{
    auto localSize = sprite.getLocalBounds();
    sprite.setScale({ size.x / localSize.width, size.y / localSize.height });
}
void SpriteObj::SetScale(Vector2f scale)
{
    sprite.setScale(scale);
}

const IntRect& SpriteObj::GetTextureRect() const
{
    return sprite.getTextureRect();
}

void SpriteObj::SetFlipX(bool flip)
{
    Vector2f scale = sprite.getScale();
    scale.x = flip ? -abs(scale.x) : abs(scale.x);
    sprite.setScale(scale);
}

void SpriteObj::SetFlipY(bool flip)
{
    Vector2f scale = sprite.getScale();
    scale.y = flip ? -abs(scale.y) : abs(scale.y);
    sprite.setScale(scale);
}

FloatRect SpriteObj::GetGlobalBounds() const
{
    return sprite.getGlobalBounds();
}
FloatRect SpriteObj::GetLocalBounds() const
{
    return sprite.getLocalBounds();
}

void SpriteObj::UseShader()
{
    spriteShader = new Shader();
}
