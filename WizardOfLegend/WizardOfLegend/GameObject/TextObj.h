#pragma once
#include "Object.h"
#include "../Framework/Utils.h"

class TextObj : public Object
{
protected:
	Text text;

public:
	TextObj();
	virtual ~TextObj();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void SetPos(Vector2f position);

	virtual Text& GetSFMLText() { return text; }
	virtual void SetSize(unsigned int size);
	virtual void SetFillColor(Color color) { text.setFillColor(color); }
	virtual void SetOutlineColor(Color color) { text.setOutlineColor(color); }
	virtual void SetOutlineThickness(float t) { text.setOutlineThickness(t); }
	virtual void SetFont(Font& font);
	virtual void SetText(string text);
	Text& GetText() { return text; }
	void SetText(Font& font, int size, Color color, string str);
	virtual void SetString(string str);
	virtual string GetString();
	virtual void SetOrigin(Origins origin);
	virtual void Translate(const Vector2f& pos) override;
	FloatRect GetGlobalBounds() { return text.getGlobalBounds(); }

	void AsciiToUnicode();	// 한글 깨질 때 사용
};

