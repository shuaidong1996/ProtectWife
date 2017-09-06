#include "Tank.h"
#include "Bullet.h"
#include "Music.h"
#include "GameLayer.h"
#include "MenuLayer.h"

Tank* Tank::create(TYPE t, int damage, int hp, STATUS s, DIR d)
{
	Tank* tank = new Tank;
	tank->setType(t);
	tank->setDamage(damage);
	tank->setHp(hp);
	tank->setStatus(s);
	tank->setDir(d);
	tank->setSpeed(250);//��ʼ���ٶ�Ϊ280

	if (tank->getType() == FRIEND)
		tank->initWithFile(IMAGE_playerU);
	else
	{
		//int rand = CCRANDOM_0_1() * 4 + 1;//"tank/enemy/enemy1D.png"
		static int rand = 0;
		tank->initWithFile(Util::format((rand++) % 6 + 1, "tank/enemy/enemy", "D.png"));

		/*���ò�ͬ���Ѷ�*/
		int level = MenuLayer::_mapIndex / 5;
		if (level == 0)
			tank->schedule(schedule_selector(Tank::fire), 1.8);
		else if (level == 1)
			tank->schedule(schedule_selector(Tank::fire), 1);
		else if (level == 2)
			tank->schedule(schedule_selector(Tank::fire), 0.5);
		else if (level == 3)
			tank->schedule(schedule_selector(Tank::fire), 0.2);

	}

	tank->autorelease();
	return tank;
}
void Tank::setDir(DIR dir)
{
	_dir = dir;
	setRotation(dir * 90);//Rotation ��ת
}
Tank::DIR Tank::getDir()
{
	return _dir;
}

void Tank::moveTank(DIR dir, AI* ai, Tank* hero)
{
	if (dir == NONE && getType() == Tank::ENEMY)//������һ�䣬����Ϊ����Ҫ�� ֹͣ�з�̹�� ��ʱ����õ�
		return;

	setDir(dir);//�ı�tankͷ�ķ���

	if (canMove(dir,ai,hero))
	{
		float off = 1.0 / 60 * getSpeed();
		switch (dir)
		{
		case U:
			setPositionY(getPositionY() + off);
			break;
		case R:
			setPositionX(getPositionX() + off);
			break;
		case D:
			setPositionY(getPositionY() - off);
			break;
		case L:
			setPositionX(getPositionX() - off);
			break;
		}
	}
	else if (getType() == ENEMY)//���ȣ�û���ƶ��������ǵз�tank  ��ִ���������
	{
		// Ҫ�Զ���ͷ�������һ�����򣬵�ͷ
		
		//while(dir = CCRANDOM_0_1() * 4 == 4);//��ȡ 0 1 2 3 �⼸���������
		int dir = rand() % 4;
		setDir((DIR)dir);//��ͷ
	}
}

bool Tank::canMove(DIR dir, AI* ai, Tank* hero)
{
	//��ȡtank��boundingbox
	CCRect rt = boundingBox();

	float off = 1.0 / 60 * getSpeed();//��ȡÿһ֡�ƶ��ľ���

	CCPoint pt[3];
	switch (dir)
	{
	case U:
		rt.origin.y += off;//�Ѿ��������ƶ�
		//��������ϸ���
		pt[0] = ccp(rt.getMinX(), rt.getMaxY());
		pt[1] = ccp(rt.getMidX(), rt.getMaxY());
		pt[2] = ccp(rt.getMaxX(), rt.getMaxY());
		break;
	case R:
		rt.origin.x += off;
		pt[0] = ccp(rt.getMaxX(), rt.getMinY());
		pt[1] = ccp(rt.getMaxX(), rt.getMidY());
		pt[2] = ccp(rt.getMaxX(), rt.getMaxY());
		break;
	case D:
		rt.origin.y -= off;
		pt[0] = ccp(rt.getMinX(), rt.getMinY());
		pt[1] = ccp(rt.getMidX(), rt.getMinY());
		pt[2] = ccp(rt.getMaxX(), rt.getMinY());
		break;
	case L:
		rt.origin.x -= off;
		pt[0] = ccp(rt.getMinX(), rt.getMinY());
		pt[1] = ccp(rt.getMinX(), rt.getMidY());
		pt[2] = ccp(rt.getMinX(), rt.getMaxY());
		break;
	default:
		return false;
	}

	//��ȡ��ͼ
	CCTMXTiledMap* map = getMap();
	
	//��ȡ��
	CCTMXLayer* layer0 = map->layerNamed("layer_0");

	for (int i = 0; i < 3; i++)
	{
		//�ж��Ƿ��ڵ�ͼ��
		if (!Util::isContent(pt[i], map))
			return false;

		//�ж��Ƿ�ײǽ
		CCPoint mapPt = Util::GL2map(pt[i], map);
		int gid = layer0->tileGIDAt(mapPt);//��ȡgid
		TILE_TYPE type = tileGidType[gid];
		//if (type == tileKing || type == tileRiver || type == tileSteel || type == tileWall)
		if (type != tileNone && type != tileGrass)
			return false;

		//���ҷ�tank�Ƿ�ͶԷ�tank��ײ
		if (getType() == FRIEND)//�ҷ��͵�����ײ���ҷ�������
		{
			for (int j = ai->_tanks->count() - 1; j >= 0; j--)//�����з�Tank
			{
				Tank* tank = (Tank*)ai->_tanks->objectAtIndex(j);
				if (tank->boundingBox().containsPoint(pt[i]))
					return false;
			}
		}
		else//�з����ҷ���ײ���з�������
		{
			if (hero->boundingBox().containsPoint(pt[i]))
				return false;
		}
	}
	return true;
}

void Tank::fire(float)
{
	Bullet* b = Bullet::create(getDamage(), getStatus(), getType());

	//���û����仰���ӵ�Ĭ�ϳ����ڣ�0,0��������û�н�ֹ̹�˶���ʱ�򲻻�����������ͷ����ȥ�ˣ������������ֹ��ǡ���ַ������ӵ����ӵ��ͻ�����ڣ�0��0��������������ڣ�0,0���ͻ���
	b->setPosition(ccp(-20, -20));

	_bullets->addObject(b);//���뵽������

	//���ӵ����뵽map��,,��map����Ⱦ�ӵ���Ҫ��ʾ����������뵽��Ⱦ���У�
	getMap()->addChild(b);

	//���ӵ���
	CCPoint delta;
	switch (_dir)
	{
	case Tank::U:
		delta = ccp(0, winSize.height * 3);
		//����λ��
		b->setPosition(getPosition() + ccp(0,getContentSize().height/2));
		break;
	case Tank::R:
		delta = ccp(winSize.height * 3, 0);
		b->setPosition(getPosition() + ccp(getContentSize().height / 2,0));
		break;
	case Tank::D:
		delta = ccp(0, -winSize.height * 3);
		b->setPosition(getPosition() + ccp(0, -getContentSize().height / 2));
		break;
	case Tank::L:
		delta = ccp(-winSize.height * 3, 0);
		b->setPosition(getPosition() + ccp(-getContentSize().height / 2,0));
		break;
	}

	b->runAction(CCMoveBy::create(3.8, delta));
}

//�����е�Ч��
void Tank::shooted()
{
	if (getType() == ENEMY)
	{
		GameLayer* game = (GameLayer*)getParent()->getParent();
		game->_killedTank++;
		game->_needKilled--;
	}

	//��Ѫ
	setHp(getHp() - 1);

	if (getHp() <= 0)//�����е�̹���ڱ�ըʱ���󣬻��п��ܷ����ӵ�
	{
		unschedule(schedule_selector(Tank::fire));
	}

	//��ըЧ��  // ��һ����������ʾ̹�˹ҵ�
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile(PLIST_blast);
	CCArray* arr = CCArray::create();
	for (int i = 0; i < 8; i++)
	{
		CCSpriteFrame* frame = cache->spriteFrameByName(Util::format(i + 1, IMAGE_blast, ".gif"));
		arr->addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, .1f);
	CCAnimate* animate = CCAnimate::create(animation);


	//�з�Ӣ�ۺ��ҷ�Ӣ�۱��򵽣����벻һ��
	if (getType() == FRIEND)//������ҷ�Ӣ�۱���
	{
		CCSprite* s = CCSprite::create();
		addChild(s);
		s->setPosition(ccp(getContentSize().width / 2, getContentSize().height / 2));
		s->runAction(CCSequence::create(animate, CCRemoveSelf::create(), NULL));//ֻ������
	}
	else
	{
		//�з�Ӣ�۱���������Ҫ�������һ������

		//����һ���Զ��嶯������
		CCCallFunc* callFunc = CCCallFunc::create(getParent()->getParent(), callfunc_selector(GameLayer::addProp));

		runAction(CCSequence::create(animate, callFunc, CCRemoveSelf::create(), NULL));


		//����������������������������ڣ��������������Ժ���п��ܲ������ߣ�������������ڵ�һ֡��ըʱ�ͻ��е��߲���
		//runAction(CCSequence::create(animate, CCRemoveSelf::create(), NULL));
		//getParent()->getParent();
		//��Ϊcocos��ÿһ֡��Ҫִ����ȫ�����룬��ը����һ֡�Ż������������Ҳ������һ֡���������Ǳ�ը��8��ͼ�������8֡
	}
}
