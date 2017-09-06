#include "AI.h"
#include "Tank.h"
#include "GameLayer.h"

void AI::onEnter()
{
	CCLayer::onEnter();

	_tanks = CCArray::create();
	_tanks->retain();

	_bullets = CCArray::create();
	_bullets->retain();

	scheduleUpdate();
	schedule(schedule_selector(AI::addTanks), 3.5);

	//�������ڵ�̹��̫���ˣ�ʵ���ܲ����ˣ��������һ��תͷ�Ķ�ʱ�����Ͳ�����ײǽ��תͷ
	schedule(schedule_selector(AI::changeDir), 2);

	_countTank = 0;
}
void AI::onExit()
{
	CCLayer::onEnter();

	_tanks->release();
	_bullets->release();
}
//�����ƶ�̹��  ����ӵ�
void AI::update(float)
{
	int count = _tanks->count();
	for (int i = count - 1; i >= 0; --i)//�Ǹ���̹��������������
	{
		//���ÿ��̹��
		Tank* tank = (Tank*)_tanks->objectAtIndex(i);
		tank->moveTank(tank->getDir(),this,(Tank*)getParent()->getChildByTag(666));
		/*
		���������������ע�͵ģ����������֣���������������һ��̹������������������ӵ������Ǹ������ܽ�����ײ���
		����Ҫ��  �з��ӵ�ȫ���ŵ�һ���������棬���仰˵���ǣ�ȫ����AI�ĵ��С�
		���Ǻ������ַ��֣�ÿһ���ӵ������ܽ�����ײ��⡣������ϸ���֣�ÿһ��AI��_bullets�ĵ�ַ����䣬
		�������ˣ�AI�͵���һ�Σ�Ϊʲô���ĵ�ַ����أ�
		���ֻ��һ����AI��_bulletsָ���������һ��Tank�������Tank   �ʼ�Ҽ��� onEnter(�������ҽ����˵��г�ʼ��)
		������Ҳ���������̹�˾͵���onEnterҲ���������_bullets��Tank���汻��һ��ֵ���ٵ�AI���ֱ���һ��ֵ��Ҳ�������

		���֤����������ǣ�onEnter�����ڴ������ʱ����õģ��������ĵ�����ҲûŪ���ף��������ѳ���һ�仰��

		onEnter������CCLayer�ķ�����ÿ�ε�layer��ʾ����Ļ��ʱ�򶼻���á��෴onExitÿ��layer�뿪��ʱ�򶼻���á�

		�������ȥ���ˣ�Tank�����_bullets��ʼ��������û������
		*/
		//���ÿ��̹�˵��ӵ�
		//GameLayer* gameLayer = (GameLayer*)getMap()->getParent();//��ȡGameLayer����㣬��Ҫ�������ļ���ӵ�����
		//gameLayer->check(tank->_bullets, getMap());
		//gameLayer->checkTankBullets(tank->_bullets);
	}
	//����Ҫ����ÿһ��Tank���ӵ������ӵ�һ�������õ�
	GameLayer* gameLayer = (GameLayer*)getMap()->getParent();//��ȡGameLayer����㣬��Ҫ�������ļ���ӵ�����
	gameLayer->check(_bullets);
	gameLayer->checkTankBullets(_bullets);
}
//��������tank
void AI::addTanks(float)
{
	//������̹�˲��ܶ��� 200
	if (_countTank++ >= 200)
		unschedule(schedule_selector(AI::addTanks));

	Tank* tank = Tank::create(Tank::ENEMY, 1, 1, Tank::NORMAL, Tank::D);

	tank->_bullets = _bullets;

	_tanks->addObject(tank);

	getMap()->addChild(tank);//��仰��Ҫ������ΪTank��Ҫ�õ��Լ��ĸ���map����û��仰������̹�˾�û�и��࣬û��map
	
	static bool bLeft = true;
	if (bLeft)
		tank->setPosition(ccp(3 * 48, (19 + _countTank % 36) * 48));
	else
		tank->setPosition(ccp(37 * 48, (19 + _countTank % 36) * 48));
	bLeft = !bLeft;
}

void AI::changeDir(float)
{
	int count = this->_tanks->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Tank* tank = (Tank*)_tanks->objectAtIndex(i);
		int dir = rand() % Tank::NONE;
		tank->setDir((Tank::DIR)dir);
	}
}
