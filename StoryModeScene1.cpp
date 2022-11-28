#include "StoryModeScene1.h"
#include "HelloWorldScene.h"
#include "PauseScene.h"
#include "DefeatScene.h"
#include <CCUserDefault.h>

USING_NS_CC;

Scene* StoryModeScene1::createScene()
{
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    auto layer = StoryModeScene1::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool StoryModeScene1::init()
{
    points = 0;

    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    _background = Sprite::create("BackgroundScene1.png");
    _player = Sprite::create("Player.png");
    _player->setScale(.5);
    if (_background == nullptr)
    {
        problemLoading("'BackgroundScene1.png'");
    }
    else
    {
        _background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_background, 0);
    }

    _player->setPosition(Vec2(visibleSize.width / 2, 150));

    auto _playerBody = PhysicsBody::createBox(_player->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    _playerBody->setCollisionBitmask(0);
    _playerBody->setContactTestBitmask(true);
    _playerBody->setDynamic(false);
    _player->setPhysicsBody(_playerBody);
    this->addChild(_player);

    auto _end = Sprite::create("Rectangle.png");
    _end->setScaleX(13);
    _end->setPosition(Point(visibleSize.width / 2, 20));
    _end->setVisible(false);
    auto _endBody = PhysicsBody::createBox(_end->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    _endBody->setDynamic(false);
    _endBody->setCollisionBitmask(5);
    _endBody->setContactTestBitmask(true);
    _end->setPhysicsBody(_endBody);
    this->addChild(_end);

    score = cocos2d::Label::createWithTTF("Score " + std::to_string(points), "fonts/ARCADECLASSIC.ttf", 60);
    score->setPosition(Point(visibleSize.width - 400, 50));
    this->addChild(score);

    auto listenerKeys = EventListenerKeyboard().create();
    listenerKeys->onKeyPressed = CC_CALLBACK_2(StoryModeScene1::onKeyPressed, this);
    listenerKeys->onKeyReleased = CC_CALLBACK_2(StoryModeScene1::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeys, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(StoryModeScene1::OnContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();
    this->schedule(CC_SCHEDULE_SELECTOR(StoryModeScene1::moveEnemy), 7, CC_REPEAT_FOREVER, 5);
    this->schedule(CC_SCHEDULE_SELECTOR(StoryModeScene1::moveEnemy2), 5, CC_REPEAT_FOREVER, 32);
    this->schedule(CC_SCHEDULE_SELECTOR(StoryModeScene1::moveEnemy3), 3, CC_REPEAT_FOREVER, 57);
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(StoryModeScene1::changeBackground), 25);
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(StoryModeScene1::changeBackground2), 50);

    return true;
}
s
void StoryModeScene1::changeBackground(float dt)
{
    _background->setTexture("BackgroundScene2.png");
}

void StoryModeScene1::changeBackground2(float dt)
{
    _background->setTexture("BackgroundScene3.png");
}

bool StoryModeScene1::playerOutOfBounds(int dx, int dy)
{
    Vec2 pos = _player->getPosition();
    return (pos.x + dx < 0 || pos.x + dx > visibleSize.width || pos.y + dy < 100 || pos.y + dy > visibleSize.height / 2 - 200);
}

void StoryModeScene1::initEnemy(Sprite* _e)
{
    _e->setScale(.35);
    _e->setPosition(Vec2(cocos2d::RandomHelper::random_int((unsigned int)(_e->getContentSize().width + 100), (unsigned int)(visibleSize.width - _e->getContentSize().width - 100)), visibleSize.height - 200));
    auto enemyBody = PhysicsBody::createBox(_e->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    enemyBody->setCollisionBitmask(2);
    enemyBody->setContactTestBitmask(true);
    enemyBody->setRotationEnable(false);
    enemyBody->setDynamic(false);
    _e->setPhysicsBody(enemyBody);
}

void StoryModeScene1::moveEnemy(float dt)
{
    auto _enemy = Sprite::create("Enemy1.png");
    initEnemy(_enemy);
    this->addChild(_enemy, 1);
    auto action = MoveTo::create(6, Point(_enemy->getPosition().x, 0));
    _enemy->runAction(action);
}

void StoryModeScene1::moveEnemy2(float dt)
{
    auto _enemy = Sprite::create("Enemy2.png");
    initEnemy(_enemy);
    this->addChild(_enemy, 1);
    auto action = MoveTo::create(4.5, Point(_enemy->getPosition().x, 0));
    _enemy->runAction(action);
}

void StoryModeScene1::moveEnemy3(float dt)
{
    auto _enemy = Sprite::create("Enemy3.png");
    initEnemy(_enemy);
    this->addChild(_enemy, 1);
    auto action = MoveTo::create(3, Point(_enemy->getPosition().x, 0));
    _enemy->runAction(action);
}

void StoryModeScene1::update(float delta)
{
    int speed = 1500;
    Vec2 loc = _player->getPosition();
    if (rightKeyPressed)
    {
        if(!playerOutOfBounds(speed * delta, 0))
            _player->setPositionX(loc.x + speed * delta);
    }
    if (leftKeyPressed)
    {
        if (!playerOutOfBounds(-speed * delta, 0))
            _player->setPositionX(loc.x - speed * delta);
    }
    if (upKeyPressed)
    {
        if (!playerOutOfBounds(0, speed * delta))
            _player->setPositionY(loc.y + speed * delta);
    }
    if (downKeyPressed)
    {
        if (!playerOutOfBounds(0, -speed * delta))
            _player->setPositionY(loc.y - speed * delta);
    }
}

void StoryModeScene1::initShot(cocos2d::Sprite* _sprite)
{
    _sprite->setScale(3);
    _sprite->setPosition(Vec2(_player->getPositionX(), _player->getPositionY()));
    _sprite->setAnchorPoint(Vec2(0.5, 0));
    auto Body = PhysicsBody::createBox((_sprite->getContentSize())*.1, PhysicsMaterial(PHYSICSBODY_MATERIAL_DEFAULT));
    Body->setCollisionBitmask(1);
    Body->setContactTestBitmask(true);
    Body->setDynamic(false);
    _sprite->addComponent(Body);
}

void StoryModeScene1::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    Sprite* _shot = Sprite::create("ShotPlayerSprite.png");
    initShot(_shot);

    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_D: case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            rightKeyPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_A: case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            leftKeyPressed = true;
            break;
        /*case EventKeyboard::KeyCode::KEY_W: case EventKeyboard::KeyCode::KEY_UP_ARROW:
            upKeyPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_S: case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            downKeyPressed = true;
            break;*/
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            Director::getInstance()->pushScene(PauseScene::createScene());
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            this->addChild(_shot, 1);

            auto move = Sequence::create(MoveTo::create(3, Point(_shot->getPositionX(), visibleSize.height + _shot->getContentSize().height)), CallFuncN::create(CC_CALLBACK_1(StoryModeScene1::doRemoveFromParent, this)), nullptr);
            _shot->runAction(move);
            break;
    }
};

void StoryModeScene1::doRemoveFromParent(Node* sender)
{
    sender->removeFromParent();
}

void StoryModeScene1::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    rightKeyPressed = false;
    leftKeyPressed = false;
    upKeyPressed = false;
    downKeyPressed = false;
}

bool StoryModeScene1::OnContactBegin(cocos2d::PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody();
    auto nodeB = contact.getShapeB()->getBody();
    int aBitMask = nodeA->getCollisionBitmask(), bBitMask = nodeB->getCollisionBitmask();
    if (aBitMask > bBitMask)
    {
        auto aux = nodeA;
        nodeA = nodeB;
        nodeB = aux;
    }

    auto die = FadeOut::create(1);
    auto sequence = Sequence::create(die, CallFuncN::create(CC_CALLBACK_1(StoryModeScene1::doRemoveFromParent, this)), nullptr);

    if ((1 == aBitMask && 2 == bBitMask) || (2 == aBitMask && 1 == bBitMask))
    {
        nodeA->getNode()->removeFromParent();
        nodeB->getNode()->stopAllActions();
        nodeB->getNode()->runAction(sequence);

        points += 100;
        score->setString("Score " + std::to_string(points));
    }
    else if ((1 == aBitMask && 3 == bBitMask) || (3 == aBitMask && 1 == bBitMask))
    {
        nodeA->getNode()->removeFromParent();
        nodeB->getNode()->stopAllActions();
        nodeB->getNode()->runAction(sequence);

        points += 250;
        score->setString("Score " + std::to_string(points));
    }
    else if ((1 == aBitMask && 4 == nodeB->getCollisionBitmask()) || (4 == aBitMask && 1 == nodeB->getCollisionBitmask()))//nodeB = personajes
    {
        nodeA->getNode()->removeFromParent();
        nodeB->getNode()->stopAllActions();
        nodeB->getNode()->runAction(sequence);
        points += 500;
        score->setString("Score " + std::to_string(points));
    }
    else if (((0 && 1) != aBitMask && 5 == nodeB->getCollisionBitmask()) || ((0 && 1) != nodeB->getCollisionBitmask() && 5 == aBitMask))
        endGame();
    else if (checkPlayerCollision(aBitMask, bBitMask))
        endGame();
   
    return true;
}

bool StoryModeScene1::checkPlayerCollision(int aBitMask, int bBitMask)
{
    if ((aBitMask == 2 || aBitMask == 3 || aBitMask == 4) && bBitMask == 0)
        return true;
    else if ((bBitMask == 2 || bBitMask == 3 || bBitMask == 4) && aBitMask == 0)
        return true;
    else
        return false;
}

void StoryModeScene1::endGame()
{
    this->unscheduleAllCallbacks();
    if(points > record->getIntegerForKey("Record"))
        record->setIntegerForKey("Record", points);
    Director::getInstance()->replaceScene(DefeatScene::createScene());
}