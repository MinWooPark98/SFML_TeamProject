#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <map>

using namespace sf;
using namespace std;

enum class Axis
{
	Horizontal,
	Vertical,
};

struct AxisInfo
{
	Axis axis;
	list<Keyboard::Key> positives;
	list<Keyboard::Key> negatives;

	float sensi;
	float value;
};

class InputMgr
{
private:
	static map<Axis, AxisInfo> axisInfoMap;

	static list<Keyboard::Key> downList; // 
	static list<Keyboard::Key> ingList;	 // 
	static list<Keyboard::Key> upList;	 // 

	static list<Mouse::Button> downMouse; // 
	static list<Mouse::Button> ingMouse;  // 
	static list<Mouse::Button> upMouse;	  //

	static int mouseWheelMoved;

	static Vector2f mousePos;
	static Vector2f prevMousePos;
	static Vector2f mousePosDisplacement;
	static bool wheelUp;
	static bool wheelDown;

public:
	static void Init();
	static void Update(float dt);
	static void ProcessInput(Event& ev);
	static void Clear();

	static bool GetKeyDown(Keyboard::Key key);
	static bool GetKey(Keyboard::Key key);
	static bool GetKeyUp(Keyboard::Key key);

	static bool GetMouseButtonDown(Mouse::Button key);
	static bool GetMouseButton(Mouse::Button key);
	static bool GetMouseButtonUp(Mouse::Button key);
	static float GetMouseWheelMoved();
	static bool GetMouseWheelUp();
	static bool GetMouseWheelDown();

	static list<Keyboard::Key> GetKeyDownList() { return downList; }
	static list<Keyboard::Key> GetKeyIngList() { return ingList; }

	static list<Mouse::Button> GetMouseDownList() { return downMouse; }
	static list<Mouse::Button> GetMouseIngList() { return ingMouse; }

	static const Vector2f& GetMousePos();
	static const Vector2f& GetMousePosDisplacement();

	static float GetAxis(Axis axis);
	static float GetAxisRaw(Axis axis);

	static char GetLastKey();
	static bool GetKeyDown();
};
