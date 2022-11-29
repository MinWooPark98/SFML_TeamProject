#include "UiObject.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework//InputMgr.h"
#include "../Framework/Utils.h"
#include "../Scene/SceneMgr.h"

UiObject::UiObject()
	: btnState(UiState::None), drag(false), dragR(false),
	isEvent(false), isClick(false), isClickR(false), isMove(false), isMoveR(false), isUiView(true)
{
}

UiObject::~UiObject()
{
	Release();
}


void UiObject::Init()
{
	Object::Init();
	Reset();
}

void UiObject::Release()
{
	Object::Release();
}

void UiObject::Reset()
{
	drag = dragR = isEvent = isClick = isClickR = isMove = false;
	btnState = UiState::None;
	isEvent = false;
	Object::Reset();
}

void UiObject::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	
}

void UiObject::Translate(const Vector2f& delta)
{
	Object::Translate(delta);
}

void UiObject::EventClear()
{
	isEvent = false;
}
void UiObject::StateClear()
{
	btnState = UiState::None;
	ColorClear();
}
void UiObject::Update(float dt)
{
	Vector2f mousePos;
	if(isUiView)
		mousePos = SCENE_MGR->GetCurrentScene()->GetUiMousePos();
	else
		mousePos = SCENE_MGR->GetCurrentScene()->GetObjMousePos();
	auto btnBound = bound;	

	switch (btnState)
	{
	case UiState::None:
		if (Utils::IsRange(btnBound, mousePos))
		{
			btnState = UiState::Enter;
			isEvent = true;
			isClick = false;
		}
		break;
	case UiState::Stay:
		if (!Utils::IsRange(btnBound, mousePos))
		{
			btnState = UiState::Exit;
			isEvent = true;
		}
		else if (!Utils::IsRange(btnBound, mousePos)) // 프레임 때문에 Exit못했을때
		{
			btnState = UiState::None;
			isEvent = false;
			isClick = false;
		}
		else if (InputMgr::GetMouseButtonDown(Mouse::Left))
		{
			if (btnState != UiState::Down)
			{
				isClick = true;
				initPos = InputMgr::GetMousePos();
			}
			else
				isClick = false;
			btnState = UiState::Down;
			isEvent = true;
		}
		else if (InputMgr::GetMouseButtonUp(Mouse::Left))
		{
			btnState = UiState::Up;
			isEvent = true;
		}
		else if (InputMgr::GetMouseButtonDown(Mouse::Right))
		{
			if (btnState != UiState::DownRight)
			{
				isClickR = true;
				initPos = InputMgr::GetMousePos();
			}
			else
				isClickR = false;
			btnState = UiState::DownRight;
			isEvent = true;
		}
		else if (InputMgr::GetMouseButtonUp(Mouse::Right))
		{
			btnState = UiState::UpRight;
			isEvent = true;
		}
		else
		{
			btnState = UiState::Stay;
			isEvent = true;
		}
		break;
	case UiState::Down:
		if (InputMgr::GetMouseButton(Mouse::Left) && (Utils::IsRange(btnBound, mousePos)))
		{
			if (btnState != UiState::Down)
			{
				isClick = true;
				initPos = InputMgr::GetMousePos();
			}
			else
				isClick = false;

			if (initPos != InputMgr::GetMousePos())
			{
				drag = true;
			}
			btnState = UiState::Down;
			isEvent = true;
		}
		else if(InputMgr::GetMouseButtonUp(Mouse::Left) && (Utils::IsRange(btnBound, mousePos)))
		{
			btnState = UiState::Up;
			isEvent = true;
			isClick = false;
		}
		else if (!Utils::IsRange(btnBound, mousePos))
		{
			btnState = UiState::Exit;
			isEvent = true;
			isClick = false;
		}
		break;
	case UiState::Up:
		if (Utils::IsRange(btnBound, mousePos))
		{
			btnState = UiState::Stay;
		}
		else
		{
			btnState = UiState::None;
		}
		drag = false;
		break;
	case UiState::DownRight:
		if (InputMgr::GetMouseButton(Mouse::Right) && (Utils::IsRange(btnBound, mousePos)))
		{
			if (btnState != UiState::DownRight)
			{
				isClickR = true;
				initPos = InputMgr::GetMousePos();
			}
			else
				isClickR = false;

			if (initPos != InputMgr::GetMousePos())
			{
				dragR = true;
			}
			btnState = UiState::DownRight;
			isEvent = true;
		}
		else if (InputMgr::GetMouseButtonUp(Mouse::Right) && (Utils::IsRange(btnBound, mousePos)))
		{
			btnState = UiState::UpRight;
			isEvent = true;
		}
		else if (!Utils::IsRange(btnBound, mousePos))
		{
			//cout << "This" << endl;
			btnState = UiState::Exit;
			isEvent = true;
		}
		break;
	case UiState::UpRight:
		if (Utils::IsRange(btnBound, mousePos))
		{
			btnState = UiState::Stay;
		}
		dragR = false;
		break;
	case UiState::Enter:
		if (InputMgr::GetMouseButton(Mouse::Left))
		{
			if (btnState != UiState::Down)
			{
				isClick = true;
				initPos = InputMgr::GetMousePos();
			}
			else
				isClick = false;
			btnState = UiState::Down;
			isEvent = true;
		}
		else if (InputMgr::GetMouseButton(Mouse::Right))
		{
			if (btnState != UiState::DownRight)
			{
				isClickR = true;
				initPos = InputMgr::GetMousePos();
			}
			else
				isClickR = false;
			btnState = UiState::DownRight;
			isEvent = true;
		}
		else
		{
			btnState = UiState::Stay;
		}
		break;
	case UiState::Exit:
		btnState = UiState::None;
		drag = false;
		dragR = false;
		break;
	default:
		break;
	}
}

void UiObject::Draw(RenderWindow& window)
{
	Object::Draw(window);
}


