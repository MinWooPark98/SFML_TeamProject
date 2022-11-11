#pragma once
#include "Object.h"
#include <functional>

class Button : public Object
{
protected:
	bool isSelected;

public:
	Button();
	virtual ~Button();

	function<void()> Selected;
	function<void()> Deselected; 
};

