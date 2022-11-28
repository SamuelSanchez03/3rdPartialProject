#include "TestScene.h"
#include "InfiniteModeScene.h"
#include "DefeatScene.h"
#include "HelloWorldScene.h"
#include <CCUserDefault.h>
#include "LeaderBoardsScene.h"


USING_NS_CC;

Scene* LeaderBoardsScene::createScene()
{
    return LeaderBoardsScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool LeaderBoardsScene::init()
{

    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _background = Sprite::create("BackgroundLeaderBoards.png");
    if (_background == nullptr)
    {
        problemLoading("'BackgroundLeaderBoards.png'");
    }
    else
    {
        _background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_background, 0);
    }

    auto _defeatLabel = Label::createWithTTF("Records", "fonts/ARCADECLASSIC.ttf", 300);
    _defeatLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 200));
    this->addChild(_defeatLabel, 1);

    auto _recordLabel = Label::createWithTTF("Highest   Score   " + std::to_string(record->getIntegerForKey("Record")), "fonts/ARCADECLASSIC.ttf", 200);
    _recordLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 200));
    this->addChild(_recordLabel);


    auto _backLabel = Label::createWithTTF("Back", "fonts/ARCADECLASSIC.ttf", 75);
    auto menu_item_1 = MenuItemLabel::create(_backLabel, CC_CALLBACK_1(LeaderBoardsScene::Back, this));
    menu_item_1->setPosition(Point((visibleSize.width / 12), visibleSize.height-150));

    auto menu = Menu::create(menu_item_1, nullptr);
    menu->setPosition(Point(0, 0));
    this->addChild(menu, 1);

    return true;
}

void LeaderBoardsScene::Back(Ref* pSender)
{
    Director::getInstance()->replaceScene(HelloWorld::createScene());
}