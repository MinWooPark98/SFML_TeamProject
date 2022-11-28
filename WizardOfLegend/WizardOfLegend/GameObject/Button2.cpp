#include "Button2.h"
#include "SpriteObj.h"
#include "TextObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/SoundMgr.h"

Button2::Button2()
    :sprite(nullptr), text(nullptr), isMouseOn(false), isClicked(false), origin(Origins::TL)
{
}

Button2::~Button2()
{
}

void Button2::Init()
{
    Object::Init();
}

void Button2::Release()
{
    Object::Release();
}

void Button2::Reset()
{
    Object::Reset();
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
    if (text != nullptr)
        text->SetString("");
}

void Button2::Update(float dt)
{
    Object::Update(dt);
    isClicked = false;
    Vector2f mousePos;
    if (isUi)
        mousePos = SCENE_MGR->GetCurrentScene()->GetUiMousePos();
    else
        mousePos = SCENE_MGR->GetCurrentScene()->GetObjMousePos();
    if (!isMouseOn)
    {
        if (btnBound.contains(mousePos))
            MouseOn();
    }
    else
    {
        if (!btnBound.contains(mousePos))
            MouseOff();
        else if (InputMgr::GetMouseButtonDown(Mouse::Left))
            Clicked();
    }
}

void Button2::Draw(RenderWindow& window)
{
    Object::Draw(window);
    if (sprite != nullptr)
        sprite->Draw(window);
    if (text != nullptr)
        text->Draw(window);
}

void Button2::SetButtonBox(const FloatRect& rect, Color fillColor, Color outLineColor, float outLineThickness)
{
    hitbox.setSize({ rect.width,rect.height });
    hitbox.setFillColor(fillColor);
    hitbox.setOutlineColor(outLineColor);
    hitbox.setOutlineThickness(outLineThickness);
    btnBound = GetHitBounds();
}

void Button2::UseSprite()
{
    sprite = new SpriteObj();
    sprite->Init();
}

void Button2::SetSprite(const string& texName)
{
    sprite->SetTexture(*RESOURCE_MGR->GetTexture(texName));
    sprite->SetOrigin(Origins::BC);
    Reposition();
}

void Button2::UseText()
{
    text = new TextObj();
    text->Init();
}

void Button2::SetText(const string& fontName, int charSize, Color textColor, String string)
{
    text->SetFont(*RESOURCE_MGR->GetFont(fontName));
    text->SetSize(charSize);
    text->SetFillColor(textColor);
    textInitColor = textColor;
    text->SetString(string);
    text->SetOrigin(Origins::MC);
    Reposition();
}

void Button2::SetPos(const Vector2f& pos)
{
    Object::SetPos(pos);
    btnBound = GetHitBounds();
    Reposition();
}

void Button2::SetOrigin(Origins origin)
{
    this->origin = origin;
    Object::SetOrigin(origin);
    btnBound = GetHitBounds();
    Reposition();
}

void Button2::Reposition()
{
    if (sprite != nullptr)
    {
        sprite->SetOrigin(Origins::BC);
        sprite->SetPos({ btnBound.left + btnBound.width * 0.5f, btnBound.top + btnBound.height });
    }
    if (text != nullptr)
    {
        text->SetOrigin(Origins::MC);
        text->SetPos({ btnBound.left + btnBound.width * 0.5f, btnBound.top + btnBound.height * 0.5f });
    }
}

void Button2::DefaultMouseOn()
{
    if (sprite != nullptr)
        sprite->SetColor({ 255, 255, 255, 153 });
    if (text != nullptr)
        text->SetFillColor(Color(255 - textInitColor.r, 255 - textInitColor.g, 255 - textInitColor.b, 255));
}

void Button2::DefaultMouseOff()
{
    if (sprite != nullptr)
        sprite->SetColor({ 255, 255, 255, 255 });
    if (text != nullptr)
        text->SetFillColor(textInitColor);
}

void Button2::ChangeFillColor()
{
    Color originalColor = hitbox.getFillColor();
    hitbox.setFillColor(Color(255 - originalColor.r, 255 - originalColor.g, 255 - originalColor.b, 255));
}

void Button2::MouseOn()
{
    isMouseOn = true;
    if (MousePointerOn != nullptr)
    {
        MousePointerOn();
        SOUND_MGR->Play("sounds/Chime.wav");
    }
}

void Button2::MouseOff()
{
    isMouseOn = false;
    if (MousePointerOff != nullptr)
        MousePointerOff();
}

void Button2::Clicked()
{
    isClicked = true;
    if (ClickOn != nullptr)
        ClickOn();
}

void Button2::UnClicked()
{
    isClicked = false;
    if (ClickOff != nullptr)
        ClickOff();
}
