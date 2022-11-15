#include "UiMgr.h"
#include "../GameObject/UiObject.h"
#include <iostream>

UiMgr::UiMgr(Scene* scene) : parentScene(scene), enabled(true)
{
}

UiMgr::~UiMgr()
{
}

void UiMgr::Init()
{
    for (auto& uiObjs : uiObjList)
    {
        for (auto& obj : uiObjs.second)
            obj->Init();
    }
    Object::Init();
}

void UiMgr::Release()
{
    for (auto& uiObjs : uiObjList)
    {
        for (auto& obj : uiObjs.second)
        {
            if (obj != nullptr)
                delete obj;
            obj = nullptr;
        }
    }
    uiObjList.clear();
    Object::Release();
}

void UiMgr::SetPos(const Vector2f& pos)
{
    Vector2f delta = pos - position;
    for (auto& uiObjs : uiObjList)
    {
        for (auto& obj : uiObjs.second)
            obj->Translate(delta);
    }
    position = pos;
    Object::SetPos(pos);
}

void UiMgr::Update(float dt)
{
    if (!enabled)
        return;

    bool isEvent = false;

    if (nowEvObj != nullptr)
    {
        if (nowEvObj->IsMove() && nowEvObj->IsDrag())
        {
            nowEvObj->Update(dt);
            isEvent = true;
        }
        else if (nowEvObj->IsMoveRight() && nowEvObj->IsDragRight())
        {
            nowEvObj->Update(dt);
            isEvent = true;
        }
    }
    for (auto uiObjs = uiObjList.rbegin(); uiObjs != uiObjList.rend(); uiObjs++)
    {
        for (auto it = (*uiObjs).second.rbegin(); it != (*uiObjs).second.rend(); it++)
        {
            if ((*it) != nowEvObj)
                (*it)->EventClear();

            if (!isEvent)
            {
                (*it)->Update(dt);
            }
            if ((*it)->GetEvent() && !isEvent)
            {
                isEvent = true;
                nowEvObj = *it;
            }
        }
    }


    if (nowEvObj != nullptr && ((nowEvObj->GetState() == UiState::Exit) || (nowEvObj->GetState() == UiState::Enter)))
    {
        for (auto uiObjs = uiObjList.rbegin(); uiObjs != uiObjList.rend(); uiObjs++)
        {
            for (auto it = (*uiObjs).second.rbegin(); it != (*uiObjs).second.rend(); it++)
            {
                if ((*it != nowEvObj))
                {
                    if ((*it)->GetState() == UiState::Stay || (*it)->GetState() == UiState::Down)
                    {
                        (*it)->SetState(UiState::Exit); //???? ?????? ??????????? ???? Exit
                        (*it)->SetEvent(true);
                    }
                    if ((*it)->GetState() == UiState::Stay || (*it)->GetState() == UiState::DownRight)
                    {
                        (*it)->SetState(UiState::Exit); //???? ?????? ??????????? ???? Exit
                        (*it)->SetEvent(true);
                    }
                    (*it)->ColorClear(); // ??? ???? ????
                }
            }
        }
        nowEvObj = nullptr;
    }
}

void UiMgr::Draw(RenderWindow& window)
{
    if (!enabled)
        return;
    for (auto& uiObjs : uiObjList)
    {
        for (auto& obj : uiObjs.second)
            obj->Draw(window);
    }
}

