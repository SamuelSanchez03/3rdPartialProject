#include "PauseScene.h"
#include "InfiniteModeScene.h"
#include "HelloWorldScene.h"
#include <cocos/editor-support/cocostudio/SimpleAudioEngine.h>

USING_NS_CC;

Scene* PauseScene::createScene()
{
    return PauseScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool PauseScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _background = Sprite::create("PauseBackground.png");
    if (_background == nullptr)
    {
        problemLoading("'PauseBackground.png'");
    }
    else
    {
        _background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_background, 0);
    }

    auto pauseSprite = Sprite::create("PauseSprite.png");
    pauseSprite->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 6) * 4));
    this->addChild(pauseSprite, 1);

    auto mainMenu_button = MenuItemImage::create("MainMenu.png", "MainMenuPressed.png", CC_CALLBACK_1(PauseScene::MainMenu, this));
    mainMenu_button->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 6) * 3));
    auto return_button = MenuItemImage::create("Return.png", "ReturnPressed.png", CC_CALLBACK_1(PauseScene::Return, this));
    return_button->setPosition(Vect(visibleSize.width / 2, (visibleSize.height / 6) * 2));

    auto menu = Menu::create(mainMenu_button, return_button, nullptr);
    menu->setPosition(Point(0, 0));
    this->addChild(menu, 1);

    auto listenerKeys = EventListenerKeyboard().create();
    listenerKeys->onKeyPressed = CC_CALLBACK_2(PauseScene::onKeyPressed, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeys, this);

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/ButtonPressed.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);

    return true;
}

void PauseScene::MainMenu(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonPressed.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(HelloWorld::createScene());
}

void PauseScene::Return(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonPressed.mp3");
    Director::getInstance()->popScene();
}

bool PauseScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event)
{
    if (key == EventKeyboard::KeyCode::KEY_ESCAPE)
        Director::getInstance()->popScene();
    return true;
}
