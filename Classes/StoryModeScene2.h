#ifndef __STORY_MODE_SCENE_2_H__
#define __STORY_MODE_SCENE_2_H__

#include "cocos2d.h"

class StoryModeScene2 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
     cocos2d::Sprite* _background;
     cocos2d::Sprite* _player;
     cocos2d::Sprite* _boss;
     cocos2d::Sprite* laser;
     cocos2d::Sprite* fireBall;
     cocos2d::Size visibleSize;
     cocos2d::Node* edgeNode;
     cocos2d::Vec2 origin;
     bool leftKeyPressed = false;
     bool rightKeyPressed = false;
     bool upKeyPressed = false;
     bool downKeyPressed = false;
     cocos2d::Label* time;
     cocos2d::UserDefault* record = cocos2d::UserDefault::getInstance();
     int bossLife = 75;
     int playerHearts = 5;
     cocos2d::Vector<cocos2d::Sprite*> heartsSprites;
     bool isBossAttacking = false;
     int id;

    virtual bool init();

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
    virtual void update(float delta) override;

    bool playerOutOfBounds(int dx, int dy);

    bool OnContactBegin(cocos2d::PhysicsContact& contact);
    void initShot(cocos2d::Sprite* _sprite);
    void initEnemy(cocos2d::Sprite* _e, int bMask);
    void moveEnemy(float dt);
    void endGame();
    bool checkPlayerCollision(int aBitMask, int bBitMask);
    void doRemoveFromParent(cocos2d::Node* sender);
    void initBoss(float dt);
    void initBossPhysics(cocos2d::Sprite* _b);
    void bossAttack1(float dt);
    void bossAttack2(float dt);
    void stopLaser();
    void levelCompleted();
    void Continue(Ref* pSender);

    CREATE_FUNC(StoryModeScene2);

private:
    cocos2d::PhysicsWorld* sceneWorld;
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
};

#endif
