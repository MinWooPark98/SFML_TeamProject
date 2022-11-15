#pragma once
#include "Object.h"
#include "../Framework/Utils.h"

class SpriteObj :public Object
{
protected:
    Sprite sprite;

public:
    SpriteObj();
    virtual ~SpriteObj();

    virtual void Init() override;
    virtual void Update(float dt) override;
    virtual void Draw(RenderWindow& window) override;

    void SetTexture(const Texture& tex);
    void SetOrigin(Origins origin);
    void SetColor(Color color);
    void SetScale(Vector2f scale);

    Vector2f GetSize()const;
    void SetSize(Vector2f size);
    
    virtual void SetPos(const Vector2f& pos) override;

    void SetTexture(Texture& tex);
    Color GetColor();
    Sprite& GetSprite() { return sprite; }
    void SetUI(bool u) { isUi = u; }
    bool IsInView();
    bool GetIsView() { return viewIn; }
    void SetTextureRect(const IntRect& rect);
    const IntRect& GetTextureRect()const;
    void SetHitBoxOrigin(Origins origin) { Utils::SetOrigin(hitbox, origin); }
    void SetHitBoxOrigin(const Vector2f& origin) { hitbox.setOrigin(origin); }
    void Rotate() { sprite.rotate(rotation); }

    void SetFlipX(bool flip);
    void SetFlipY(bool flip);

    FloatRect GetGlobalBounds() const;
    FloatRect GetLocalBounds() const;
};
