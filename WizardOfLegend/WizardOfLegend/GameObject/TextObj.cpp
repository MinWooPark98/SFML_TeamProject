#include "TextObj.h"
#include "../Framework/Utils.h"

TextObj::TextObj()
{
}

TextObj::~TextObj()
{
}

void TextObj::Init()
{
	Object::Init();
}

void TextObj::Update(float dt)
{
	Object::Update(dt);
}

void TextObj::Draw(RenderWindow& window)
{
	Object::Draw(window);
	window.draw(text);
}

void TextObj::SetPos(Vector2f position)
{
	Object::SetPos(position);
	text.setPosition(position);
}

void TextObj::SetSize(unsigned int size)
{
	text.setCharacterSize(size);
}

void TextObj::SetFont(Font& font)
{
	text.setFont(font);
}

void TextObj::SetText(string str)
{
	text.setString(str);
}

void TextObj::SetText(Font& font, int size, Color color, string str)
{
	text.setFont(font);
	text.setCharacterSize(size);
	text.setString(str);
	text.setFillColor(color);
}

void TextObj::SetString(string str)
{
	text.setString(str);
}

void TextObj::WrapString(int wordLim)
{
	auto str = GetString();
	int i = 0;
	for (string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if ((i % wordLim) == 0)
			it = str.insert(it, '\n');
		++i;
	}
	SetString(str);
}

void TextObj::SetOrigin(Origins origin)
{
	Object::SetOrigin(origin);
	Utils::SetOrigin(text, origin);
}

string TextObj::GetString()
{
	return text.getString();
}

void TextObj::Translate(const Vector2f& pos)
{
	Object::Translate(pos);
	text.setPosition(position);
}

void TextObj::AsciiToUnicode()
{
	static locale loc("");
	auto& facet = use_facet<std::codecvt<wchar_t, char, mbstate_t>>(loc);
	text.setString(wstring_convert<remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(text.getString()));
}
