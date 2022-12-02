#include "TestScene.h"
#include "InfiniteModeScene.h"
#include "DefeatScene.h"
#include "HelloWorldScene.h"
#include <CCUserDefault.h>
#include <cocos/editor-support/cocostudio/SimpleAudioEngine.h>


USING_NS_CC;

Scene* DefeatScene::createScene()
{
    return DefeatScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool DefeatScene::init()
{

    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _background = Sprite::create("DefeatScreen.png");
    if (_background == nullptr)
    {
        problemLoading("'DefeatScreen.png'");
    }
    else
    {
        _background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_background, 0);
    }

    auto _defeatLabel = Label::createWithTTF("Defeat", "fonts/ARCADECLASSIC.ttf", 150);
    _defeatLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 200));
    this->addChild(_defeatLabel, 1);

    auto score = Label::createWithTTF("Your   score " + std::to_string(points), "fonts/ARCADECLASSIC.ttf", 100);
    score->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(score);

    _label = Label::createWithTTF("Press   Enter   to   continue", "fonts/ARCADECLASSIC.ttf", 60);
    _label->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 200));
    this->addChild(_label, 1);

    auto listenerKeys = EventListenerKeyboard().create();
    listenerKeys->onKeyPressed = CC_CALLBACK_2(DefeatScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeys, this);

    this->schedule(CC_SCHEDULE_SELECTOR(DefeatScene::flickering), .5, CC_REPEAT_FOREVER, .5);

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/DefeatSceneMusic.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/DefeatSceneMusic.mp3", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(.5);

    return true;
}

void DefeatScene::flickering(float dt)
{
    if (labelState == false)
        _label->setString("");
    else
        _label->setString("Press   Enter   to   continue");

    labelState = !labelState;
}

void DefeatScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        this->unscheduleAllCallbacks();
        Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));
    }
}