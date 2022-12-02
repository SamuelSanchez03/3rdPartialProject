#include "StoryModeScene2.h"
#include "HelloWorldScene.h"
#include "PauseScene.h"
#include "DefeatScene.h"
#include "VictoryScene.h"
#include "StoryModeScene1.h"
#include "StoryModeScene3.h"
#include "LevelCompletedScene.h"
#include <CCUserDefault.h>
#include "SMDefeatScene.h"
#include <cocos/editor-support/cocostudio/SimpleAudioEngine.h>

USING_NS_CC;

Scene* StoryModeScene2::createScene()
{
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    auto layer = StoryModeScene2::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool StoryModeScene2::init()
{
    points = 0;

    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    _background = Sprite::create("BackgroundScene2.png");
    _player = Sprite::create("Player.png");
    _player->setScale(.35);
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

    time = Label::createWithTTF(std::to_string(gameTime), "fonts/ARCADECLASSIC.ttf", 75);
    time->setPosition(Point(75, visibleSize.height - 100));
    this->addChild(time);

    for (int i = 0; i < 5; i++)
    {
        auto heart = Sprite::create("Heart.png");
        heart->setPosition(Point(100 + 75 * i, 50));
        this->addChild(heart);
        heartsSprites.pushBack(heart);
    }

    auto listenerKeys = EventListenerKeyboard().create();
    listenerKeys->onKeyPressed = CC_CALLBACK_2(StoryModeScene2::onKeyPressed, this);
    listenerKeys->onKeyReleased = CC_CALLBACK_2(StoryModeScene2::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeys, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(StoryModeScene2::OnContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();

    this->schedule(CC_SCHEDULE_SELECTOR(StoryModeScene2::moveEnemy), 2, CC_REPEAT_FOREVER, 1);
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(StoryModeScene2::initBoss), 30);

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/Level2Music.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/Level2Music.mp3", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(.5);

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/PlayerShot.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/EnemyDead.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/PlayerHit.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/BossHit.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/LaserAttack.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/BossScream.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/FireBallAttack.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);

    return true;
}

bool StoryModeScene2::playerOutOfBounds(int dx, int dy)
{
    Vec2 pos = _player->getPosition();
    return (pos.x + dx < 0 || pos.x + dx > visibleSize.width || pos.y + dy < 25 || pos.y + dy > visibleSize.height-115);
}

void StoryModeScene2::initEnemy(cocos2d::Sprite* _e, int bMask)
{
    _e->setScale(.35);
    _e->setPosition(Vec2(cocos2d::RandomHelper::random_int((unsigned int)(_e->getContentSize().width + 100), (unsigned int)(visibleSize.width - _e->getContentSize().width - 100)), visibleSize.height - 200));
    auto enemyBody = PhysicsBody::createBox(_e->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    enemyBody->setCollisionBitmask(bMask);
    enemyBody->setContactTestBitmask(true);
    enemyBody->setRotationEnable(false);
    enemyBody->setDynamic(false);
    _e->setPhysicsBody(enemyBody);
}

void StoryModeScene2::moveEnemy(float dt)
{
    auto _enemy = Sprite::create("Enemy2.png");
    auto _enemy2 = Sprite::create("Enemy2.png");
    initEnemy(_enemy, 2);
    this->addChild(_enemy, 1);
    auto action = MoveTo::create(4, Point(_enemy->getPosition().x, 0));
    _enemy->runAction(action);
}

void StoryModeScene2::update(float delta)
{
    int speed = 1200;
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

    if (bossLife == 0)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/BossScream.mp3");
        levelCompleted();
    }

    if (playerHearts < 5 && playerHearts > 0)
    {
        heartsSprites.at(playerHearts)->setColor(Color3B::BLACK);
    }

    if (playerHearts <= 0)
    {
        endGame();
    }

    gameTime += 60*delta;
    time->setString(std::to_string(gameTime));
}

void StoryModeScene2::initBossPhysics(cocos2d::Sprite* _b)
{
    _b->setScale(1.5);
    _b->setPosition(Point(visibleSize.width / 2, visibleSize.height));
    auto bossBody = PhysicsBody::createBox(_b->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    bossBody->setCollisionBitmask(3);
    bossBody->setContactTestBitmask(true);
    bossBody->setRotationEnable(false);
    bossBody->setDynamic(false);
    _b->setPhysicsBody(bossBody);
}

void StoryModeScene2::initBoss(float dt)
{
    _boss = Sprite::create("Boss2.png");
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/BossScream.mp3");
    initBossPhysics(_boss);
    this->addChild(_boss, 1);
    _boss->runAction(MoveBy::create(2, Point(0, -250)));
    this->schedule(CC_SCHEDULE_SELECTOR(StoryModeScene2::bossAttack1), 9, CC_REPEAT_FOREVER, 3);
    this->schedule(CC_SCHEDULE_SELECTOR(StoryModeScene2::bossAttack2), 9, CC_REPEAT_FOREVER, 9);
    //this->scheduleOnce(CC_SCHEDULE_SELECTOR(StoryModeScene2::bossAttack2), 4);
}

void StoryModeScene2::bossAttack1(float dt)
{
    id = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/LaserAttack.mp3", true);
    int angle;
    bool sign = cocos2d::RandomHelper::random_int(0, 1);
    (sign == 0) ? angle = 50 : angle = -50;
    this->unschedule(CC_SCHEDULE_SELECTOR(StoryModeScene2::moveEnemy));
    laser = Sprite::create("Laser.png");
    laser->setScale(.01);
    laser->setAnchorPoint(Vec2(.5, 1));
    laser->setPosition(Point(visibleSize.width / 2, visibleSize.height - (_boss->getContentSize().height)));
    
    auto laserBody = PhysicsBody::createBox(laser->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    laserBody->setDynamic(false);
    laserBody->setCollisionBitmask(4);
    laserBody->setContactTestBitmask(true);
    laserBody->setRotationEnable(true);
    laser->setPhysicsBody(laserBody);

    laser->setRotation(angle);
    this->addChild(laser);
    auto strecht = ScaleBy::create(1, 1, 50);
    auto unstrecht = ScaleBy::create(2, 1, 1 / 50);
    auto rotate1 = RotateBy::create(2, -2*angle);
    auto rotate2 = RotateBy::create(1, 2*angle);
    auto fullAttack = Sequence::create(strecht, rotate1, rotate2, unstrecht, CallFuncN::create(CC_CALLBACK_1(StoryModeScene2::doRemoveFromParent, this)), CallFuncN::create(CC_CALLBACK_0(StoryModeScene2::stopLaser, this)), nullptr);
    laser->runAction(fullAttack);
}

void StoryModeScene2::stopLaser()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(id);
}

void StoryModeScene2::bossAttack2(float dt)
{
    int angle;
    bool sign = cocos2d::RandomHelper::random_int(0, 1);
    (sign == 0) ? angle = 1 : angle = -1;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/FireBallAttack.mp3");
    fireBall = cocos2d::Sprite::create("FireBall.png");
    fireBall->setScale(.25);
    fireBall->setPosition(Point(_boss->getPosition().x, _boss->getPosition().y-150));
    auto fireBallBody = PhysicsBody::createCircle(fireBall->getContentSize().height / 2, PHYSICSBODY_MATERIAL_DEFAULT);
    fireBallBody->setCollisionBitmask(6);
    fireBallBody->setContactTestBitmask(true);
    fireBallBody->setDynamic(false);
    fireBall->setPhysicsBody(fireBallBody);
    this->addChild(fireBall, 1);

    auto pPos = _player->getPosition();
    auto out = MoveTo::create(.5, Point(visibleSize.width/2 + (angle * 750), visibleSize.height - 1.5 * _boss->getContentSize().height));
    auto move = MoveTo::create(.5, Point(pPos.x, pPos.y));
    auto fireBallAttack = Sequence::create(out, move, CallFuncN::create(CC_CALLBACK_1(StoryModeScene2::doRemoveFromParent, this)), nullptr);
    fireBall->runAction(fireBallAttack);
}

void StoryModeScene2::initShot(cocos2d::Sprite* _sprite)
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

void StoryModeScene2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    cocos2d::Sprite* _shot = Sprite::create("ShotPlayerSprite.png");
    initShot(_shot);

    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_D: case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            rightKeyPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_A: case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            leftKeyPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_W: case EventKeyboard::KeyCode::KEY_UP_ARROW:
            upKeyPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_S: case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            downKeyPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            Director::getInstance()->pushScene(PauseScene::createScene());
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            this->addChild(_shot, 1);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/PlayerShot.mp3");
            auto move = Sequence::create(MoveTo::create(3, Point(_shot->getPositionX(), visibleSize.height + _shot->getContentSize().height)), CallFuncN::create(CC_CALLBACK_1(StoryModeScene2::doRemoveFromParent, this)), nullptr);
            _shot->runAction(move);
            break;
    }
};

void StoryModeScene2::doRemoveFromParent(Node* sender)
{
    sender->removeFromParent();
}

void StoryModeScene2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    rightKeyPressed = false;
    leftKeyPressed = false;
    upKeyPressed = false;
    downKeyPressed = false;
}

bool StoryModeScene2::OnContactBegin(cocos2d::PhysicsContact& contact)
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
    auto sequence = Sequence::create(die, CallFuncN::create(CC_CALLBACK_1(StoryModeScene2::doRemoveFromParent, this)), nullptr);

    auto toRed = TintTo::create(.5, 200, 0, 0);
    auto toNormal = TintTo::create(.5, 255, 255, 255);
    auto gotHit = Sequence::create(toRed, toNormal, nullptr);

    if ((1 == aBitMask && 2 == bBitMask) || (2 == aBitMask && 1 == bBitMask))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/EnemyDead.mp3");
        if (nodeA->getNode() != nullptr)
              nodeA->getNode()->removeFromParent();
        nodeB->getNode()->stopAllActions();
        nodeB->removeFromWorld();
        nodeB->getNode()->runAction(sequence);
    }
    else if ((1 == aBitMask && 3 == bBitMask) || (3 == aBitMask && 1 == bBitMask))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/BossHit.mp3");
        if (nodeA->getNode() != nullptr)
            nodeA->getNode()->removeFromParent();
        nodeB->getNode()->runAction(gotHit);
        bossLife--;
    }
    else if ((0 == aBitMask && 6 == bBitMask) || (6 == aBitMask && 0 == bBitMask))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/PlayerHit.mp3");
        nodeB->getNode()->removeFromParent();
        playerHearts--;
    }
    else if ((0 == aBitMask && 4 == bBitMask) || (4 == aBitMask && 0 == bBitMask))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/PlayerHit.mp3");
        playerHearts--;
    }
    else if ((1 == aBitMask && 4 == bBitMask) || (1 == bBitMask && 4 == aBitMask))
    {
        if (nodeA->getNode() != nullptr)
            nodeA->getNode()->removeFromParent();
    }
    else if ((2 == aBitMask && 5 == bBitMask) || (2 == bBitMask && 5 == aBitMask))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/PlayerHit.mp3");
        playerHearts--;
        nodeA->getNode()->removeFromParent();
    }
    else if (checkPlayerCollision(aBitMask, bBitMask))
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/PlayerHit.mp3");
        if (nodeB->getNode() != nullptr && nodeB->getCollisionBitmask() == 2)
            nodeB->getNode()->removeFromParent();
        playerHearts--;
    }
   
    return true;
}

bool StoryModeScene2::checkPlayerCollision(int aBitMask, int bBitMask)
{
    if ((aBitMask == 2 || aBitMask == 3) && bBitMask == 0)
        return true;
    else if ((bBitMask == 2 || bBitMask == 3) && aBitMask == 0)
        return true;
    else
        return false;
}

void StoryModeScene2::endGame()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    this->unscheduleAllCallbacks();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    //if(points > record->getIntegerForKey("Record"))
       // record->setIntegerForKey("Record", points);
    Director::getInstance()->replaceScene(TransitionFade::create(1, SMDefeatScene::createScene()));
}

void StoryModeScene2::levelCompleted()
{
    this->unscheduleAllCallbacks();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(TransitionProgressHorizontal::create(1, LevelCompletedScene::createScene()));
}