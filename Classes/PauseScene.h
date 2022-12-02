#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"

class PauseScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
     cocos2d::Sprite* _background;

    virtual bool init();
    void MainMenu(Ref* pSender);
    void Return(Ref* pSender);
    bool onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);

    CREATE_FUNC(PauseScene);
};

#endif