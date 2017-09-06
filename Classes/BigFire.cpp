#include "BigFire.h"
#include "GameLayer.h"

bool BigFire::init(int num, bool inGame)
{
	CCMenuItemImage::init();

	_inGame = inGame;

	if (num == 0)
	{
		initWithNormalImage(IMAGE_bigFireButton, IMAGE_bigFireButton2, "", this, menu_selector(BigFire::fire));
		if (inGame)
		{
			setScale(0.6);
			setPosition(850, -100);
		}
		else
			setPosition(ccp(790, 200));

	}
	else if (num == 1)
	{
		initWithNormalImage("paddle/FireButton1.png", "paddle/FireButton2.png", "", this, menu_selector(BigFire::fire1));
		if (inGame)
		{
			setScale(0.3);
			setPosition(700, -200);
		}
		else
		{
			setScale(0.5);
			setPosition(ccp(800, -150));
		}
	}
	else if (num == 2)
	{
		initWithNormalImage("tank/bullet/1006-1.png", "tank/bullet/1006-2.png", "", this, menu_selector(BigFire::fire2));
		setScale(0.8);
		setPosition(ccp(580, -400));
	}
	return true;
}

void BigFire::fire(CCObject*)
{
	if (_inGame)
	{
		GameLayer* game = (GameLayer*)getParent()->getParent();
		if (game->_canBigFire1)//����д���
		{
			game->shootBigFire1();
			return;
		}
	}
	/*
	�����м����ط���Ҫע��1��������CCTexture2D�����棬�ٴ�������֡���������Խ�Լ�ڴ棬��Ҳ����ֱ��������ע�͵Ļ���һ��͸㶨
						  2���ڽ��о���֡�ü���ʱ��һ��Ҫע�⣬���õ���UI���꣬������openGL����
	*/
	CCArray* arr = CCArray::create();
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(IMAGE_People);
	for (int i = 0; i < 4; i++)
	{
		CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture,CCRect(0, i * 250, 250, 250));
		arr->addObject(frame);
		//CCSpriteFrame* frame = CCSpriteFrame::create(IMAGE_People, CCRect(0, i * 250, 250, 250));
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, 0.15f);
	CCAnimate* animate = CCAnimate::create(animation);
	CCSprite* s = CCSprite::create();
	getParent()->getParent()->addChild(s);//���뵽GameLayer��
	s->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	s->setScale(1.5);

	if (!_inGame)
		s->setScale(2.5);

	s->runAction(CCSequence::create(animate, CCRemoveSelf::create(), NULL));
}

void BigFire::fire1(CCObject*)
{
	if (_inGame)
	{
		GameLayer* game = (GameLayer*)getParent()->getParent();
		if (game->_canBigFire2)//����д���
		{
			game->shootBigFire2();
			return;
		}
		return;
	}

	CCArray* arr = CCArray::create();
	CCTexture2D* text;
	CCSpriteFrame* frame;
	for (int i = 0; i <= 5; i++)
	{
		text = CCTextureCache::sharedTextureCache()->addImage(Util::format(i,"tank/bullet/",".png"));

		frame = CCSpriteFrame::createWithTexture(text, CCRect(0, 0, 150, 600));

		arr->addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, 0.1);
	CCAnimate* animate = CCAnimate::create(animation);

	CCSprite* s = CCSprite::create();
	getParent()->getParent()->addChild(s);
	s->setPosition(ccp(winSize.width/2, winSize.height/2));
	
	if (!_inGame)
		s->setScale(1.8);

	s->runAction(CCSequence::create(animate,CCRemoveSelf::create(),NULL));
}
void BigFire::fire2(CCObject*)
{
	GameLayer* game = (GameLayer*)getParent()->getParent();
	if (game->_canBigFire3)//����д���
	{
		game->shootBigFire3();
		return;
	}
}