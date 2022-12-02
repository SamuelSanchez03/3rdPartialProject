#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

extern int points;
extern int sceneNumber;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
     cocos2d::Sprite* _titleSprite;
     cocos2d::Sprite* _background;

    virtual bool init();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
    void InfiniteMode(Ref* pSender);
    void StoryMode(Ref* pSender);
    void LeaderBoards(Ref* pSender);
    void Exit(Ref* pSender);

    CREATE_FUNC(HelloWorld);
};

#endif
