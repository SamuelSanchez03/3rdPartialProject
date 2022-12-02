#include "VictoryScene.h"
#include "StoryModeScene1.h"
#include "HelloWorldScene.h"
#include <CCUserDefault.h>
#include "LevelCompletedScene.h"
#include <cocos/editor-support/cocostudio/SimpleAudioEngine.h>


USING_NS_CC;

Scene* VictoryScene::createScene()
{
    return VictoryScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool VictoryScene::init()
{

    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _background = Sprite::create("Victory.png");
    //_background->setScale(.9);
    if (_background == nullptr)
    {
        problemLoading("'DefeatScreen.png'");
    }
    else
    {
        _background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_background, 0);
    }

    auto _victoryLabel = Label::createWithTTF("Victory", "fonts/ARCADECLASSIC.ttf", 150);
    _victoryLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 200));
    _victoryLabel->setTextColor(Color4B::BLACK);
    this->addChild(_victoryLabel, 1);

    auto score = Label::createWithTTF("Your   time " + std::to_string(gameTime), "fonts/ARCADECLASSIC.ttf", 100);
    score->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    score->setTextColor(Color4B::BLACK);
    this->addChild(score);

    _label = Label::createWithTTF("Press   Enter   to   continue", "fonts/ARCADECLASSIC.ttf", 60);
    _label->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 200));
    _label->setTextColor(Color4B::BLACK);
    this->addChild(_label, 1);

    auto listenerKeys = EventListenerKeyboard().create();
    listenerKeys->onKeyPressed = CC_CALLBACK_2(VictoryScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeys, this);

    this->schedule(CC_SCHEDULE_SELECTOR(VictoryScene::flickering), .5, CC_REPEAT_FOREVER, .5);

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/VictoryMusic.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/VictoryMusic.mp3", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(.5);

    return true;
}

void VictoryScene::flickering(float dt)
{
    if (labelState == false)
        _label->setString("");
    else
        _label->setString("Press   Enter   to   continue");

    labelState = !labelState;
}

void VictoryScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
    {
        this->unscheduleAllCallbacks();
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        Director::getInstance()->replaceScene(HelloWorld::createScene());
    }
}