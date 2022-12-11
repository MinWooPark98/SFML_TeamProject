#pragma once
#include "Object.h"
#include "../Framework/Utils.h"

class SpriteObj : public Object
{
protected:
    Sprite sprite;

    Shader spriteShader;
    Texture texColorTable;
    int spritePaletteIndex;
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

    void SetSpritePaletteIndex(int index) { spritePaletteIndex = index; };
    void SetSpritePaletteSize(int size) { spritePaletteSize = size; };
    void SetSpriteColorTable(string table) { spriteColorTable.loadFromFile(table); };
    void SetSpriteColor(int index)
    {
        spritePaletteIndex = (spritePaletteIndex - index) % spritePaletteSize;
        spriteShader.setUniform("colorTable", spriteColorTable);
        spriteShader.setUniform("paletteIndex", (float)spritePaletteIndex / spritePaletteSize);
    }
    void SetSpritePalette(int index, int size, string table)
    {
        SetSpritePaletteIndex(index);
        SetSpritePaletteSize(size);
        SetSpriteColorTable(table);
    };

    void SetSpriteShader() { spriteShader.loadFromFile("shaders/palette.frag", Shader::Fragment); };
};

