#include "MapToolScene.h"
#include "../GameObject/VertexArrayObj.h"
#include "../Framework/Framework.h"
#include "../Framework/InputMgr.h"
//#include "../SFML-2.5.1/include/SFML/Graphics.hpp"

MapToolScene::MapToolScene()
	:Scene(Scenes::MapTool)
{
}

MapToolScene::~MapToolScene()
{
}

void MapToolScene::Init()
{
	Vector2i size = FRAMEWORK->GetWindowSize();

	CreateTileMap(100, 100);
	tileMap->SetPos({ 0, 0 });
	tileMap->Init();



}

void MapToolScene::Release()
{
}

void MapToolScene::Reset()
{
}

void MapToolScene::Enter()
{
	Vector2i size = FRAMEWORK->GetWindowSize();
	worldView.setSize(1980, 1080);
	worldView.setCenter(size.x * 0.5f, size.y * 0.5f);
	
}

void MapToolScene::Exit()
{
}

void MapToolScene::Update(float dt)
{
	tileMap->Update(dt);
	if (InputMgr::GetMouseWheelMoved() < 0)
	{
		Vector2f size = worldView.getSize();
		worldView.setSize(size.x * 1.05f, size.y * 1.05f);
	}
	if (InputMgr::GetMouseWheelMoved() > 0)
	{
		Vector2f size = worldView.getSize();
		worldView.setSize(size.x * 0.95f, size.y * 0.95f);
	}
	if (InputMgr::GetKeyDown(Keyboard::W))
		worldView.move(0.f, -75.f);
	if (InputMgr::GetKeyDown(Keyboard::S))
		worldView.move(0.f, 75.f);
	if (InputMgr::GetKeyDown(Keyboard::A))
		worldView.move(-75.f, 0.f);
	if (InputMgr::GetKeyDown(Keyboard::D))
		worldView.move(75.f, 0.f);
	
	Scene::Update(dt);	

}

void MapToolScene::Draw(RenderWindow& window)
{
	Scene::Draw(window);
	tileMap->Draw(window);
}

void MapToolScene::CreateTileMap(int rows, int cols)
{
	Vector2i size = FRAMEWORK->GetWindowSize();
	if (tileMap == nullptr)
	{
		tileMap = new VertexArrayObj();
		objList.push_back(tileMap);
	}
	float numLines = rows+ cols + 2;
	VertexArray& va = tileMap->GetVA();
	va.clear();
	va.setPrimitiveType(Lines);
	va.resize(2 * (numLines));

	// row separators
	for (int i = 0; i < rows + 1; i++) {
		int r = i;
		float rowY = 64 * r;
		va[i * 2].position = { 0, rowY };
		va[i * 2 + 1].position = { (float)64*cols, rowY };
	}
	// column separators
	for (int j =0; j < cols+1; j++) {
		int c = j;
		float colX = 64 * c;
		va[numLines +j * 2].position = { colX, 0 };
		va[numLines +j * 2 + 1].position = { colX, (float)64*rows };
	}
}