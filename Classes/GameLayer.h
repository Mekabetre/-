#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_
#include "cocos2d.h"
USING_NS_CC;

class GameLayer : public CCLayer
{
public:
	static CCScene * scene();
	CREATE_FUNC(GameLayer);
	bool init();

	void addNormalBlocks(int lineIndex);
	void addStartBottomBlocks();
	void addLastTopBlocks();
	void moveDown();
	void gameOver(float delay);
	void gameSuccess();
	void restart();
	void preloadBackgroundMusic();
	void playMusic();

	void update(float delta);
	CCLabelTTF * labelTime;

	int count;

	bool continueAddBlock;
	bool isStart;
	float startTime;

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

#endif