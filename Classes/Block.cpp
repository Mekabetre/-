#include "Block.h"

CCArray * Block::allBlocks = NULL;
Block * Block::create(CCSize size, ccColor3B color, CCString str, ccColor3B strColor){
	if (allBlocks == NULL){
		allBlocks = CCArray::create();
		allBlocks->retain();
	}
	Block *pRet = new Block();
	if (pRet != NULL&&pRet->init(size, color, str, strColor)){
		pRet->autorelease();
		allBlocks->addObject(pRet);
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}
Block * Block::create(CCSize size, ccColor3B color){
	return create(size, color, "", color);
}
bool Block::init(CCSize size, ccColor3B color, CCString str, ccColor3B strColor){
	CCSprite::init();
	setContentSize(size);
	setTextureRect(CCRectMake(0, 0, size.width, size.height));
	setColor(color);
	ignoreAnchorPointForPosition(true);

	CCLabelTTF* label = CCLabelTTF::create();
	label->setTag(999);
	label->setFontSize(30);
	label->setString(str.getCString());
	label->setColor(strColor);
	label->setPosition(ccp(size.width / 2, size.height / 2));

	addChild(label);
	return true;
}

CCArray* Block::getBlocks(){
	return allBlocks;
}