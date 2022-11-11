#include "MapToolScene.h"
#include "../GameObject/VertexArrayObj.h"
#include "../Framework/Framework.h"
#include "../Framework/InputMgr.h"
#include "../Ui/MapToolUiMgr.h"
#include <math.h>
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

	boldTile.setSize({ 64,64 });
	boldTile.setOutlineThickness(4);
	boldTile.setOutlineColor(Color::Magenta);
	boldTile.setFillColor({0, 0, 0, 0});

	uiMgr = new MapToolUiMgr();
	uiMgr->Init();
}

void MapToolScene::Release()
{
	Scene::Release();
	if (uiMgr != nullptr)
	{
		uiMgr->Release();
		delete uiMgr;
	}
}

void MapToolScene::Reset()
{
	for (auto& obj : objList)
	{
		obj->Reset();
	}
	uiMgr->Reset();
}

void MapToolScene::Enter()
{
	Vector2f size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(size.x * 0.5f, size.y * 0.5f);
	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);
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
	if (InputMgr::GetMouseButton(Mouse::Middle))
	{
		Vector2f pos = InputMgr::GetMousePosDisplacement();
		worldView.setCenter(worldView.getCenter() + pos);
	}

	float boldTilePosX = floor(objMousePos.x / 64) * 64;
	float boldTilePosY = floor(objMousePos.y / 64) * 64;
	boldTile.setPosition({ boldTilePosX,boldTilePosY });
	//0~63�� 0���� �����Ѵ�.
	Scene::Update(dt);	
	uiMgr->Update(dt);

}

void MapToolScene::Draw(RenderWindow& window)
{
	Scene::Draw(window);
	tileMap->Draw(window);
	window.draw(boldTile);

	window.setView(uiView);
	uiMgr->Draw(window);
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