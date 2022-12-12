#include "DevScene.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../Ui/DevUiMgr.h"
#include "../Framework/InputMgr.h"
#include "SceneMgr.h"
#include "../GameObject/Skill.h"
#include "../GameObject/Interactive/SkillBook.h"
#include "../Ui/SkillBookUi.h"
#include "../GameObject/Item/ItemMgr.h"
#include "../DataTable/DataTableMGR.h"
#include "../DataTable/RelicTable.h"
#include "../GameObject/Interactive/Wardrobe.h"
#include "../GameObject/Interactive/ItemBox.h"
#include "../Ui/ItemBoxUi.h"
#include "../Ui/WardrobeUi.h"
#include "../GameObject/Store.h"

DevScene::DevScene()
	:Scene(Scenes::Dev), itemMgr(nullptr)
{
}

DevScene::~DevScene()
{
}

void DevScene::Init()
{
	Scene::Init();
	auto& windowSize = FRAMEWORK->GetWindowSize();

	itemMgr = new ItemMgr();

	Player* player = new Player();
	player->Init();
	player->Translate({ -240.f, 90.f });
	player->SetName("PLAYER");
	player->SetSkillSet(0, "FireBall");
	player->SetSkillSet(1, "FireDash");
	player->SetSkillSet(4, "DragonArc");
	player->SetSkillSet(5, "JumpMeteor");
	objList[LayerType::Object][0].push_back(player);

	SkillBook* book = new SkillBook();
	book->Init();
	book->SetName("SKILLBOOK");
	book->SetPos((Vector2f)windowSize * 0.05f);
	objList[LayerType::Object][0].push_back(book);

	Wardrobe* wardrobe = new Wardrobe();
	wardrobe->Init();
	wardrobe->SetName("WARDROBE");
	wardrobe->SetPos((Vector2f)windowSize * 0.05f + Vector2f(250.f, 0.f));
	objList[LayerType::Object][0].push_back(wardrobe);

	ItemBox* itemBox = new ItemBox();
	itemBox->Init();
	itemBox->SetName("ITEMBOX");
	itemBox->SetPos(Vector2f(windowSize.x * 0.05f, windowSize.y * 0.2f));
	objList[LayerType::Object][0].push_back(itemBox);

	Store* store = new Store(Goods::Payment::Gold, Goods::Types::Relic);
	store->Init();
	store->SetPos(Vector2f(windowSize.x * 0.05f + 150.f, windowSize.y * 0.2f));
	objList[LayerType::Object][0].push_back(store);

	uiMgr = new DevUiMgr();
	uiMgr->Init();

	book->Interact = bind(&SkillBookUi::SetActive, (SkillBookUi*)uiMgr->FindUiObj("SKILLBOOKUI"), true);
	wardrobe->Interact = bind(&WardrobeUi::SetActive, (WardrobeUi*)uiMgr->FindUiObj("WARDROBEUI"), true);
	itemBox->Interact = bind(&ItemBox::SetActive, (ItemBoxUi*)uiMgr->FindUiObj("ITEMBOXUI"), true);
}

void DevScene::Release()
{
	Scene::Release();
}

void DevScene::Reset()
{
	Scene::Reset();
}

void DevScene::Update(float dt)
{
	Scene::Update(dt);
}

void DevScene::Draw(RenderWindow& window)
{
	Scene::Draw(window);
}

void DevScene::Enter()
{
	Scene::Enter();
	Vector2f size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size * 0.25f);
	worldView.setCenter(size * 0.125f);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
}

void DevScene::Exit()
{
	Scene::Exit();
}
