#ifndef __INFINITE_MODE_SCENE_H__
#define __INFINITE_MODE_SCENE_H__

#include "cocos2d.h"

class InfiniteModeScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
     cocos2d::Sprite* _background;
     cocos2d::Sprite* _player;
     cocos2d::Size visibleSize;
     cocos2d::Node* edgeNode;
     cocos2d::Vec2 origin;
     bool leftKeyPressed = false;
     bool rightKeyPressed = false;
     bool upKeyPressed = false;
     bool downKeyPressed = false;
     cocos2d::Label* score;
     cocos2d::UserDefault* record = cocos2d::UserDefault::getInstance();

    virtual bool init();

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
    virtual void update(float delta) override;

    bool playerOutOfBounds(int dx, int dy);

    bool OnContactBegin(cocos2d::PhysicsContact& contact);
    void initShot(cocos2d::Sprite* _sprite);
    void initEnemy(cocos2d::Sprite* _e);
    void moveEnemy(float dt);
    void moveEnemy2(float dt);
    void moveEnemy3(float dt);
    void changeBackground(float dt);
    void changeBackground2(float dt);
    void endGame();
    bool checkPlayerCollision(int aBitMask, int bBitMask);
    void checkBulletCollision(int aBitMask, int bBitMask);
    void doRemoveFromParent(cocos2d::Node* sender);
    
    CREATE_FUNC(InfiniteModeScene);

private:
    cocos2d::PhysicsWorld* sceneWorld;
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
};

#endif
