#pragma once
#include "Button.h"

class TextObj;

class TextButton : public Button
{
protected:
	TextObj* text;

public:
	TextButton();
	virtual ~TextButton();


};

