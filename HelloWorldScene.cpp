#include "HelloWorldScene.h"
#include "TestScene.h"
#include "InfiniteModeScene.h"
#include "PauseScene.h"
#include "DefeatScene.h"
#include "StoryModeScene.h"
#include <CCUserDefault.h>
#include "LeaderBoardsScene.h"

USING_NS_CC;

extern int points = 0;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    points = 0;

    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _titleSprite = Sprite::create("TitleSprite.png");
    _background = Sprite::create("BackgroundMenuSp.png");
    if (_titleSprite == nullptr)
    {
        problemLoading("'Spaceship.png'");
    }
    else
    {
        _titleSprite->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height/7)*6));
        _background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height/2));
        this->addChild(_background, 0);
        this->addChild(_titleSprite, 1);

        auto menu_item_1 = MenuItemImage::create("InfiniteModePressed.png", "InfiniteMode.png", CC_CALLBACK_1(HelloWorld::InfiniteMode, this));
        auto menu_item_2 = MenuItemImage::create("StoryModePressed.png", "StoryMode.png", CC_CALLBACK_1(HelloWorld::StoryMode, this));
        auto menu_item_3 = MenuItemImage::create("LeaderBoardsPressed.png", "LeaderBoards.png", CC_CALLBACK_1(HelloWorld::LeaderBoards, this));
        auto menu_item_4 = MenuItemImage::create("ExitButtonPressed.png", "ExitButton.png", CC_CALLBACK_1(HelloWorld::Exit, this));

        menu_item_1->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6)*4));
        menu_item_2->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6)*3));
        menu_item_3->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6)*2));
        menu_item_4->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6)));

        auto menu = Menu::create(menu_item_1, menu_item_2, menu_item_3, menu_item_4, nullptr);
        menu->setPosition(Point(0, 0));

        this->addChild(menu, 1);
    }

    return true;
}


void HelloWorld::Exit(Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::InfiniteMode(Ref* pSender)
{
    Director::getInstance()->pushScene(InfiniteModeScene::createScene());
}

void HelloWorld::StoryMode(Ref* pSender)
{
    Director::getInstance()->replaceScene(StoryModeScene::createScene());
}

void HelloWorld::LeaderBoards(Ref* pSender)
{
    Director::getInstance()->replaceScene(LeaderBoardsScene::createScene());
}