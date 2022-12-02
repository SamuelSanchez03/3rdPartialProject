#ifndef __LEADER_BOARDS_SCENE_H__
#define __LEADER_BOARDS_SCENE_H__

#include "cocos2d.h"

class LeaderBoardsScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
     cocos2d::Sprite* _background;
     cocos2d::Label* _label;
     bool labelState = false;
     cocos2d::UserDefault* record = cocos2d::UserDefault::getInstance();

    virtual bool init();
    void Back(Ref* pSender);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
    
    CREATE_FUNC(LeaderBoardsScene);
};

#endif