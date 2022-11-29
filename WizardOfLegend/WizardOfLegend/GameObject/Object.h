#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Framework/FileMgr.h"
#include <list>
#include "../Framework/Utils.h"

using namespace std;
using namespace sf;

class Object
{
public:
    enum class ObjTypes
    {
        Tile,
        Wall,
        Player,
        Enemy,
        rnejddl,
        Sector,
        ETC,
    };

protected:
    int id;
    string name;
    bool enabled;
    ObjTypes objtype;

    Vector2f position;
    float rotation;
    Vector2f direction;
    Vector2f scale;

    float gravity;
    bool gravityApply;

    static int objCount;

    RectangleShape hitbox;
    RectangleShape lowhitbox;
    Vector2f initPos;
    bool isDevMode;

    bool isUi;
    bool viewIn;

public:
    Object();
    virtual ~Object();


    virtual bool GetActive() const;
    virtual void SetActive(bool active);

    virtual void Init();
    virtual void Release();

    virtual void Reset();

    virtual void Update(float dt);
    virtual void Draw(RenderWindow& window);

    virtual void SetPos(const Vector2f& pos);
    virtual const Vector2f& GetPos() const;
    virtual void SetOrigin(Origins origin);
    virtual void Translate(const Vector2f& delta);

    void SetGravity(float g) { gravity = g; }
    void SetGravityApply(bool apply) { gravityApply = apply; }
    bool GetGravityApply() const { return gravityApply; }

    virtual void SetRotation(float rotation) { this->rotation = rotation; }

    void SetUI(bool u) { isUi = u; }
    bool IsInView();
    const int& GetObjId() const;
    void SetId(int id) { this->id = id; }

    void SetName(string name) { this->name = name; }
    const string& GetName() const { return name; }

    void SetDirection(const Vector2f& dir) { direction = dir; }
    const Vector2f& GetDirection() const { return direction; }

    virtual void SetDevMode(bool devMode) { isDevMode = devMode; }
    virtual bool GetDevMode() const { return isDevMode; }
    virtual void SwitchDevMode() { isDevMode = !isDevMode; }
    virtual void SetHitBox(const FloatRect& rect, Color color = Color::Red);
    virtual void SetHitBox(string path);
    RectangleShape& GetHitBox() { return hitbox; }
    FloatRect GetHitBounds() const { return hitbox.getGlobalBounds(); }

    void SetObjType(ObjTypes type) { objtype = type; }
    ObjTypes GetObjType() { return objtype; }

    void SetLowHitBox(const FloatRect& rect, Color color = Color::White)
    {
        lowhitbox.setSize({ rect.width,rect.height });
        lowhitbox.setFillColor(color);
    }
};