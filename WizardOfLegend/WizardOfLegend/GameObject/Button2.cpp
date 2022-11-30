#include "Button2.h"
#include "SpriteObj.h"
#include "TextObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/SoundMgr.h"

Button2::Button2()
    :sprite(nullptr), text(nullptr), activated(true), isOtherView(false), isMouseOn(false), isClicked(false), origin(Origins::TL)
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
    SetActivated(true);
    UnClicked();
    MouseOff();
}

void Button2::Update(float dt)
{
    Object::Update(dt);
    isClicked = false;
    if (!isOtherView)
    {
        if (isUi)
            mousePos = SCENE_MGR->GetCurrentScene()->GetUiMousePos();
        else
            mousePos = SCENE_MGR->GetCurrentScene()->GetObjMousePos();
    }

    bool mouseOnBtn = btnBound.contains(mousePos);
    if (!isMouseOn)
    {
        if (mouseOnBtn)
            MouseOn();
    }
    else if(!mouseOnBtn)
        MouseOff();

    if (InputMgr::GetMouseButtonDown(Mouse::Left))
    {
        if (mouseOnBtn)
        {
            if(!isClicked)
                Clicked();
        }
        else
            UnClicked();
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

void Button2::FillBoxComplementaryColor()
{
    hitbox.setFillColor(Color(255 - boxInitColor.r, 255 - boxInitColor.g, 255 - boxInitColor.b, 255));
}

void Button2::FillBoxInitColor()
{
    hitbox.setFillColor(Color(boxInitColor.r, boxInitColor.g, boxInitColor.b, 255));
}

void Button2::SetActivated(bool activate)
{
    Color initColor = hitbox.getFillColor();
    if (activate)
    {
        hitbox.setFillColor({ initColor.r, initColor.g, initColor.b, 255 });
        activated = true;
    }
    else
    {
        hitbox.setFillColor({ initColor.r, initColor.g, initColor.b, 175 });
        activated = false;
    }
}

void Button2::MouseOn()
{
    if (!activated)
        return;
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
    if (!activated)
        return;
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
