#pragma once
#include "../GameObject/Button.h"


class LoadWindowBox : public Button
{
private:
	vector<Button*> loadFiles;
	map<string, vector<ObjectData>> allMaps;
	int idx;
	string loadPath;

public:
	LoadWindowBox(UiMgr* mgr);
	~LoadWindowBox();
	virtual void Release();
	void Init();
	void Reset();
	bool IsLoad();
	string GetLoadPath() { return loadPath; }
	virtual void Draw(RenderWindow& window);
	virtual void Update(float dt);
};