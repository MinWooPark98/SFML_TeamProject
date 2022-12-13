#pragma once
#include "../GameObject/Object.h"
#include "../DataTable/DataTable.h"
#include <functional>

class OptionButtons;

class DataTableList : public Object
{
protected:
	RenderWindow window;
	OptionButtons* buttons;
	DataTable::Types dataType;
	Clock clock;
	Time deltaTime;

	View view;

public:
	DataTableList();
	virtual ~DataTableList();

	virtual void SetActive(bool active) override;
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	void Draw();
	void SetDataTable(DataTable::Types dataType);
	void EndSelection(const string& str);
	function<void(string)> Selected;

	void LoadExtraSkillSetList(int idx);
};
