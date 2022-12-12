#pragma once
#include "Object.h"
#include "../Framework/Utils.h"

class SpriteObj : public Object
{
protected:
    Sprite sprite;

    Shader* spriteShader;
    Texture texColorTable;
    int spritePaletteSize;
    Texture spriteColorTable;

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

    Color GetColor();
    Sprite& GetSprite() { return sprite; }
    void SetUI(bool u) { isUi = u; }
    void SetTextureRect(const IntRect& rect);
    const IntRect& GetTextureRect()const;
    void SetLowHitBoxOrigin(Origins origin) { Utils::SetOrigin(lowhitbox, origin); }
    void Rotate(float rotation) { sprite.rotate(rotation); }

    void SetFlipX(bool flip);
    void SetFlipY(bool flip);

    FloatRect GetGlobalBounds() const;
    FloatRect GetLocalBounds() const;

    void SetSpritePaletteSize(int size) { spritePaletteSize = size; };
    void SetSpriteColorTable(string table) { spriteColorTable.loadFromFile(table); };
    void SetPaletteColor(int index)
    {
        float spritePaletteIndex = index % spritePaletteSize;
        spriteShader->setUniform("colorTable", spriteColorTable);
        spriteShader->setUniform("paletteIndex", spritePaletteIndex / spritePaletteSize);
    }
    void SetSpritePalette(int size, string table)
    {
        SetSpritePaletteSize(size);
        SetSpriteColorTable(table);
    };

    void SetSpriteShader() 
    { 
        if (spriteShader != nullptr)
            spriteShader->loadFromFile("shaders/palette.frag", Shader::Fragment); 
    };
    void UseShader();
};

