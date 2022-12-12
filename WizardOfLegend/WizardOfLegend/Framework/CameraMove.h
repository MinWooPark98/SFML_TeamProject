#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class CameraMove
{
protected:
	static int shakeCount;
	static bool viewShakeLeft;
	static bool viewShakeRight;

	static bool viewShake;
	static bool viewSet;

	static Vector2f view;
	static float shakeSpeed;
	static float shakeXPos;

public:
	static void CameraMoving();
	static void CameraShake(float dt);
	static void SetViewShake(bool set) { viewShake = set; };
	static void SetShakeSpeed(float speed) { shakeSpeed = speed; };
	static void SetShakeViewX(float shakeX) { shakeXPos = shakeX; };
	static void SetCameraShakeTime(int shakeTime);
};

