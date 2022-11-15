#pragma once
#include "../GameObject/Button.h"

class TextObj;
class SaveWindowBox : public Button 
{
private:
	Button* save;
	Button* cancle;
	string path;
	TextObj* txt;
public:
	SaveWindowBox(UiMgr* mgr);
	~SaveWindowBox();
	void Init();
	virtual void Draw(RenderWindow& window);
	virtual void Update(float dt);
	virtual void Release();

	void SetPath(string path);

	virtual bool IsSave();
	virtual bool IsCancle();
	virtual string GetPath() { return path ; }
};

