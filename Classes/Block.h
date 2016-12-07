#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "cocos2d.h"
USING_NS_CC;

class Block : public CCSprite
{
	static CCArray * allBlocks;
public:
	static Block * create(CCSize size, ccColor3B color, CCString str, ccColor3B strColor);
	static Block * create(CCSize size, ccColor3B color);
	bool init(CCSize size, ccColor3B color, CCString str, ccColor3B strColor);

	CC_SYNTHESIZE(int, _lineIndex, LineIndex);
	static CCArray* getBlocks();
};

#endif