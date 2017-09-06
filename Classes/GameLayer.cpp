#include "GameLayer.h"
#include "Bullet.h"
#include "MenuLayer.h"
#include "Music.h"
#include "QuitDialog.h"
#include "StartLayer.h"
#include "SceneScore.h"

float GameLayer::winHeight;
bool GameLayer::_isFullScreamTouch;

//int GameLayer::_canBigFire1 = getFire1();
//int GameLayer::_canBigFire2 = getFire2();
//int GameLayer::_canBigFire3 = getFire3();

int GameLayer::_canBigFire1;
int GameLayer::_canBigFire2;
int GameLayer::_canBigFire3;

GameLayer* GameLayer::create(int level, bool isFullScreamTouch)
{
	_isFullScreamTouch = isFullScreamTouch;
	GameLayer* g = new GameLayer;
	g->_level = level;
	g->autorelease();
	return g;
}

void GameLayer::onEnter()
{
	CCLayer::onEnter();

	//��������
	Music::stopBackgroundMusic();
	Music::StartGameMusic();

	// ��ʼ����ͼ
	initMap();

	//����Ӣ��
	initHero();

	//��ʼ������
	initWife();

	//��ʼ����Ϸ��ť(��������ģʽ��ʼ��Ϸ)
	if (_isFullScreamTouch)
		fullScreamTouch();
	else
		nofullScreamTouch();

	//��ʼ���з�tank���ӵ�
	initAI();

	//����һ�����ذ�ť
	initReturn();

	//������ʱ�� ����ת��tank���򣬿���Ϸ�Ƿ����,����ӵ�
	scheduleUpdate();

	//Label��ʾ
	initshowItem();

	//����
	_props = CCArray::create();
	_props->retain();

	//����Ҫɱ���ĵ���
	if (MenuLayer::_mapIndex + 1 <= 2)//ǰ2�����õļ�Щ
		_needKilled = 10;
	else
		_needKilled = (MenuLayer::_mapIndex + 1) / 5 * 10 + 30;
	_killedTank = 666;//��ֹ�޸�

	//�����
	_isStrongFire = false;

	
	//��ʼ�������ӵ���ť
	initfire();

	/*�����пؼ�������ͬһ��λ����*/
	winHeight = winSize.height / 2;

	/*��ȡ���еĸ���*/
	_canBigFire1 = getFire1();
	_canBigFire2 = getFire2();
	_canBigFire3 = getFire3();

	//��ʼ������  Ҫ������ʼ���������˺ü��ο�
	initBigFire();
}

void GameLayer::onExit()
{
	CCLayer::onExit();
	_props->release();
	_bigFires1->release();
	_bigFires2->release();
	_bigFires3->release();
	_hero->_bullets->release();
}

void GameLayer::initBigFire()
{
	//����1����
	_bigFires1 = CCArray::create();
	_bigFires1->retain();
	_bigFireCount1 = 0;

	//����2����
	_bigFires2 = CCArray::create();
	_bigFires2->retain();
	_bigFireCount2 = 0;

	//����3����
	_bigFires3 = CCArray::create();
	_bigFires3->retain();
	_bigFireCount3 = 0;

	//�������о���
	CCSprite* s;
	for (int i = 0; i < _canBigFire1; ++i)
	{
		s = CCSprite::create(IMAGE_bigFire);
		_bigFires1->addObject(s);
	}

	for (int i = 0; i < _canBigFire2; ++i)
	{
		s = CCSprite::create(IMAGE_bigFire2);
		_bigFires2->addObject(s);
	}

	for (int i = 0; i < _canBigFire3; ++i)
	{
		s = CCSprite::create(IMAGE_bigFire3);
		_bigFires3->addObject(s);
	}

	//�ڶ�����������������һ�������磬���������ǰ���뵱ǰ������ռʱ���������ѣ���������Ŀ�ģ���Լ����ͼƬʱ��
	CCTexture2D* text = CCTextureCache::sharedTextureCache()->addImage(IMAGE_bigFire2);
	_sL = CCSprite::createWithTexture(text);
	_sR = CCSprite::createWithTexture(text);
	addChild(_sL);
	addChild(_sR);
	_sL->setScale(2);
	_sR->setScale(2);
}

void GameLayer::initMap()
{
	//������ͼ
	_map = CCTMXTiledMap::create(Util::format(_level, MAP_Round, ".tmx"));

	//�ѵ�ͼ���뵽Game����
	addChild(_map);

	//��������
	_xuanwo = CCSprite::create(IMAGE_nextLevel);
	addChild(_xuanwo);
	int x, y;
	if (MenuLayer::_mapIndex + 1 <= 1)//1�����õļ�Щ
	{
		x = 2;
		y = 2;
	}
	else
	{
		x = 40 * CCRANDOM_0_1();
		y = 40 * CCRANDOM_0_1() + 20;
	}
	_xuanwo->setPosition(ccp(48 * x, 48 * y));
	_xuanwo->setZOrder(-1);
	_xuanwo->runAction(CCRepeatForever::create(CCRotateBy::create(1, 100)));
}

void GameLayer::initHero()
{
	// ����һ���Ѿ�̹�ˣ�����FRIEND��ɱ������1�� ����ֵ��3�� ״̬��ͨ����������
	_hero = Tank::create(Tank::FRIEND, 1, 5, Tank::NORMAL, Tank::U);

	_hero->_bullets = CCArray::create();
	_hero->_bullets->retain();

	_hero->setTag(666);//Ϊ��ai�������Ӣ��

	//���뵽Map��  Ŀ�ģ����ô�������tank  tankֱ�ӻ�ȡ���ڵ������
	_map->addChild(_hero);

	_hero->setPosition(ccp(17 * 48, 1 * 48));
	_heroDir = Tank::NONE;
}

void GameLayer::initfire()
{
	Panel* menu = Panel::create(true);
	addChild(menu);
	//���뷢���ӵ��İ�ť
	CCMenuItemImage* fireitem = CCMenuItemImage::create(IMAGE_fire, IMAGE_fire2);
	menu->addChild(fireitem);
	_fireitem = fireitem;

	fireitem->setTarget(this, menu_selector(GameLayer::fireCallBack));
	fireitem->setScale(2.8);
	fireitem->setPosition(ccp(820, -410));

	//����1
	_b1 = BigFire::create();
	menu->addChild(_b1);

	//����2
	_b2 = BigFire::create(1);
	menu->addChild(_b2);

	//����3
	_b3 = BigFire::create(2);
	menu->addChild(_b3);
}

void  GameLayer::fireCallBack(CCObject*)
{
	_hero->fire();

	//��������
	Music::shootedMusic();
}

void GameLayer::DirCallback(CCObject* sender)
{
	CCMenuItem* item = (CCMenuItem*)sender;
	_heroDir = (Tank::DIR)(item->getTag() - 1000);//��int ת��Ϊ TYPE  ��ֵ������_heroDir  �Ӷ��ﵽ���Ʒ���
}

void GameLayer::update(float t)
{
#ifdef WIN32
	SHORT status = GetKeyState('A');
	if (status < 0)
	{
		_heroDir = Tank::L;
	}
	status = GetKeyState('W');
	if (status < 0)
	{
		_heroDir = Tank::U;
	}
	status = GetKeyState('S');
	if (status < 0)
	{
		_heroDir = Tank::D;
	}
	status = GetKeyState('D');
	if (status < 0)
	{
		_heroDir = Tank::R;
	}
	status = GetKeyState('J');//ֻ�ж��Ƿ񱻰���
	if (status < 0)
	{
		_hero->fire();
	}
#endif
	if (_heroDir != Tank::NONE)
	{
		_hero->moveTank(_heroDir, _ai, _hero);//���������ˣ����ж��Ƿ�������

		if (!_isFullScreamTouch)
			_heroDir = Tank::NONE;//һ��Ҫ��0  ��Ȼ��һֱ��ǰ�ߣ���Ϊһֱ����_heroDir != Tank::NONE
	}
	

	//����ӵ�  ���ͼ  ֮�����ײ  ���Ƿ�ɳ���ͼ��
	check(_hero->_bullets);

	//����ҷ��ӵ�  �͵з�̹��֮�����ײ  ������һ������
	checkTankBullets(_hero->_bullets);

	//���hero��prop
	checkHeroProp();

	//��ʾѪ��
	showItem();

	//���bigFire
	checkBigFire();

	//��������Ƿ���Ա���
	wifeRun();

	//��������Ƿ񱻾���
	checkWife();

	//����ƶ���Ļ��
	checkMoveScream();

	//�����Ϸ�Ƿ��Ѿ�ʤ����
	checkWin();
}

void GameLayer::checkWife()
{
	if (_wife == NULL)
	{
		_smallWife->setVisible(true);
		CCPoint pt = _hero->getPosition();
		switch (_hero->getDir())
		{
		case Tank::U:
			_smallWife->setPosition(pt + ccp(48, 0));
			break;
		case Tank::R:
			_smallWife->setPosition(pt + ccp(0, -48));
			break;
		case Tank::D:		
			_smallWife->setPosition(pt + ccp(-48, 0));
			break;
		case Tank::L:			
			_smallWife->setPosition(pt + ccp(0, 48));
			break;
		default:
			break;
		}
	}
}

void GameLayer::checkBigFire()
{
	for (int i = _bigFireCount1 - 1; i >= 0; i--)
	{
		CCSprite* bigFire = (CCSprite*)_bigFires1->objectAtIndex(i);

		//�з�tank��bigFire
		for (int j = _ai->_tanks->count() - 1; j >= 0; j--)
		{
			Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(j);
			//if (bigFire->boundingBox().intersectsRect(tank->boundingBox()))//�����Ӵ�����ʹ���������
			if (bigFire->boundingBox().containsPoint(tank->getPosition()))//����Ҫ��һ��
			{
				tank->shooted();
				_ai->_tanks->removeObjectAtIndex(j);
			}
		}	
	}
	for (int i = _bigFireCount2 - 1; i >= 0; i--)
	{
		CCSprite* bigFire1 = (CCSprite*)_bigFires2->objectAtIndex(i);
		//�з�tank��bigFire
		for (int j = _ai->_tanks->count() - 1; j >= 0; j--)
		{
			Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(j);
			if (bigFire1->boundingBox().containsPoint(tank->getPosition()) || _sL->boundingBox().containsPoint(tank->getPosition()) || _sR->boundingBox().containsPoint(tank->getPosition())) //����Ҫ��һ��
			{
				tank->shooted();
				_ai->_tanks->removeObjectAtIndex(j);
			}
		}
	}
	for (int i = _bigFireCount3 - 1; i >= 0; i--)
	{
		CCSprite* bigFire2 = (CCSprite*)_bigFires3->objectAtIndex(i);

		//�з�tank��bigFire
		for (int j = _ai->_tanks->count() - 1; j >= 0; j--)
		{
			Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(j);
			if (bigFire2->boundingBox().intersectsRect(tank->boundingBox()))//����Ҫ��һ��
			{
				tank->shooted();
				_ai->_tanks->removeObjectAtIndex(j);
			}
		}
	}
}

void GameLayer::checkHeroProp()
{
	for (int i = _props->count() - 1; i >= 0; i--)
	{
		Prop* p = (Prop*)_props->objectAtIndex(i);
		if (p->boundingBox().intersectsRect(_hero->boundingBox()))
		{
			eatProp(p->getType());
			_props->removeObjectAtIndex(i);
			p->removeFromParent();
			return;
		}
	}
}

void GameLayer::checkWin()
{
	//if (_ai->_countTank >= 1)//������
	//if (_killedTank >= 1)
	//	gameOver_win();//������
	if (_wife == NULL && _xuanwo->boundingBox().containsPoint(_hero->getPosition()) && _needKilled <= 0)
	{
		gameOver_win();
		return;
	}
	if (_xuanwo->boundingBox().containsPoint(_hero->getPosition()))
	{
		CCSprite* s = CCSprite::create(IMAGE_tishiNoGo);
		addChild(s);
		s->setPosition(ccp(winSize.width / 2, winHeight + winSize.height / 2 - 105));
		stopAllActions();
		s->runAction(CCSequence::create(CCFadeOut::create(0.5), CCRemoveSelf::create(), NULL));
	}
}

void GameLayer::checkTankBullets(CCArray* bullets)
{
	int count = bullets->count();

	for (int i = count - 1; i >= 0; i--)//�����ӵ�
	{
		Bullet* b = (Bullet*)bullets->objectAtIndex(i);

		//�����ű�����û
		if (_wife != NULL&&_wife->boundingBox().containsPoint(b->getPosition()))
			gameOver_lose();

		if (b->getType() == Tank::FRIEND)//������ҷ��ӵ�
		{
			int tanks = _ai->_tanks->count();//��ȡ�ط�tank����
			for (int j = tanks - 1; j >= 0; j--)//�����Է�̹��
			{
				Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(j);//��ȡ�з�ĳһ��̹��
				if (b->boundingBox().intersectsRect(tank->boundingBox()))
				{
					//tank Ч��
					tank->shooted();
					_ai->_tanks->removeObjectAtIndex(j);
					//�ӵ� Ч��
					b->removeFromParent();
					bullets->removeObjectAtIndex(i);
					break;
				}
			}
		}
		else//����ǵط��ӵ�
		{
			if (b->boundingBox().intersectsRect(_hero->boundingBox()))//����з��ӵ�  ������̹��
			{
				_hero->shooted();
				//�ӵ� Ч��
				b->removeFromParent();
				bullets->removeObjectAtIndex(i);
				if (_hero->getHp() <= 0)
					gameOver_lose();
			}
		}
	}
}

void GameLayer::check(CCArray* bullets)
{
	int count = bullets->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Bullet* b = (Bullet*)bullets->objectAtIndex(i);
		
		//�ӵ��ɳ�����
		//if (!Util::isContent(b->getPosition(), _map))
		//{
		//	b->removeFromParent();
		//	//_heroBullets->removeObject(bullet);  ���д�����һ���ģ�������һ��Ч��Ӧ�ø���
		//	bullets->removeObjectAtIndex(i);
		//	continue;
		//}
		//CCPoint mapPt = Util::GL2map(b->getPosition(), _map);
		//checkFireInMap(mapPt, bullets, b, i);

		float size = b->getContentSize().width / 6;
		float width = b->getPositionX();
		float height = b->getPositionY();
		/*
			�����ӵ����������жϣ��Ͳ��������������򵥵�b->getPosition()�ж���_map������
			��Ϊ�п����ӵ��ܳ�ȥһ���û����ȫ��ȥ����������û�а���removeFromParent
			������ȡ��ȡ��ͼ�����꣬������ͼ�о�û�����������꣬����Խ��
		*/
		if (!Util::isContent(ccp(width, height + size), _map) || !Util::isContent(ccp(width, height - size), _map)
			|| !Util::isContent(ccp(width - size, height), _map) || !Util::isContent(ccp(width + size, height), _map))
		{
			b->removeFromParent();
			//_heroBullets->removeObject(bullet);  ���д�����һ���ģ�������һ��Ч��Ӧ�ø���
			bullets->removeObjectAtIndex(i);
			continue;
		}
		//���ӵ��������Ҷ������ж�
		CCPoint mapPt = Util::GL2map(ccp(width, height + size), _map);
		if (!checkFireInMap(mapPt, bullets, b, i))
		{
			mapPt = Util::GL2map(ccp(width, height - size), _map);
			if (!checkFireInMap(mapPt, bullets, b, i))
			{
				mapPt = Util::GL2map(ccp(width - size, height), _map);
				if (!checkFireInMap(mapPt, bullets, b, i))
				{
					mapPt = Util::GL2map(ccp(width + size, height), _map);
					checkFireInMap(mapPt, bullets, b, i);
				}
			}
		}
	}

}
bool GameLayer::checkFireInMap(CCPoint &mapPt, CCArray* bullets, Bullet* b,int index)
{
	CCTMXLayer* layer0 = _map->layerNamed("layer_0");
	int gid = layer0->tileGIDAt(mapPt);
	//int gid = Util::getMapGID(_map, "layer_0", bullet->getPosition());
	TILE_TYPE type = tileGidType[gid];

	//�ӵ���ǽ
	if (type == tileWall)
	{
		b->removeFromParent();
		bullets->removeObjectAtIndex(index);
		layer0->setTileGID(0, mapPt);
		return true;
	}
	//�ӵ����ת
	if (type == tileSteel)
	{
		b->removeFromParent();
		bullets->removeObjectAtIndex(index);

		if (_isStrongFire && b->getType() == Tank::FRIEND && gid != 11)
			layer0->setTileGID(0, mapPt);
		return true;
	}
	return false;
}

void GameLayer::gameOver_lose()
{
	unscheduleUpdate();//��仰һ���üӡ�����Ȼ��������ᱻִ�����Σ��ڼ�����Ϸʱ
	_ai->unscheduleUpdate();//��������ˣ��з�����ӵ��� ��ʱ��Ҳ����ر�

	addChild(QuitDialog::create(false,true,false));
}

void GameLayer::gameOver_win()
{
	unscheduleUpdate();
	_ai->unscheduleUpdate();

	//�����жϵ�ǰ�ؿ��Ƿ�����Ѿ�ͨ�������ؿ�.....�ܲ����ڣ��浽��10�أ�Ȼ����һ�µ�һ�أ��ͱ����1��
	if (MenuLayer::_mapIndex + 1 == getLevel())//+ 1 ��Ϊ _mapIndex ��0��ʼ��
	{
		setLevel(getLevel() + 1);
	}

	addChild(QuitDialog::create(false,true,true,this));
}

void GameLayer::win(CCObject*)
{
	//˳����ȥ��һ��
	MenuLayer::_mapIndex++;//��־ҲҪ���ż�1
	//changeScene(GameLayer,getLevel());//ע�����ﲻ����getLevel������������ͨ��10�أ����һ�ص�ʱ�����ǵڶ������11�أ���������ǰ�����õ���_mapIndex�ǵ���getLevel����һ�أ�����Ҫ��11����10��
	changeScene(GameLayer, MenuLayer::_mapIndex + 1,_isFullScreamTouch);

//	_hero->_bullets->release();
}
void GameLayer::lose(CCObject*)
{
	changeScene(MenuLayer);
	Music::StartLayerMusic();

//	_hero->_bullets->release();
}


void GameLayer::initAI()
{
	_ai = AI::create();
	_map->addChild(_ai);//ֻ��Ϊ����AI����û�ȡ��ͼ����
}

void GameLayer::initReturn()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_back_normal_white, IMAGE_back_press_white);
	menu->addChild(item);
	item->setTarget(this, menu_selector(GameLayer::rCallback));
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 2, winSize.height / 2 - item->getContentSize().height / 2));

	_returnItem = item;
}

void GameLayer::rCallback(CCObject*)
{
	addChild(QuitDialog::create(false));
}

//���ӵ���
void GameLayer::addProp()
{
	//r = Prop::SUM_TYPE * 10 * CCRANDOM_0_1();//����������ߣ�cocos�ײ�Ҳ���õ�rand()
	int r = rand() % 6;//�����������

	//kill����  �� 25��֮1�Ļ���  �����Ķ���5��֮1 ����
	if ((Prop::TYPE)r == Prop::KILL)
	{
		r = rand() % 6;
	}
	//CCLog("-------%d---------", r);

	Prop* prop = Prop::create((Prop::TYPE)r);
	addChild(prop);

	_props->addObject(prop);

	float x = winSize.width - prop->getContentSize().width;
	x *= CCRANDOM_0_1();
	//float x = h - prop->getContentSize().width;
	float y = winSize.height - prop->getContentSize().height;
	y *= CCRANDOM_0_1();
	prop->setPosition(ccp(x + prop->getContentSize().width / 2, y + prop->getContentSize().height / 2 + winHeight - winSize.height / 2));
}

void GameLayer::eatProp(Prop::TYPE t)
{
	//enum TYPE{ LIFE, KILL, BOSS, FIRE, STOP, SUM_TYPE };
	switch (t)
	{
	case Prop::LIFE:
		eatLife();
		break;
	case Prop::KILL:
		eatKill();
		break;
	case Prop::BOSS:
		eatBoss();
		break;
	case Prop::FIRE:
		eatFire();
		break;
	case Prop::STOP:
		eatStop();
		break;
	case Prop::STRONG:
		eatStrong();
		break;
	}
}

void GameLayer::eatStrong()
{
	_isStrongFire = true;

	scheduleOnce(schedule_selector(GameLayer::StrongFire), 10);
}
void GameLayer::eatLife()
{
	_hero->setHp(_hero->getHp() + 1);

	Music::eat1();
}
void GameLayer::eatKill()
{
	int count = _ai->_tanks->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(i);
		tank->shooted();
		_ai->_tanks->removeObjectAtIndex(i);
	}
	Music::eat2();
}
void GameLayer::eatBoss()
{
	//����שλ��
	CCPoint pts[] = {
		ccp(0, 51), ccp(1, 51), ccp(2, 51), ccp(3, 51),
		ccp(0, 52), ccp(3, 52),
		ccp(0, 53), ccp(3, 53),
		ccp(0, 54), ccp(1, 54), ccp(2, 54), ccp(3, 54),
	};
	//��ȡ��
	CCTMXLayer* layer0 = _map->layerNamed("layer_0");
	//���ò������GID  ���ϼұ����ͨש
	for (int i = 0; i < 12; i++)
	{
		layer0->setTileGID(14, pts[i]);
	}

	//���ö�ʱ����15����Զ���ԭ��ͨש
	scheduleOnce(schedule_selector(GameLayer::recoveryBricks),15);

	Music::eat2();
}

void GameLayer::recoveryBricks(float)
{
	CCRect rt(0, 48 * 5, 48 * 4, 48 * 4);
	if (rt.containsPoint(_hero->getPosition()))
		return;

	//����שλ��
	CCPoint pts[] = {
		ccp(0, 51), ccp(1, 51), ccp(2, 51), ccp(3, 51),
		ccp(0, 52), ccp(3, 52),
		ccp(0, 53), ccp(3, 53),
		ccp(0, 54), ccp(1, 54), ccp(2, 54), ccp(3, 54),
	};
	//��ȡ��
	CCTMXLayer* layer0 = _map->layerNamed("layer_0");
	//���ò������GID  ���ϼұ��ש
	for (int i = 0; i < 12; i++)
	{
		layer0->setTileGID(11, pts[i]);
	}

	Music::eat1();
}

void GameLayer::eatFire()
{
	//����2 4��֮1�ļ��ʻ��  ����3 20��֮1�ļ��ʻ��
	CCSprite* s;
	int r = rand() % 20;
	if (r == 2)
	{
		//��������3
		s = CCSprite::create(IMAGE_bigFire3);
		_bigFires3->addObject(s);
		/*�Ӹ��жϾ���Ϊ�˷�ֹ���޸����޸�*/
		if (getFire3() == _canBigFire3)
			setFire3(++_canBigFire3);
	}
	else if (r % 4 == 0)
	{
		//��������2
		s = CCSprite::create(IMAGE_bigFire2);
		_bigFires2->addObject(s);
		if (getFire2() == _canBigFire2)
			setFire2(++_canBigFire2);
	}
	else
	{
		//��������1
		s = CCSprite::create(IMAGE_bigFire);
		_bigFires1->addObject(s);
		if (getFire1() == _canBigFire1)
			setFire1(++_canBigFire1);
	}

	Music::eat2();
}
void GameLayer::eatStop()
{
	int count = _ai->_tanks->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(i);
		tank->setDir(Tank::NONE);
	}

	_ai->unschedule(schedule_selector(AI::changeDir));
	scheduleOnce(schedule_selector(GameLayer::continueMove),5);

	Music::eat1();
}

void GameLayer::StrongFire(float)
{
	_isStrongFire = false;
}

void GameLayer::initshowItem()
{
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
	const CCString* str = dict->valueForKey("blood");
	_blood = CCLabelTTF::create(Util::format(_hero->getHp(), str->getCString()), "Arial", 40);
	addChild(_blood);

	str = dict->valueForKey("bigFire");
	_bigFireItem = CCLabelTTF::create(Util::format(_canBigFire1, str->getCString()), "Arial", 40);
	addChild(_bigFireItem);

	str = dict->valueForKey("bigFire1");
	_bigFireItem1 = CCLabelTTF::create(Util::format(0, str->getCString()), "Arial", 40);
	addChild(_bigFireItem1);

	str = dict->valueForKey("bigFire2");
	_bigFireItem2 = CCLabelTTF::create(Util::format(0, str->getCString()), "Arial", 40);
	addChild(_bigFireItem2);

	str = dict->valueForKey("killedTank");
	_killedTankItem = CCLabelTTF::create(Util::format(0, str->getCString()), "Arial", 40);
	addChild(_killedTankItem);

	_blood->setAnchorPoint(ccp(0, 1));
	_blood->setPosition(ccp(10, winSize.height));
	_blood->enableStroke(ccc3(0, 0, 255), 5);

	_killedTankItem->setAnchorPoint(ccp(0, 1));
	_killedTankItem->setPosition(ccp(10, winSize.height - 40));
	_killedTankItem->enableStroke(ccc3(0, 0, 255), 5);

	_bigFireItem->setAnchorPoint(ccp(0, 1));
	_bigFireItem->setPosition(ccp(10, winSize.height - 80));
	_bigFireItem->enableStroke(ccc3(0, 0, 255), 5);

	_bigFireItem1->setAnchorPoint(ccp(0, 1));
	_bigFireItem1->setPosition(ccp(10, winSize.height - 120));
	_bigFireItem1->enableStroke(ccc3(0, 0, 255), 5);

	_bigFireItem2->setAnchorPoint(ccp(0, 1));
	_bigFireItem2->setPosition(ccp(10, winSize.height - 160));
	_bigFireItem2->enableStroke(ccc3(0, 0, 255), 5);
}

void GameLayer::showItem()
{
	//���ﲻ֪��Ϊʲô���������¼���xml�ļ�������Ȼ���ĸ����治��������ÿ�ζ������¼��ء����о�ͦ�˷ѵġ��������ҿ��Բ���TTF��������Ū��
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
	const CCString* str = dict->valueForKey("blood");
	_blood->setString(Util::format(_hero->getHp(), str->getCString()));
	
	str = dict->valueForKey("bigFire");
	_bigFireItem->setString(Util::format(_canBigFire1, str->getCString()));

	str = dict->valueForKey("bigFire1");
	_bigFireItem1->setString(Util::format(_canBigFire2, str->getCString()));

	str = dict->valueForKey("bigFire2");
	_bigFireItem2->setString(Util::format(_canBigFire3, str->getCString()));

	str = dict->valueForKey("killedTank");
	if (_needKilled < 0)
		_needKilled = 0;
	_killedTankItem->setString(Util::format(_needKilled, str->getCString()));
}

void GameLayer::continueMove(float)
{
	int count = _ai->_tanks->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Tank::DIR dir = (Tank::DIR)(int)(Tank::NONE * CCRANDOM_0_1());//�޷���floatת��ΪTank::dir   ���Լ���һ��int
		Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(i);
		tank->setDir(dir);
	}
	_ai->schedule(schedule_selector(AI::changeDir),2);
}

void GameLayer::shootBigFire1()
{
	/*Ϊ�˷�ֹ�޸����޸�_canBigFire��ֵ�����ԴӴ��̶�ȡ���ݼ���*/
	if (getFire1() != _canBigFire1)
	{
		setFire1(0);
		setFire2(0);
		setFire3(0);
		exit(0);
	}

	//_bigFireCount1�����ϵĴ���  ����
	CCSprite* s = (CCSprite*)_bigFires1->objectAtIndex(_bigFireCount1++);//��_bigFireCount1�����ԭ����Ϊ�˽��������ͬʱ����������
	addChild(s);
	s->setPosition(_hero->getPosition());

	//�����԰��Ĵ�����  ����һ��
	setFire1(--_canBigFire1);

	//�ô��ж�����
	Tank::DIR dir = _hero->getDir();
	CCMoveBy* move;
	switch (dir)
	{
	case Tank::U:
		move = CCMoveBy::create(8, ccp(0, winSize.width));
		break;
	case Tank::R:
		move = CCMoveBy::create(8, ccp(winSize.width, 0));
		break;
	case Tank::D:
		move = CCMoveBy::create(8, ccp(0, -winSize.width));
		break;
	case Tank::L:
		move = CCMoveBy::create(8, ccp(-winSize.width, 0));
		break;
	default:
		break;
	}

	//�ô�����ת
	CCRotateBy* rotate = CCRotateBy::create(8, 360);

	//�ô�����˸
	CCBlink* blink = CCBlink::create(6, 5);//����û�м���˸

	//��һ�����顣ͬʱ���������
	CCSpawn* action = CCSpawn::create(move, rotate,NULL);

	//��ΪҪ����ײ��⣬����������Ӵ��е�������ɾ����Ҳ���ܴӼ����е�ʱ��ɾ������Ϊ����ֻ����ֹͣ��ʱ���ɾ��
	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(GameLayer::removeBigFire1));

	//ִ���궯�����Զ�ɾ���Լ�
	s->runAction(CCSequence::create(action, callFunc, CCRemoveSelf::create(), NULL));
}

void GameLayer::shootBigFire2()
{
	/*Ϊ�˷�ֹ�޸����޸�_canBigFire��ֵ�����ԴӴ��̶�ȡ���ݼ���*/
	if (getFire2() != _canBigFire2)
	{
		setFire1(0);
		setFire2(0);
		setFire3(0);
		exit(0);
	}


	CCSprite* s = (CCSprite*)_bigFires2->objectAtIndex(_bigFireCount2++);
	addChild(s);
	s->setScale(2);

	setFire2(--_canBigFire2);

	s->setPosition(ccp(_hero->getPositionX(), winHeight + 30));
	_sL->setPosition(ccp(_hero->getPositionX() - 400, winHeight + 30));
	_sR->setPosition(ccp(_hero->getPositionX() + 400, winHeight + 30));
	_sL->setVisible(true);
	_sR->setVisible(true);


	CCArray* arr = CCArray::create();
	CCTexture2D* text;
	CCSpriteFrame* frame;
	for (int i = 0; i <= 5; i++)
	{
		text = CCTextureCache::sharedTextureCache()->addImage(Util::format(i, "tank/bullet/", ".png"));
		frame = CCSpriteFrame::createWithTexture(text, CCRect(0, 0, 150, 600));
		arr->addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, 0.1f);
	CCAnimate* animate1 = CCAnimate::create(animation);
	CCAnimate* animate2 = CCAnimate::create(animation);
	CCAnimate* animate3 = CCAnimate::create(animation);

	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(GameLayer::removeBigFire2));
	CCCallFunc* callFunc2 = CCCallFunc::create(this, callfunc_selector(GameLayer::setVisiable));

	s->runAction(CCSequence::create(animate1, callFunc, CCRemoveSelf::create(), NULL));
	_sL->runAction(CCSequence::create(animate2, callFunc2,NULL));
	_sR->runAction(CCSequence::create(animate3, callFunc2, NULL));
}
void GameLayer::shootBigFire3()
{
	if (getFire3() != _canBigFire3)
	{
		setFire1(0);
		setFire2(0);
		setFire3(0);
		exit(0);
	}


	//_bigFireCount1�����ϵĴ���  ����
	CCSprite* s = (CCSprite*)_bigFires3->objectAtIndex(_bigFireCount3++);//��_bigFireCount1�����ԭ����Ϊ�˽��������ͬʱ����������
	addChild(s);
	s->setScale(3);

	//�����԰��Ĵ�����  ����һ��
	setFire3(--_canBigFire3);

	s->setPosition(ccp(winSize.width / 2, winHeight + 20));

	CCArray* arr = CCArray::create();
	CCTexture2D* text;
	CCSpriteFrame* frame;
	for (int i = 1; i <= 11; i++)
	{
		text = CCTextureCache::sharedTextureCache()->addImage(Util::format(i, "tank/bullet/1006-", ".png"));
		frame = CCSpriteFrame::createWithTexture(text, CCRect(0, 0, 540, 450));
		arr->addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, 0.15f);
	CCAnimate* animate = CCAnimate::create(animation);

	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(GameLayer::removeBigFire3));

	s->runAction(CCSequence::create(animate, callFunc, CCRemoveSelf::create(), NULL));
}
void GameLayer::removeBigFire1()
{
	_bigFires1->removeObjectAtIndex(0);//ÿ������Ķ������ȼ��������  ����
	_bigFireCount1--;
}

void GameLayer::setVisiable()
{
	_sL->setVisible(false);
	_sR->setVisible(false);
}

void GameLayer::removeBigFire2()
{
	_bigFires2->removeObjectAtIndex(0);//ÿ������Ķ������ȼ��������  ����
	_bigFireCount2--;
}

void GameLayer::removeBigFire3()
{
	_bigFires3->removeObjectAtIndex(0);//ÿ������Ķ������ȼ��������  ����
	_bigFireCount3--;
}

void GameLayer::initWife()
{
	_wife = CCSprite::create(IMAGE_wife);
	addChild(_wife);
	_wife->setPosition(ccp(48*2, 48*7));
	_wife->setTag(1314);

	_smallWife = CCSprite::create(IMAGE_wife);
	addChild(_smallWife);
	_smallWife->setScale(0.4);
	_smallWife->setVisible(false);
}

void GameLayer::wifeRun()
{
	if (_wife!=NULL && _wife->boundingBox().intersectsRect(_hero->boundingBox()))
	{
		CCJumpTo* jump = CCJumpTo::create(0.3f, _hero->getPosition(), 30, 1);
		_wife->runAction(CCSequence::create(jump, CCRemoveSelf::create(), NULL));
		_wife = NULL;
		//_b2->setVisible(true);

		Music::eat2();
	}
}

void GameLayer::checkMoveScream()
{
	//���winSize.height�ǲ����ġ�����������Ҫ��������������ƶ�  �������ƶ�
	
	int curHeight = _hero->getPositionY();
	//static float winHeight = winSize.height / 2;//��ǰ���ڵ�һ��   �������������������Ƿ���Ҫ�ƶ�

	//����������жϿ�֪�����ϣ����£��ƶ������Ժϲ���һ������������һ����
	//���Ӿ��ϣ����������ƶ�
	//if (winHeight + winSize.height / 2  <= _map->getContentSize().height && curHeight > winHeight)//���Կ�������ߵط������ܳ������TiledMap
	//{
	//	setPosition(getPosition() - ccp(0, curHeight - winHeight));

	//	winHeight = curHeight;
	//}
	//���Ӿ��ϣ����������ƶ�
	//else if (winHeight > winSize.height / 2 && curHeight < winHeight)
	//{
	//	setPosition(getPosition() - ccp(0, curHeight - winHeight));
	//	winHeight = curHeight;
	//}
	if (winHeight + winSize.height / 2 <= _map->getContentSize().height && curHeight > winHeight || winHeight > winSize.height / 2 && curHeight < winHeight)//���Կ�������ߵط������ܳ������TiledMap
	{
		setPosition(getPosition() - ccp(0, curHeight - winHeight));

		//�ƶ���ť  ��������
		if (!_isFullScreamTouch)
			for (int i = 0; i < 4; i++)
			{
				CCMenuItem* item = (CCMenuItem*)_menu->getChildByTag(1000 + i);
				item->setPosition(item->getPosition() + ccp(0, curHeight - winHeight));
			}

		//�ӵ���ť
		_b1->setPosition(_b1->getPosition() + ccp(0, curHeight - winHeight));
		_b2->setPosition(_b2->getPosition() + ccp(0, curHeight - winHeight));
		_b3->setPosition(_b3->getPosition() + ccp(0, curHeight - winHeight));
		_fireitem->setPosition(_fireitem->getPosition() + ccp(0, curHeight - winHeight));

		//����CCLabelTTF
		_blood->setPosition(_blood->getPosition() + ccp(0, curHeight - winHeight));
		_bigFireItem->setPosition(_bigFireItem->getPosition() + ccp(0, curHeight - winHeight));
		_bigFireItem1->setPosition(_bigFireItem1->getPosition() + ccp(0, curHeight - winHeight));
		_bigFireItem2->setPosition(_bigFireItem2->getPosition() + ccp(0, curHeight - winHeight));
		_killedTankItem->setPosition(_killedTankItem->getPosition() + ccp(0, curHeight - winHeight));

		//���ذ�ť
		_returnItem->setPosition(_returnItem->getPosition() + ccp(0, curHeight - winHeight));

		winHeight = curHeight;
	}
}


void GameLayer::fullScreamTouch()
{
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}
void GameLayer::nofullScreamTouch()
{
	//����
	const char* panelFiles[] = {
		IMAGE_UP_1,
		IMAGE_UP_2,
		IMAGE_RIGHT_1,
		IMAGE_RIGHT_2,
		IMAGE_DOWN_1,
		IMAGE_DOWN_2,
		IMAGE_LEFT_1,
		IMAGE_LEFT_2
	};
	//CCMenu* menu = CCMenu::create();  //����Ͳ���
	Panel* menu = Panel::create();		//Ҫ������Ĺ��� CCMenu
	addChild(menu);
	_menu = menu;

	//���뷽��ť
	for (size_t i = 0; i < sizeof(panelFiles) / sizeof(*panelFiles); i += 2)
	{
		CCMenuItemImage* item = CCMenuItemImage::create(panelFiles[i], panelFiles[i + 1]);
		item->setTag(1000 + i / 2);
		item->setTarget(this, menu_selector(GameLayer::DirCallback));
		item->setScale(2.5);
		menu->addChild(item);
	}

	//�������ĸ���ť��λ��
	CCPoint center = ccp(-700, -285);
	float off = 145;//�����е�

	CCMenuItem* item;

	item = (CCMenuItem*)menu->getChildByTag(1000);
	item->setPosition(center + ccp(0, off));

	item = (CCMenuItem*)menu->getChildByTag(1000 + 1);
	item->setPosition(center + ccp(off, 0));

	item = (CCMenuItem*)menu->getChildByTag(1000 + 2);
	item->setPosition(center + ccp(0, -off));

	item = (CCMenuItem*)menu->getChildByTag(1000 + 3);
	item->setPosition(center + ccp(-off, 0));
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint move = pTouch->getDelta();
	float x = move.x;
	float y = move.y;

	float xx = x;
	float yy = y;

	if (xx < 0)
		xx = -xx;
	if (yy < 0)
		yy = -yy;

	if (xx < 3 && yy < 3)
		return;

	if (xx > yy)//����������Ϊ��X���ƶ�������Ϊ����ֻ��Ҫ��������
	{
		if (x > 0)
			_heroDir = Tank::R;
		else
			_heroDir = Tank::L;

	}
	else//����������Ϊ��Y���ƶ�
	{
		if (y > 0)
			_heroDir = Tank::U;
		else
			_heroDir = Tank::D;
	}
}

void  GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	//����ʱ��λ��
	CCPoint start = pTouch->getStartLocation();
	//�ɿ�ʱ��λ��
	CCPoint end = pTouch->getLocation();//��Ϊ�������ľ���  ����뿪ʱ��touch

	// ������ľ���С��5����ô������Ϊ�û��ڵ��
	if (start.getDistanceSq(end) <= 25)
		_heroDir = Tank::NONE;
}