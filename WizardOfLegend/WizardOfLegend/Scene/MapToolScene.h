#pragma once
#include "Scene.h"

class VertexArrayObj;

class MapToolScene :  public Scene
{
protected:
	
	VertexArrayObj* tileMap;
public:
	MapToolScene();
	virtual ~MapToolScene();


	virtual void Init(); //����
	virtual void Release();	 //����

	virtual void Reset();

	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	void CreateTileMap(int rows, int cols);

};

