#pragma once
#include "../3rd/Singleton.h"
#include "FileData.h"

class FileMgr : public Singleton<FileMgr>
{
private:
	map<string, vector<ns::RectangleInfo>> hitBoxData; //find by path
	map<string, vector<ObjectData>> mapInfo;
	map<string, vector<EditorObjs>> editorObjs;
public:
	FileMgr();
	~FileMgr();

	void LoadAll();
	const vector<ns::RectangleInfo>& GetHitBox(string name);

	const vector<ObjectData>& GetMap(string name);
	const map<string, vector<ObjectData>>& GetAllMap() { return mapInfo; };
	void SaveMap(vector<ObjectData> newData, string name);

	const map<string, vector<EditorObjs>>& GetEditorObjs() { return editorObjs; }
};

#define FILE_MGR (FileMgr::GetInstance())