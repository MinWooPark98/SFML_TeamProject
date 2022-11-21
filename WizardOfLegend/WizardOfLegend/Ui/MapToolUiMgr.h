#pragma once
#include "UiMgr.h"
#include "../Framework/FileData.h"

class Button;
class DrawSelect;
class DrawObj;
class SaveWindowBox;
class LoadWindowBox;
class UiObject;

class MapToolUiMgr : public UiMgr
{
private:
	map<int, vector<UiObject*>> uiObjList;

	Button* saveBtn;
	Button* loadBtn;
	//Button* exitBtn;
	Button* paletteBook;

	Button* selectBtn;
	vector<string> selects;
	vector<int> selectTextSize;
	vector<float> selectPosY;
	int selIdx;
	map<string, vector<Button*>> type_selects;

	vector<DrawSelect*> drawObj;
	DrawObj* nowDraw;
	map<string, vector<EditorObjs>> editorObjs;

	SaveWindowBox* saveWindow;
	LoadWindowBox* loadWindow;

	UiObject* nowEvObj;

public:
	MapToolUiMgr(Scene* scene);
	virtual ~MapToolUiMgr();
	virtual void Init();
	virtual void Release();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	virtual void SetPos(const Vector2f& pos);
	virtual void Select(DrawSelect* select);
	DrawObj* GetDraw() { return nowDraw; }
	void DeleteDraw();
	bool IsUnder();
	
	void SetLoadPath(string path);
	void SetLoadInit();
	bool IsSave();
	bool IsLoad();
	bool LoadActive();
	string loadFile();

	bool IsExit();

	string GetPath();

};

