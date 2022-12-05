#include "Object.h"
#include "../Scene/SceneMgr.h"
#include "../Framework/InputMgr.h"

int Object::objCount = 0;

Object::Object()
    :isDevMode(false), rotation(0.f), enabled(true), gravity(0.f), gravityApply(false), direction(1.f, 0.f)
{
    id = ++objCount;
}

Object::~Object()
{
    Release();
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
    hitbox.setPosition(position);
    lowhitbox.setPosition({ position.x, position.y + (hitbox.getSize().y * 0.5f) + (lowhitbox.getSize().y * 0.5f)});
}

const Vector2f& Object::GetPos() const
{
    return position;
}

void Object::SetOrigin(Origins origin)
{
    Utils::SetOrigin(hitbox, origin);
}

void Object::Update(float dt)
{
    if (gravityApply)
        direction.y += dt * gravity;
    if (!(objtype == ObjTypes::Wall))
    {
        hitbox.setPosition(position);
        lowhitbox.setPosition({ position.x, position.y + (hitbox.getSize().y * 0.5f) + (lowhitbox.getSize().y * 0.5f) });
    }
    /*if (InputMgr::GetKeyDown(Keyboard::F1))
    {
        isDevMode = !isDevMode;
    }*/
}


void Object::Draw(RenderWindow& window)
{
    if (isDevMode)
    {
        window.draw(hitbox);
        window.draw(lowhitbox);
    }
}

void Object::SetHitBox(const FloatRect& rect, Color color)
{
    hitbox.setSize({ rect.width,rect.height });
    hitbox.setFillColor(color);
}

void Object::SetHitBox(string path)
{
    auto hitData = FILE_MGR->GetHitBox(path);

    for (auto& hit : hitData)
    {        
        SetHitBox(FloatRect(hit.pos.x, hit.pos.y, hit.size.x, hit.size.y));
        hitbox.setPosition(GetPos()+ hit.pos);
        Utils::SetOrigin(hitbox,Origins::BC);
    }
}

void Object::Translate(const Vector2f& delta)
{
    SetPos({ position.x + delta.x, position.y + delta.y });
}