#include "GameLayer.h"
#include "Block.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define winSize CCDirector::sharedDirector()->getWinSize()
#define blockSize CCSize((winSize.width - 3) / 4, (winSize.height - 3) / 4)

static bool operator==(ccColor3B colorA, ccColor3B colorB){
	return colorA.r == colorB.r&&colorA.g == colorB.g&&colorA.b == colorB.b;
}

CCScene * GameLayer::scene(){
	CCScene * scene = CCScene::create();
	scene->addChild(create());
	return scene;
}

bool GameLayer::init(){
	continueAddBlock = false;
	isStart = false;
	startTime = 0;


	CCLayer::init();
	preloadBackgroundMusic();
	labelTime = CCLabelTTF::create();
	labelTime->setFontSize(35);
	labelTime->setString("0.00");
	labelTime->setColor(ccYELLOW);
	labelTime->setZOrder(100);
	labelTime->setPosition(ccp(winSize.width / 2, winSize.height - 35));
	addChild(labelTime);

	addStartBottomBlocks();
	addNormalBlocks(1);
	addNormalBlocks(2);
	addNormalBlocks(3);

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	return true;
}

void GameLayer::addNormalBlocks(int lineIndex){
	count++;
	int index = rand() % 4;
	for (int i = 0; i < 4; i++){
		Block * block;
		if (lineIndex == 1)
			block = Block::create(blockSize, i == index ? ccORANGE : ccWHITE, "Start", ccWHITE);
		else
			block = Block::create(blockSize, i == index ? ccORANGE : ccWHITE);
		block->setLineIndex(lineIndex);
		block->setPosition(ccp(i*winSize.width / 4, lineIndex*winSize.height / 4));
		addChild(block);
	}
}

void GameLayer::addStartBottomBlocks(){
	count = 0;
	for (int i = 0; i < 4; i++){
		Block * block = Block::create(blockSize, ccYELLOW);
		block->setLineIndex(0);
		block->setPosition(ccp(i*winSize.width / 4, 0));
		addChild(block);
	}
}

void GameLayer::addLastTopBlocks(){
	CCString * str = CCString::createWithFormat("Time:%.2f", startTime);
	Block * block = Block::create(winSize, ccGREEN);
	block->setLineIndex(4);
	block->setPosition(ccp(0, 4 * winSize.height / 4));
	addChild(block);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	CCArray* array = Block::getBlocks();
	CCObject* obj;
	CCARRAY_FOREACH(Block::getBlocks(), obj){
		Block* block = (Block*)obj;
		if ((block->getColor() == ccRED || block->getColor() == ccGREEN) && block->getLineIndex() == 0){
			restart();
			break;
		}

		if (block->getLineIndex() != 1 || !block->boundingBox().containsPoint(pTouch->getLocation()))
			continue;
		if (block->getColor() == ccORANGE){
			if (!isStart){
				scheduleUpdate();
				continueAddBlock = true;
				isStart = true;
			}

			playMusic();
			ccColor3B color;
			color.r = 255;
			color.g = 192;
			color.b = 203;
			block->setColor(color);
			if (count != 30 && continueAddBlock)
				addNormalBlocks(4);
			else if (continueAddBlock){
				addLastTopBlocks();
				continueAddBlock = false;
			}
			moveDown();
			if (count == 30 && ((Block*)Block::getBlocks()->lastObject())->getLineIndex() == 1)
				gameSuccess();
		}
		else if (block->getColor() == ccWHITE){
			if (isStart){
				unscheduleUpdate();
				isStart = false;
			}
			block->setColor(ccRED);
			scheduleOnce(schedule_selector(GameLayer::gameOver), 0.5);
		}
		break;
	}
	return false;
}

void GameLayer::moveDown(){
	CCObject* obj;
	Block* block;
	CCARRAY_FOREACH_REVERSE(Block::getBlocks(), obj){
		block = (Block*)obj;
		block->setLineIndex(block->getLineIndex() - 1);
		CCMoveTo * mv = CCMoveTo::create(0.06, ccp(block->getPositionX(), block->getPositionY() - winSize.height / 4));
		block->runAction(mv);
		if (block->getLineIndex() < -1){
			Block::getBlocks()->removeObject(block);
			block->removeFromParentAndCleanup(true);
		}
	}
}

void GameLayer::gameOver(float delay){
	Block * block = Block::create(winSize, ccRED, "Game over", ccWHITE);
	block->setLineIndex(4);
	block->setPosition(ccp(0, 4 * winSize.height / 4));
	addChild(block);
	for (int i = 0; i < 4; i++)
		moveDown();
}

void GameLayer::gameSuccess(){
	if (isStart){
		unscheduleUpdate();
		isStart = false;
	}
	moveDown();
	Block* block = (Block*)Block::getBlocks()->lastObject();
	CCLabelTTF * label = (CCLabelTTF *)block->getChildByTag(999);
	label->setColor(ccWHITE);
	label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	CCString * str = CCString::createWithFormat("Your Time:%.2f", startTime);
	label->setString(str->getCString());
}

void GameLayer::update(float delta){
	startTime += delta;
	CCString * str = CCString::createWithFormat("%.2f", startTime);
	labelTime->setString(str->getCString());
}

void GameLayer::restart(){
	startTime = 0;
	CCString * str = CCString::createWithFormat("0.00");
	labelTime->setString(str->getCString());

	CCObject* obj;
	Block* block;
	CCARRAY_FOREACH_REVERSE(Block::getBlocks(), obj){
		block = (Block*)obj;
		Block::getBlocks()->removeObject(block);
		block->removeFromParentAndCleanup(true);
	}

	addStartBottomBlocks();
	addNormalBlocks(1);
	addNormalBlocks(2);
	addNormalBlocks(3);
}

void GameLayer::playMusic(){
	//SimpleAudioEngine::sharedEngine()->playEffect("piano/soundA.m4a");
	int index = rand() % 25;
	CCString * path = CCString::createWithFormat("piano/sound%c.m4a", index + 65);
	SimpleAudioEngine::sharedEngine()->playEffect(path->getCString());
}


void GameLayer::preloadBackgroundMusic(){
	//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("piano/soundA.m4a");
	for (int i = 65; i <= 90; i++){
		CCString * path = CCString::createWithFormat("piano/sound%c.m4a", i);
		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(path->getCString());
	}
}