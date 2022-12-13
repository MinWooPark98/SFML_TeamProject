#include "PlayScene.h"
#include "../GameObject/Lancer.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Skill.h"
#include "../GameObject/Projectile.h"
#include "../GameObject/Archer.h"
#include "../GameObject/HeavyBombingArcher.h"
#include "../GameObject/FireBoss.h"
#include "../Framework/InputMgr.h"
#include "../Scene/SceneMgr.h"
#include "../Ui/PlayUiMgr.h"
#include "../GameObject/SkillSet.h"
#include "../GameObject/Sector.h"
#include "../GameObject/Cliff.h"
#include "../GameObject/CastingCircle.h"
#include "../GameObject/FinalBoss.h"
#include "../Ui/ShowDamage.h"
#include "../Framework/CameraMove.h"
#include "../GameObject/Gold.h"
#include "../GameObject/ChaosFragments.h"
#include "../GameObject/Dummy.h"
#include "../GameObject/HitSpark.h"
#include "../GameObject/GlassTube.h"
#include "../GameObject/Portal.h"
#include "../GameObject/Heal.h"
#include "../GameObject/Turret.h"
#include "../GameObject/Summoner.h"
#include "../GameObject/SpawnArea.h"
#include "../GameObject/Interactive/SkillBook.h"
#include "../GameObject/Interactive/Wardrobe.h"
#include "../GameObject/Interactive/ItemBox.h"
#include "../Ui/SkillBookUi.h"
#include "../Ui/ItemBoxUi.h"
#include "../Ui/WardrobeUi.h"
#include "../GameObject/PortalEffect.h"
#include "../GameObject/Store.h"
#include "../GameObject/Skill.h"

PlayScene::PlayScene()
	:Scene(Scenes::Play)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	Scene::Init();
	auto& windowSize = FRAMEWORK->GetWindowSize();
	showDamages = new ObjectPool<ShowDamage>;
	golds = new ObjectPool<Gold>;
	platinums = new ObjectPool<ChaosFragments>;
	enemyHitSparks = new ObjectPool<HitSpark>;
	playerHitSparks = new ObjectPool<HitSpark>;

	uiMgr = new PlayUiMgr();
	uiMgr->Init();

	isMap = true;
	auto& data = FILE_MGR->GetMap(mapName);

	for (auto& obj : data)
	{
		if (obj.type == "SECTOR")
		{
			Sector* sector = new Sector();
			sector->SetName(obj.type);
			sector->SetPos(obj.position);
			sector->SetSize(obj.size);
			sector->SetHitBox((FloatRect)sector->GetSectorShape()->getGlobalBounds());
			sector->SetObjType(Object::ObjTypes::Sector);
			sector->SetOutlineColor({ 0, 0, 0, 0 });
			room.push_back(*sector);
			objList[LayerType::Sector][0].push_back(sector);
		}
	}

	for (int i = 0; i < room.size(); i++)
	{
		collisionList.push_back(map<Object::ObjTypes, list<Object*>>());
	}

	for (auto& obj : data)
	{
		if (obj.type == "WALL")
		{
			SpriteObj* draw = new SpriteObj();
			draw->Init();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);
			if(obj.path=="graphics/Map/Palette/AirWallFoundation.png"||
				obj.path == "graphics/Map/Palette/FireWallFoundation.png" || 
				obj.path == "graphics/Map/Palette/IceWallFoundation.png")
			{
				draw->SetObjType(Object::ObjTypes::Tile);
				objList[LayerType::Tile][0].push_back(draw);
			}
			else if (obj.path == "graphics/Map/Wall/AirWallTop.png" ||
				obj.path == "graphics/Map/Wall/FireWallTop.png" ||
				obj.path == "graphics/Map/Palette/IceWallTop.png")
			{
				draw->SetObjType(Object::ObjTypes::Wall);
				objList[LayerType::Wall][0].push_back(draw);
			}
			else
			{
				draw->SetObjType(Object::ObjTypes::Wall);
				objList[LayerType::Wall][0].push_back(draw);
			}
		}
		else if (obj.type == "TILE")
		{
			SpriteObj* draw = new SpriteObj();
			draw->Init();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetObjType(Object::ObjTypes::Tile);

			objList[LayerType::Tile][0].push_back(draw);
		}
		else if (obj.type == "PLAYER")
		{
			player = new Player();
			player->Init();
			player->SetName(obj.type);
			player->SetPos(obj.position);
			player->SetObjType(Object::ObjTypes::Player);
			player->SetLastPosition({ 0,0 });
			objList[LayerType::Object][5].push_back(player);
			auto& skillSet = player->GetSkillSets();
			player->SetSkillSet(0, "FireBall", true);
			player->SetSkillSet(1, "FireDash", true);
			player->SetSkillSet(2, "JumpMeteor", true);
			player->SetSkillSet(4, "DragonArc", true);
			player->SetSkillSet(5, "FireFull", true);
			((PlayUiMgr*)uiMgr)->SetPlayer(player);
		}
		else if (obj.type == "OBJECT")
		{
			if (obj.path == "graphics/Map/Object/Portal.png")
			{
				portal = new Portal();
				portal->Init();
				portal->SetName(obj.type);
				portal->SetPos(obj.position);
				portal->SetObjType(Object::ObjTypes::ETC);
				
				if (mapName == "TUTORIALMAP")
					portal->SetChanegeMap("TUTORIALFIGHT");
				else if (mapName == "TUTORIALFIGHT")
					portal->SetChanegeMap("TEST");

				objList[LayerType::Middle][0].push_back(portal);
			}
			else if (obj.path == "graphics/Map/Object/HealthCrystal.png")
			{
				Heal* heal = new Heal();
				heal = new Heal();
				heal->Init();
				heal->SetName(obj.type);
				heal->SetPos(obj.position);
				heal->SetObjType(Object::ObjTypes::BrokenObject);
				heal->SetPlayer(player);

				objList[LayerType::Object][0].push_back(heal);
			}
			else if (obj.path == "graphics/Map/Object/tutorial_skill_1.png" ||
					obj.path == "graphics/Map/Object/tutorial_skill_2.png" ||
					obj.path == "graphics/Map/Object/tutorial_skill_3.png" ||
					obj.path == "graphics/Map/Object/tutorial_skill_4.png")
			{
				glassTube = new GlassTube();
				glassTube->SetFileName(obj.path);
				glassTube->SetName(obj.type);
				glassTube->SetPos(obj.position);
				glassTube->Init();
				glassTube->SetHitBox(obj.path);
				glassTube->SetObjType(Object::ObjTypes::ETC);

				objList[LayerType::Object][0].push_back(glassTube);
				glassTubes.push_back(glassTube);
				((PlayUiMgr*)uiMgr)->NewGlassTubes(glassTube);
			}
			else if (obj.path == "graphics/TreasureChestClosed.png")
			{
				ItemBox* itemBox = new ItemBox();
				itemBox->Init();
				itemBox->SetName("ITEMBOX");
				itemBox->SetPos(obj.position);
				itemBox->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Object][0].push_back(itemBox);
				itemBox->Interact = bind(&ItemBox::SetActive, (ItemBoxUi*)uiMgr->FindUiObj("ITEMBOXUI"), true);
			}
			else if (obj.path == "graphics/SpellBookIdle.png")
			{
				SkillBook* book = new SkillBook();
				book->Init();
				book->SetName("SKILLBOOK");
				book->SetPos(obj.position);
				book->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Object][0].push_back(book);
				book->Interact = bind(&SkillBookUi::SetActive, (SkillBookUi*)uiMgr->FindUiObj("SKILLBOOKUI"), true);
			}
			else if (obj.path == "graphics/Map/Object/WardrobeIdle.png")
			{
				Wardrobe* wardrobe = new Wardrobe();
				wardrobe->Init();
				wardrobe->SetName("WARDROBE");
				wardrobe->SetPos(obj.position);
				wardrobe->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Object][0].push_back(wardrobe);
				wardrobe->Interact = bind(&WardrobeUi::SetActive, (WardrobeUi*)uiMgr->FindUiObj("WARDROBEUI"), true);
			}
			else if (obj.path == "graphics/Map/Object/HoodTarp.png")
			{
				Store* store = new Store(Goods::Payment::Gold, Goods::Types::Hood);
				store->Init();
				store->SetName(obj.path);
				store->SetPos(obj.position);
				store->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Middle][0].push_back(store);

			}
			else if (obj.path == "graphics/Map/Object/ItemTarp.png")
			{
				Store* store = new Store(Goods::Payment::Gold, Goods::Types::Relic);
				store->Init();
				store->SetName(obj.path);
				store->SetPos(obj.position);
				store->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Middle][0].push_back(store);
			}
			else if (obj.path == "graphics/Map/Object/SkillTarp.png")
			{
				Store* store = new Store(Goods::Payment::Gold, Goods::Types::Skill);
				store->Init();
				store->SetName(obj.path);
				store->SetPos(obj.position);
				store->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Middle][0].push_back(store);
			}
			else if (obj.path == "graphics/Map/Object/SquareHoodTarp.png")
			{
				Store* store = new Store(Goods::Payment::Platinum, Goods::Types::Hood);
				store->Init();
				store->SetName(obj.path);
				store->SetPos(obj.position);
				store->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Middle][0].push_back(store);
			}
			else if (obj.path == "graphics/Map/Object/SquareItemTarp.png")
			{
				Store* store = new Store(Goods::Payment::Platinum, Goods::Types::Relic);
				store->Init();
				store->SetName(obj.path);
				store->SetPos(obj.position);
				store->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Middle][0].push_back(store);
			}
			else if (obj.path == "graphics/Map/Object/SquareSkillTarp.png")
			{
				Store* store = new Store(Goods::Payment::Platinum, Goods::Types::Skill);
				store->Init();
				store->SetName(obj.path);
				store->SetPos(obj.position);
				store->SetObjType(Object::ObjTypes::ETC);
				objList[LayerType::Middle][0].push_back(store);
			}
			else if (obj.path == "graphics/Map/Object/TeleportLandingPad.png")
			{
				SpriteObj* draw = new SpriteObj();
				draw->Init();
				draw->SetName(obj.type);
				draw->SetFileName(obj.path);
				draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
				draw->SetOrigin(Origins::BC);
				draw->SetPos(obj.position);
				draw->SetHitBox(obj.path);
				draw->SetObjType(Object::ObjTypes::Middle);
				objList[LayerType::Middle][0].push_back(draw);
			}
			else
			{
				SpriteObj* draw = new SpriteObj();
				draw->Init();
				draw->SetName(obj.type);
				draw->SetFileName(obj.path);
				draw->SetTexture(*RESOURCE_MGR->GetTexture(obj.path));
				draw->SetOrigin(Origins::BC);
				draw->SetPos(obj.position);
				draw->SetHitBox(obj.path);
				draw->SetObjType(Object::ObjTypes::ETC);
				if (obj.path == "graphics/Map/Object/GateFull.png" ||
					obj.path == "graphics/Map/Palette/LeftGate.png" ||
					obj.path == "graphics/Map/Palette/RightGate.png"||
					obj.path == "graphics/Map/Palette/invisibleGate.png")
				{
					draw->SetActive(false);
					objList[LayerType::High][0].push_back(draw);
				}
				else if(obj.path == "graphics/Space.png")
				{
					objList[LayerType::High][0].push_back(draw);
				}
				else
				{
					objList[LayerType::Object][0].push_back(draw);
				}
			}
		}
		else if (obj.type == "ENEMY")
		{
			if (obj.path == "graphics/Map/Lancer.png")
			{
				Lancer* lancer = new Lancer();
				lancer->Init();
				lancer->SetName(obj.type);
				lancer->SetPos(obj.position);
				lancer->SetCardPos(lancer->GetPos());
				lancer->SetObjType(Object::ObjTypes::Enemy);
				lancer->SetActive(false);
				lancer->SetLastPosition({ 0,0 });
				objList[LayerType::Object][0].push_back(lancer);

			}
			else if (obj.path == "graphics/Map/ArcherNormal.png")
			{
				Archer* archer = new Archer();
				archer->Init();
				archer->SetName(obj.type);
				archer->SetPos(obj.position);
				archer->SetCardPos(archer->GetPos());
				archer->SetObjType(Object::ObjTypes::Enemy);
				archer->SetColor(3);
				archer->SetActive(false);
				archer->SetLastPosition({ 0,0 });
				objList[LayerType::Object][1].push_back(archer);
			}
			else if (obj.path == "graphics/Map/ArcherBosspng.png")
			{
				heavyBombingArcher = new HeavyBombingArcher();
				heavyBombingArcher->Init();
				((PlayUiMgr*)uiMgr)->SetHeavyBombingArcher(heavyBombingArcher);
				heavyBombingArcher->SetName(obj.type);
				heavyBombingArcher->SetPos(obj.position);
				heavyBombingArcher->SetCardPos(heavyBombingArcher->GetPos());
				heavyBombingArcher->SetObjType(Object::ObjTypes::Enemy);
				heavyBombingArcher->SetColor(2);
				heavyBombingArcher->SetActive(false);
				heavyBombingArcher->SetLastPosition({ 0,0 });
				objList[LayerType::Object][2].push_back(heavyBombingArcher);
			}
			else if (obj.path == "graphics/Map/FireBoss.png")
			{
				fireBoss = new FireBoss();
				fireBoss->Init();
				((PlayUiMgr*)uiMgr)->SetFireBoss(fireBoss);
				fireBoss->SetName(obj.type);
				fireBoss->SetPos(obj.position);
				fireBoss->SetObjType(Object::ObjTypes::Enemy);
				fireBoss->SetActive(false);
				fireBoss->SetLastPosition({ 0,0 });
				objList[LayerType::Object][3].push_back(fireBoss);
			}
			else if (obj.path == "graphics/Map/FinalBoss.png")
			{
				finalBoss = new FinalBoss();
				finalBoss->Init();
				((PlayUiMgr*)uiMgr)->SetFinalBoss(finalBoss);
				finalBoss->SetName(obj.type);
				finalBoss->SetPos(obj.position);
				finalBoss->SetObjType(Object::ObjTypes::FinalBoss);
				finalBoss->SetActive(false);
				finalBoss->SetLastPosition({ 0,0 });
				objList[LayerType::Object][0].push_back(finalBoss);
			}
			else if (obj.path == "graphics/TurretMerged.png")
			{
				Turret* turret = new Turret();
				turret->Init();
				turret->SetName(obj.type);
				turret->SetPos(obj.position);
				turret->EyePos(obj.position);
				turret->SetObjType(Object::ObjTypes::Enemy);
				turret->SetLastPosition({ 0,0 });
				turret->SetActive(true);
				objList[LayerType::Object][1].push_back(turret);
			}
			else if (obj.path == "graphics/SummonerIdleDown.png")
			{
				Summoner* summoner = new Summoner();
				summoner->Init();
				summoner->SetName(obj.type);
				summoner->SetPos(obj.position);
				summoner->SetCardPos(summoner->GetPos());
				summoner->SetObjType(Object::ObjTypes::Enemy);
				summoner->SetColor(2);
				summoner->SetActive(false);
				summoner->SetLastPosition({ 0,0 });
				objList[LayerType::Object][1].push_back(summoner);
			}
			else if (obj.path == "graphics/TrainingDummy.png")
			{
				Dummy* trainingDummy = new Dummy();
				trainingDummy->Init();
				trainingDummy->SetName(obj.type);
				trainingDummy->SetPos(obj.position);
				trainingDummy->SetInitPos(obj.position);
				trainingDummy->SetObjType(Object::ObjTypes::Dummy);
				trainingDummy->SetActive(true);
				trainingDummy->SetLastPosition({ 0,0 });
				objList[LayerType::Object][2].push_back(trainingDummy);
			}
		}
		else if (obj.type == "CLIFF")
		{
			Cliff* cliff = new Cliff();
			cliff->Init();
			cliff->SetName(obj.type);
			cliff->SetPos(obj.position);
			cliff->SetSize({obj.size.x-8, obj.size.y-8});
			cliff->SetHitBox({ (FloatRect)cliff->GetCliffShape()->getGlobalBounds() }, Color::Red);
			cliff->SetOutlineColor({ 0, 0, 0, 0 });
			cliff->SetObjType(Object::ObjTypes::Cliff);
			objList[LayerType::Object][3].push_back(cliff);
		}
		else if (obj.type == "SPAWNAREA")
		{
			SpawnArea* spawnArea= new SpawnArea();
			spawnArea->Init();
			spawnArea->SetName(obj.type);
			spawnArea->SetPos(obj.position);
			spawnArea->SetSize({ obj.size.x - 8, obj.size.y - 8 });
			spawnArea->SetHitBox({ (FloatRect)spawnArea->GetSpawnAreaShape()->getGlobalBounds() });
			spawnArea->SetOutlineColor({ 0, 0, 0, 0 });
			spawnArea->SetObjType(Object::ObjTypes::SpawnArea);
			objList[LayerType::Object][3].push_back(spawnArea);
		}
	}

	for (int i = 0; i < room.size(); i++)
	{
		for (auto& a : objList)
		{
			for (auto& b : a.second)
			{
				for (auto& c : b.second)
				{
					if (Utils::OBB(c->GetHitBox(), room[i].GetHitBox()))
					{
						collisionList[i][c->GetObjType()].push_back(c);
					}
				}
			}
		}
		room[i].SetAliveEnemyCount(collisionList[i][Object::ObjTypes::Enemy].size() + collisionList[i][Object::ObjTypes::FinalBoss].size());
		//cout << i << "�� ���ʹ� �� : " << collisionList[i][Object::ObjTypes::Enemy].size() << endl;
	}

	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 16;
	mapSize.height = (tiles.back())->GetPos().y;

	for (int i = 0; i < room.size(); i++)
	{
		for (auto& enemy : collisionList[i][Object::ObjTypes::Enemy])
		{
			((Enemy*)enemy)->SetPlayer(player);
		}
		for (auto& finalBoss : collisionList[i][Object::ObjTypes::FinalBoss])
		{
			((FinalBoss*)finalBoss)->SetPlayer(player);
		}
	}
	if (fireBoss != nullptr)
		fireBoss->SetPlayerLastPos(player->GetPos());

	portalEffect = new PortalEffect();
	portalEffect->Init();
}

void PlayScene::Update(float dt)
{
	CameraMove::CameraShake(dt);
	Scene::Update(dt);

	if (portal != nullptr)
	{
		portal->Update(dt);
		if (player->GetHitBounds().intersects(portal->GetHitBounds()))
			portal->ChangeMap();
	}
	/*if (glassTubes.size() != 0)
	{
		for (int i = 0; i < glassTubes.size(); i++)
		{
			glassTubes[i]->Update(dt);

			if (glassTubes[i]->GetIsPlayerAdjacent())
				((PlayUiMgr*)uiMgr)->GlassTubeSet(i, true);
		}
	}*/

	if (InputMgr::GetKeyDown(Keyboard::Key::Escape))
	{
		if (GetPause())
		{
			if (InputMgr::GetStackedOrder().empty() && !((PlayUiMgr*)uiMgr)->IsOption())
				SetPause(false);
		}
		else if (InputMgr::GetEscapable() && ((PlayUiMgr*)uiMgr)->IsOption())
			SetPause(true);
	}

	showDamages->Update(dt);
	golds->Update(dt);
	platinums->Update(dt);
	enemyHitSparks->Update(dt);
	playerHitSparks->Update(dt);
	portalEffect->Update(dt);

	//�÷��̾� �� ��ġ 
	for (int i = 0; i < room.size(); i++)
	{
		auto& collListPlayer = collisionList[i][Object::ObjTypes::Player];
		if (player->GetLowHitBounds().intersects(room[i].GetHitBounds()))
		{
			if (find(playerRooms.begin(), playerRooms.end(), i) == playerRooms.end())
			{
				collisionList[i][Object::ObjTypes::Player].push_back(player);
				playerRooms.push_back(i);
			}
			SpawnEnemy(i, dt);
			AllDieEnemy(i);
		}
		else
		{
			if (find(playerRooms.begin(), playerRooms.end(), i) != playerRooms.end())
			{
				collisionList[i][Object::ObjTypes::Player].remove(player);
				playerRooms.remove(i);
			}
		}
	}

	for (int i = 0; i < (int)Object::ObjTypes::Count; ++i)
	{
		switch ((Object::ObjTypes)i)
		{
		case Object::ObjTypes::Enemy:
		case Object::ObjTypes::FinalBoss:
		case Object::ObjTypes::Player:
			for (int j = 0; j < collisionList.size(); ++j)
			{
				if (collisionList[j][Object::ObjTypes::Player].empty())
					continue;
				for (auto& obj : collisionList[j][(Object::ObjTypes)i])
				{
					OnCollisionWall(j, obj);
					OnCollisionETC(j, obj);
				}
			}
			break;
		case Object::ObjTypes::BrokenObject:
			for (int j = 0; j < collisionList.size(); ++j)
			{
				if (collisionList[j][Object::ObjTypes::Player].empty())
					continue;
				for (auto& obj : collisionList[j][Object::ObjTypes::Dummy])
				{
					if ((Dummy*)obj->GetActive())
					{
						OnCollisionWall(j, (Dummy*)obj);
						OnCollisionETC(j, (Dummy*)obj);
					}
				}
			}
			break;
		default:
			break;
		}
	}

	auto& usingProjectiles = projectiles->GetUseList();
	for (auto& projectile : usingProjectiles)
	{
		if (((Projectile*)projectile)->GetAtkShape() != Skill::AttackShape::Wave || projectile->GetMovingTimer() <= 0.05f)
			continue;
		for (int i = 0; i < collisionList.size(); ++i)
		{
			if (collisionList[i][Object::ObjTypes::Player].empty())
				continue;
			for (auto& coll : collisionList[i][Object::ObjTypes::Wall])
			{
				auto projectileBnd = projectile->GetHitBounds();
				if (projectile->GetHitBounds().intersects(coll->GetHitBounds()))
				{
					projectile->SetDrawable(false);
					if (coll->GetHitBounds().contains(Vector2f(projectileBnd.left + projectileBnd.width * 0.5f, projectileBnd.top + projectileBnd.height * 0.5f)) ||
						projectile->GetUndrawTimer() >= 0.2f)
					{
						projectile->SetMoving(false);
						if (projectile->GetSubjectType() == Skill::SubjectType::Player)
							enemyHitSparks->Get()->SetPos(projectile->GetPos());
						else
							playerHitSparks->Get()->SetPos(projectile->GetPos());
						break;
					}
				}
				else
					projectile->SetDrawable(true);
			}
			for (auto& coll : collisionList[i][Object::ObjTypes::ETC])
			{
				if (projectile->GetHitBounds().intersects(coll->GetHitBounds()))
				{
					projectile->SetMoving(false);
					if (projectile->GetSubjectType() == Skill::SubjectType::Player)
						enemyHitSparks->Get()->SetPos(projectile->GetPos());
					else
						playerHitSparks->Get()->SetPos(projectile->GetPos());
					break;
				}
			}
		}
	}
}

void PlayScene::Draw(RenderWindow& window)
{
	Vector2i min = { (int)(worldView.getCenter().x - (int)worldView.getSize().x * 0.5f), (int)(worldView.getCenter().y - (int)worldView.getSize().y * 0.5f) };
	Vector2i max = { (int)(worldView.getCenter().x + (int)worldView.getSize().x * 0.5f), (int)(worldView.getCenter().y + (int)worldView.getSize().y * 0.5f) };

	window.setView(worldView);
	drawSortObjs.clear();
	for (int i = 0; i < 4; i++)
	{
		drawSortObjs.push_back(vector<Object*>());

	}
	for (auto& layer : objList)
	{
		if (layer.first == Scene::LayerType::Tile)
		{
			for (auto& obj_pair : layer.second)
			{
				auto& objs = obj_pair.second;
				for (auto& obj : objs)
				{
					if (obj->GetPos().x<max.x + 80 && obj->GetPos().y < max.y + 80 && obj->GetPos().x > min.x - 80 && obj->GetPos().y > min.y - 80)
					{
						if (obj->GetActive())
						{
							drawSortObjs[0].push_back(obj);
						}
					}
				}
			}
		}
		else if (layer.first == Scene::LayerType::Middle)
		{
			for (auto& obj_pair : layer.second)
			{
				auto& objs = obj_pair.second;
				for (auto& obj : objs)
				{
					if (obj->GetPos().x<max.x + 80 && obj->GetPos().y < max.y + 80 && obj->GetPos().x > min.x - 80 && obj->GetPos().y > min.y - 80)
					{
						if (obj->GetActive())
						{
							drawSortObjs[1].push_back(obj);
						}
					}
				}
			}
		}
		else if (layer.first == Scene::LayerType::Wall || layer.first == Scene::LayerType::Object)
		{
			for (auto& obj_pair : layer.second)
			{
				auto& objs = obj_pair.second;
				for (auto& obj : objs)
				{
					if (obj->GetPos().x<max.x + 80 && obj->GetPos().y < max.y + 80 && obj->GetPos().x > min.x - 80 && obj->GetPos().y > min.y - 80)
					{
						if (obj->GetActive())
						{
							drawSortObjs[2].push_back(obj);
						}
					}
				}
			}
		}
		else if (layer.first == Scene::LayerType::High)
		{
			for (auto& obj_pair : layer.second)
			{
				auto& objs = obj_pair.second;
				for (auto& obj : objs)
				{
					if (obj->GetPos().x<max.x + 80 && obj->GetPos().y < max.y + 80 && obj->GetPos().x > min.x - 80 && obj->GetPos().y > min.y - 80)
					{
						if (obj->GetActive())
						{
							drawSortObjs[3].push_back(obj);
						}
					}
				}
			}
		}
	}
	//for (auto& layer : objList)
	//{
	//		for (auto& obj_pair : layer.second)
	//		{
	//			auto& objs = obj_pair.second;
	//			for (auto& obj : objs)
	//			{
	//				if (obj->GetPos().x<max.x + 80 && obj->GetPos().y < max.y + 80 && obj->GetPos().x > min.x - 80 && obj->GetPos().y > min.y - 80)
	//				{
	//					if (obj->GetActive())
	//					{
	//						if (obj->GetObjType() == Object::ObjTypes::Tile)
	//						{
	//							drawSortObjs[0].push_back(obj);
	//						}
	//						else if (obj->GetObjType() == Object::ObjTypes::Middle)
	//						{
	//							drawSortObjs[1].push_back(obj);
	//						}
	//						else if (obj->GetObjType() == Object::ObjTypes::Wall || obj->GetObjType() == Object::ObjTypes::Player || obj->GetObjType() == Object::ObjTypes::Enemy || obj->GetObjType() == Object::ObjTypes::FinalBoss || obj->GetObjType() == Object::ObjTypes::ETC || obj->GetObjType() == Object::ObjTypes::Dummy || obj->GetObjType() == Object::ObjTypes::BrokenObject)
	//						{
	//							drawSortObjs[2].push_back(obj);
	//						}
	//					}
	//				}
	//			}
	//		}
	//}
	DrawSort(drawSortObjs[2]);

	for (auto& layer : drawSortObjs)
	{
		for (auto& obj : layer)
		{
			obj->Draw(window);
		}
	}
	for (auto showDamage : showDamages->GetUseList())
	{
		showDamage->Draw(window);
	}
	for (auto gold : golds->GetUseList())
	{
		gold->Draw(window);
	}
	for (auto platinum : platinums->GetUseList())
	{
		platinum->Draw(window);
	}
	for (auto enemyHitSpark : enemyHitSparks->GetUseList())
	{
		enemyHitSpark->Draw(window);
	}
	for (auto playerHitSpark : playerHitSparks->GetUseList())
	{
		playerHitSpark->Draw(window);
	}
	if (portalEffect != nullptr && portalEffect->GetActive())
		portalEffect->Draw(window);

	if (glassTubes.size() != 0)
	{
		for (int i = 0; i < glassTubes.size(); i++)
			glassTubes[i]->Draw(window);
	}

	if (uiMgr != nullptr && uiMgr->GetActive())
	{
		window.setView(uiView);
		uiMgr->Draw(window);
	}

}

void PlayScene::Release()
{
	Scene::Release();

	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto& objs = obj_pair.second;
			for (auto& obj : objs)
			{
				obj->Release();
				delete obj;
			}
		}
	}
	glassTubes.clear();
	portal = nullptr;
	objList.clear();
	room.clear();
	playerRooms.clear();
	collisionList.clear();
	fireBoss = nullptr;
	finalBoss = nullptr;
	heavyBombingArcher = nullptr;
	player = nullptr;
	if (projectiles != nullptr)
		projectiles->Release();
	if (circles != nullptr)
		circles->Release();
	isPause = false;
	hpBarSet = true;
	if (uiMgr != nullptr)
		delete uiMgr;
	uiMgr = nullptr;
}

void PlayScene::Reset()
{
	Scene::Reset();
	((Player*)player)->Reset();
	((FinalBoss*)finalBoss)->Reset();
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto& objs = obj_pair.second;
			for (auto& obj : objs)
			{
				if (obj->GetObjType() == Object::ObjTypes::Enemy)
				{
					((Enemy*)obj)->Reset();
				}
			}
		}
	}
}

void PlayScene::Enter()
{
	Scene::Enter();
	Vector2f size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size * 0.25f);
	worldView.setCenter(size * 0.125f);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Release();
	Init();
	player->LoadPlatinum();
	portalEffect->ShowPortalEffect({ player->GetPos().x, player->GetPos().y + (player->GetSize().y * 0.5f)});
}

void PlayScene::Exit()
{
	Scene::Exit();
}

void PlayScene::SpawnEnemy(int i, float dt)
{
	
	for (auto& c_list : collisionList[i])
	{
		for (auto& obj : c_list.second)
		{
			if (obj->GetObjType() == Object::ObjTypes::SpawnArea)
			{
				if (!obj->GetHitBounds().intersects(player->GetHitBounds()))
				{
					room[i].SetIsSpawn(false);
					return;
				}
				else
				{
					room[i].SetIsSpawn(true);
				}
			}
		}
	}
	if (room[i].GetIsSpawn())
	{
		for (auto& c_list : collisionList[i])
		{
			for (auto& obj : c_list.second)
			{

					if ((obj->GetObjType() == Object::ObjTypes::Enemy && ((Enemy*)obj)->GetIsAlive()) ||
						((obj->GetObjType() == Object::ObjTypes::FinalBoss) && ((FinalBoss*)obj)->GetState() != FinalBoss::States::Die))
					{
						obj->SetActive(true);
					}
					if (obj->GetObjType() == Object::ObjTypes::ETC)
					{
						if (obj->GetFileName() == "graphics/Map/Object/GateFull.png" ||
							obj->GetFileName() == "graphics/Map/Palette/LeftGate.png" ||
							obj->GetFileName() == "graphics/Map/Palette/RightGate.png")
						{
							obj->SetActive(true);
						}
					}
			}
		}
		room[i].SetIsSpawn(false);
	}
	
}

void PlayScene::AllDieEnemy(int i)
{
	if (collisionList[i][Object::ObjTypes::Enemy].empty() && collisionList[i][Object::ObjTypes::FinalBoss].empty())
	{
		if (room[i].GetAllEnemyDead())
		{
			for (auto& c_list : collisionList[i])
			{
				for (auto& obj : c_list.second)
				{
					if (obj->GetObjType() == Object::ObjTypes::ETC)
					{
						if (obj->GetFileName() == "graphics/Map/Object/GateFull.png" ||
							obj->GetFileName() == "graphics/Map/Palette/LeftGate.png" ||
							obj->GetFileName() == "graphics/Map/Palette/RightGate.png")
						{
							obj->SetActive(false);
						}
					}
				}
			}
		}
		return;
	}
	for (auto& c_list : collisionList[i])
	{
		for (auto& obj : c_list.second)
		{
			if (obj->GetObjType() == Object::ObjTypes::Enemy ||
				obj->GetObjType() == Object::ObjTypes::FinalBoss)
			{
				if (obj->GetObjType() == Object::ObjTypes::FinalBoss)
				{
					if (finalBoss != nullptr)
					{
						if (finalBoss->GetActive())
						{
							if (hpBarSet)
							{
								((PlayUiMgr*)uiMgr)->SetBossType(PlayUiMgr::BossType::FinalBoss);
								((PlayUiMgr*)uiMgr)->SetBossMaxHp(finalBoss->GetMaxHp());
								((PlayUiMgr*)uiMgr)->SetBossName("Final Boss");

								hpBarSet = false;
							}
						}
					}
				}
				else if (obj->GetObjType() == Object::ObjTypes::Enemy)
				{
					if (hpBarSet)
					{
						if (fireBoss != nullptr)
						{
							if (fireBoss->GetActive())
							{
								((PlayUiMgr*)uiMgr)->SetBossType(PlayUiMgr::BossType::FireBoss);
								((PlayUiMgr*)uiMgr)->SetBossMaxHp(fireBoss->GetMaxHp());
								((PlayUiMgr*)uiMgr)->SetBossName("Flame Queen");
								hpBarSet = false;
							}
						}

						if (heavyBombingArcher != nullptr)
						{
							if (heavyBombingArcher->GetActive())
							{
								((PlayUiMgr*)uiMgr)->SetBossType(PlayUiMgr::BossType::Archer);
								((PlayUiMgr*)uiMgr)->SetBossMaxHp(heavyBombingArcher->GetMaxHp());
								((PlayUiMgr*)uiMgr)->SetBossName("Heavy Bombing Archer");
								hpBarSet = false;
							}
						}

					}
				}
			}
			if (finalBoss != nullptr)
			{
				if (finalBoss->GetActive() && obj->GetObjType() == Object::ObjTypes::FinalBoss)
				{
					if (((FinalBoss*)obj)->GetState() != FinalBoss::States::Die)
						return;
				}
			}
			if (obj->GetObjType() == Object::ObjTypes::Enemy)
			{
				if (((Enemy*)obj)->GetIsAlive())
					return;
			}

		}
		hpBarSet = true;
		room[i].SetAllEnemyDead(true);
	}
	if (room[i].GetAllEnemyDead())
	{
		for (auto& c_list : collisionList[i])
		{
			for (auto& obj : c_list.second)
			{
				if (obj->GetObjType() == Object::ObjTypes::ETC)
				{
					if (obj->GetFileName() == "graphics/Map/Object/GateFull.png" ||
						obj->GetFileName() == "graphics/Map/Palette/LeftGate.png" ||
						obj->GetFileName() == "graphics/Map/Palette/RightGate.png")
					{
						obj->SetActive(false);
					}
				}
			}
		}
	}
}

void PlayScene::OnCollisionWall(int roomVec, Object* obj)
{
	for (auto& coll : collisionList[roomVec][Object::ObjTypes::Wall])
	{
		if (obj->GetLowHitBounds().intersects(coll->GetHitBounds()))
		{
			bool leftandRight = false;
			bool topandLow = false;

			float objRightPoint = obj->GetLowHitBounds().width + obj->GetLowHitBounds().left;
			float objLowPoint = obj->GetLowHitBounds().height + obj->GetLowHitBounds().top;

			float collXPoint = (coll->GetHitBounds().width * 0.5f) + coll->GetHitBounds().left;
			float collYPoint = (coll->GetHitBounds().height * 0.5f) + coll->GetHitBounds().top;

			if (obj->GetLowHitBounds().top >= collYPoint || objLowPoint <= collYPoint)
				topandLow = true;
			if (obj->GetLowHitBounds().left >= collXPoint || objRightPoint <= collXPoint)
				leftandRight = true;

			if (topandLow)
			{
				obj->SetPos({ obj->GetPos().x, obj->GetLastPosition().y });
			}
			if (leftandRight)
			{
				obj->SetPos({ obj->GetLastPosition().x, obj->GetPos().y });
			}
		}
	}
}

void PlayScene::OnCollisionETC(int roomVec, Object* obj)
{
	for (auto& coll : collisionList[roomVec][Object::ObjTypes::ETC])
	{
		if (!coll->GetActive())
			continue;
		if (obj->GetLowHitBounds().intersects(coll->GetHitBounds()))
		{
			bool leftandRight = false;
			bool topandLow = false;

			float objRightPoint = obj->GetLowHitBounds().width + obj->GetLowHitBounds().left;
			float objLowPoint = obj->GetLowHitBounds().height + obj->GetLowHitBounds().top;

			float collXPoint = (coll->GetHitBounds().width * 0.5f) + coll->GetHitBounds().left;
			float collYPoint = (coll->GetHitBounds().height * 0.5f) + coll->GetHitBounds().top;

			if (obj->GetLowHitBounds().top >= collYPoint || objLowPoint <= collYPoint)
				topandLow = true;
			if (obj->GetLowHitBounds().left >= collXPoint || objRightPoint <= collXPoint)
				leftandRight = true;

			if (topandLow)
			{
				obj->SetPos({ obj->GetPos().x, obj->GetLastPosition().y });
			}
			if (leftandRight)
			{
				obj->SetPos({ obj->GetLastPosition().x, obj->GetPos().y });
			}
		}
	}

	for (auto& dummy : collisionList[roomVec][Object::ObjTypes::Dummy])
	{
		if (!dummy->GetActive())
			continue;
		if (obj->GetLowHitBounds().intersects(dummy->GetHitBounds()))
		{
			bool leftandRight = false;
			bool topandLow = false;

			float objRightPoint = obj->GetLowHitBounds().width + obj->GetLowHitBounds().left;
			float objLowPoint = obj->GetLowHitBounds().height + obj->GetLowHitBounds().top;

			float collXPoint = (dummy->GetHitBounds().width * 0.5f) + dummy->GetHitBounds().left;
			float collYPoint = (dummy->GetHitBounds().height * 0.5f) + dummy->GetHitBounds().top;

			if (obj->GetLowHitBounds().top >= collYPoint || objLowPoint <= collYPoint)
				topandLow = true;
			if (obj->GetLowHitBounds().left >= collXPoint || objRightPoint <= collXPoint)
				leftandRight = true;

			if (topandLow)
			{
				obj->SetPos({ obj->GetPos().x, obj->GetLastPosition().y });
			}
			if (leftandRight)
			{
				obj->SetPos({ obj->GetLastPosition().x, obj->GetPos().y });
			}
		}
	}
}