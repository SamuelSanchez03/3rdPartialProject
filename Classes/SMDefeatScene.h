#ifndef __S_M_DEFEAT_SCENE_H__
#define __S_M_DEFEAT_SCENE_H__

#include "cocos2d.h"

class SMDefeatScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
     cocos2d::Sprite* _background;
     cocos2d::Label* _label;
     bool labelState = false;
     cocos2d::UserDefault* record = cocos2d::UserDefault::getInstance();

    virtual bool init();
    void flickering(float dt);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
    
    CREATE_FUNC(SMDefeatScene);
};

#endif