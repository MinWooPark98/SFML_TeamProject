#include "Store.h"
#include "../../DataTable/DataTableMGR.h"
#include "../../DataTable/RelicTable.h"
#include "../../DataTable/HoodTable.h"
#include "../../DataTable/SkillSetTable.h"
#include "Interactive/GoodsHood.h"
#include "Interactive/GoodsRelic.h"
#include "Interactive/GoodsSkill.h"
#include "../Scene/SceneMgr.h"
#include "../Scene/PlaySceneDataMgr.h"

Store::Store(Goods::Payment payment, Goods::Types goodsType)
	:payment(payment), goodsType(goodsType)
{
}

Store::~Store()
{
}

void Store::Init()
{
	SpriteObj::Init();
	sprite.setScale({ 1.25f, 1.25f });
	auto& objList = SCENE_MGR->GetCurrentScene()->GetObjList();

	switch (goodsType)
	{
	case Goods::Types::Hood:
		{
			auto wholeTable = DATATABLE_MGR->Get<HoodTable>(DataTable::Types::Hood)->GetTable();
			auto& table = payment == Goods::Payment::Gold ? wholeTable[HoodTable::Locked::Unlocked] : wholeTable[HoodTable::Locked::Locked];
			vector<int> toShuffle;
			for (auto& data : table)
			{
				toShuffle.push_back(data.first);
			}
			if (payment == Goods::Payment::Gold)
			{
				auto hoodId = PLAYSCENE_DATAMGR->GetHoodId();
				for (auto it = toShuffle.begin(); it != toShuffle.end();)
				{
					if (hoodId == table[*it].id)
						it = toShuffle.erase(it);
					else
						++it;
				}
			}

			random_device rd;
			mt19937 g(rd());
			std::shuffle(toShuffle.begin(), toShuffle.end(), g);
			for (int i = 0; i < 3; ++i)
			{
				Goods* newGoods = new GoodsHood(payment);
				newGoods->Init();
				if (i < toShuffle.size())
					((GoodsHood*)newGoods)->SetInfo(table[toShuffle[i]]);
				goodsList.push_back(newGoods);
			}
			sprite.setTexture(*RESOURCE_MGR->GetTexture("graphics/StoreTarp.png"));
			SetOrigin(Origins::BC);
		}
		break;
	case Goods::Types::Relic:
		{
			auto wholeTable = DATATABLE_MGR->Get<RelicTable>(DataTable::Types::Relic)->GetTable();
			auto& table = payment == Goods::Payment::Gold ? wholeTable[RelicTable::Locked::Unlocked] : wholeTable[RelicTable::Locked::Locked];
			vector<int> toShuffle;
			for (auto& data : table)
			{
				toShuffle.push_back(data.first);
			}
			if (payment == Goods::Payment::Gold)
			{
				auto& currRelicIds = PLAYSCENE_DATAMGR->GetRelicIds();
				for (auto it = toShuffle.begin(); it != toShuffle.end();)
				{
					bool isExist = false;
					for (auto currRelicId : currRelicIds)
					{
						if (currRelicId == table[*it].id)
						{
							it = toShuffle.erase(it);
							isExist = true;
							break;
						}
					}
					if (!isExist)
						++it;
				}
			}

			random_device rd;
			mt19937 g(rd());
			std::shuffle(toShuffle.begin(), toShuffle.end(), g);
			for (int i = 0; i < 1; ++i)
			{
				if (i >= toShuffle.size())
					break;
				Goods* newGoods = new GoodsRelic(payment);
				newGoods->Init();
				((GoodsRelic*)newGoods)->SetInfo(table[toShuffle[i]]);
				goodsList.push_back(newGoods);
			}
			sprite.setTexture(*RESOURCE_MGR->GetTexture("graphics/RelicStoreTarp.png"));
			SetOrigin(Origins::BC);
		}
		break;
	case Goods::Types::Skill:
		{
			auto wholeTable = DATATABLE_MGR->Get<SkillSetTable>(DataTable::Types::SkillSet)->GetTable();
			auto& table = payment == Goods::Payment::Gold ? wholeTable[SkillSetTable::Locked::Unlocked] : wholeTable[SkillSetTable::Locked::Locked];
			vector<string> toShuffle;
			for (auto& element : table)
			{
				for (auto& data : element.second)
				{
					toShuffle.push_back(data.first);
				}
			}
			random_device rd;
			mt19937 g(rd());
			std::shuffle(toShuffle.begin(), toShuffle.end(), g);
			for (int i = 0; i < 2; ++i)
			{
				if (i >= toShuffle.size())
					break;
				Goods* newGoods = new GoodsSkill(payment);
				newGoods->Init();
				((GoodsSkill*)newGoods)->SetInfo(toShuffle[i]);
				goodsList.push_back(newGoods);
			}
			sprite.setTexture(*RESOURCE_MGR->GetTexture("graphics/StoreTarp.png"));
			SetOrigin(Origins::BC);
		}
		break;
	default:
		break;
	}

	for (auto goods : goodsList)
	{
		goods->SetObjType(Object::ObjTypes::ETC);
		objList[Scene::LayerType::Object][0].push_back(goods);
	}
}

void Store::Update(float dt)
{
	SpriteObj::Update(dt);
	for (auto goods : goodsList)
	{
		if (goods->GetActive())
			goods->Update(dt);
	}
}

void Store::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
}

void Store::SetPos(const Vector2f& pos)
{
	SpriteObj::SetPos(pos);
	auto bound = GetGlobalBounds();
	for (int i = 0; i <  goodsList.size(); ++i)
	{
		goodsList[i]->SetPos({ bound.left + bound.width * (1.f / (goodsList.size() * 2)) * (i * 2 + 1), bound.top + bound.height * 0.85f });
	}
}

void Store::SwitchDevMode()
{
	SpriteObj::SwitchDevMode();
	for (auto goods : goodsList)
	{
		goods->SwitchDevMode();
	}
}
