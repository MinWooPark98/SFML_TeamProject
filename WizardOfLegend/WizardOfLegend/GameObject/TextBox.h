#pragma once
#include "TextObj.h"
#include <functional>

class TextBox : public TextObj
{
protected:
	string str;

public:
	TextBox();
	virtual ~TextBox();

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	virtual void SetActive(bool active) override;
	
	virtual void SetString(string str) override;
	virtual string GetString() override;
	function<void()> SendWritten;
	void FinishWrite();
};

