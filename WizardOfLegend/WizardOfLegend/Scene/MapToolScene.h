#pragma once
#include "Scene.h"

class VertexArrayObj;

class MapToolScene :  public Scene
{
protected:
	
	VertexArrayObj* tileMap;
	RectangleShape boldTile;
public:
	MapToolScene();
	virtual ~MapToolScene();


	virtual void Init() override; //생성
	virtual void Release() override;	 //해제

	virtual void Reset() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	void CreateTileMap(int rows, int cols);

};

