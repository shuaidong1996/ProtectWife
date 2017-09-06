#include "MenuLayer.h"
#include "GameLayer.h"
#include "StartLayer.h"
#include "BigFire.h"
#include "PayLayer.h"

int MenuLayer::_mapIndex = 0;//��һ��������Ĭ��ֵ

bool MenuLayer::init()
{
	CCLayer::init();

	//��������ٴ�����Ϊ0����������10�أ��˳���ѡ��һ�ؽ�ȥ_mapIndex�ǲ��ᱻ�ı��
	_mapIndex = 0;

	//����
	Util::addBackageground(IMAGE_levelBackground,this);

	//���������ľ���
	initJumpSprite();

	//����һ������
	initLightning();

	//����һ�����ذ�ť
	initReturn();

	//����ѡ��ؿ�Item
	initSwitchLevel();

	//��鰴ť
	initCheck();

	//������ʾ
	initcue();

	//Ӧ���̵�
	initStore();

	return true;
}

void MenuLayer::initLightning()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	menu->addChild(BigFire::create(0, false));
	menu->addChild(BigFire::create(1, false));
}

void MenuLayer::initcue()
{
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
	const CCString* str1 = dict->valueForKey("cue1");
	const CCString* str2 = dict->valueForKey("cue2");
	CCLabelTTF* ttf = CCLabelTTF::create(Util::format(getLevel()-1, str1->getCString(), str2->getCString()), "Arial", 60);
	addChild(ttf);
	ttf->setAnchorPoint(ccp(1, 1));
	ttf->setColor(ccc3(255, 0, 0));
	ttf->setPosition(ccp(winSize.width, winSize.height));
}
//Item�Ļص�����  
void MenuLayer::Toggle(CCObject* sender)
{
	CCMenuItemToggle* itemToggle = (CCMenuItemToggle*)sender;
	_mapIndex = itemToggle->getSelectedIndex();//��ȡѡ��  �ڼ����ؿ�  ��0��ʼ��
}
void MenuLayer::Go(CCObject*)
{
	//�ȼ���Ƿ���Դ����������
	if (getLevel() >= _mapIndex + 1)//�����Level������ڵ���_mapIndex + 1   ��Ϊ ��0��ʼ��
	{
		CCMenu* menu = CCMenu::create();
		addChild(menu);
		CCMenuItem* noquanItem = CCMenuItemImage::create(IMAGE_anniuTouch, IMAGE_blankItem);
		CCMenuItem* quanItem = CCMenuItemImage::create(IMAGE_quanTouch, IMAGE_blankItem);
		menu->addChild(noquanItem);
		menu->addChild(quanItem);

		menu->alignItemsVerticallyWithPadding(50);


		quanItem->setTarget(this, menu_selector(MenuLayer::quanTouchGame));
		noquanItem->setTarget(this, menu_selector(MenuLayer::noquanTouchGame));
	}
	else
		_item->setVisible(true);
}

void MenuLayer::quanTouchGame(CCObject*)
{
	changeScene(GameLayer, _mapIndex + 1, true);
}
void MenuLayer::noquanTouchGame(CCObject*)
{
	changeScene(GameLayer, _mapIndex + 1, false);
}

void MenuLayer::initCheck()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	_item = CCMenuItemImage::create(IMAGE_tishi, IMAGE_tishi2);
	menu->addChild(_item);
	_item->setTarget(this, menu_selector(MenuLayer::checkCallback));
	_item->setVisible(false);
}

void MenuLayer::checkCallback(CCObject* sender)
{
	_item->setVisible(false);
}

void MenuLayer::initReturn()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_back_normal_white, IMAGE_back_press_white);
	menu->addChild(item);
	item->setTarget(this, menu_selector(MenuLayer::rCallback));
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 2, -winSize.height / 2 + item->getContentSize().height / 2));
}
void MenuLayer::rCallback(CCObject*)
{
	changeScene(StartLayer,false);
}

void MenuLayer::initSwitchLevel()
{
	//��ȡxml�е��ļ� createWithContentsOfFile  
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);//�����ֵ� Dictionary�ֵ�

	//ͨ����ֵ����ȡ����  
	const CCString* string = dict->valueForKey("item");//��ֵҲ��const char*
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCArray* arr = CCArray::create();
	for (int i = 1; i <= 20; i++)
	{
		char* value = Util::format(i, string->getCString());
		CCMenuItemFont* subItem = CCMenuItemFont::create(value);//Ĭ������32
		/*
		������һ���ص㣬�ʼ��  setFontSize�������ÿ�ε�һ��Item�����С����32���ȵ��ڶ������壬��С�Ż��
		��ʼ����Ϊ��Toggle�������⣬��������CCMenuItemFontԴ��ŷ���
		set default font size		������Ĭ�������С
		static void setFontSize(unsigned int s);  ���һ��Ǿ�̬�ģ������֪����һ�δ���������϶���32����һ��Ĭ������Ż�ı�
		setFontSizeObj�������ֻ�Ǹı䵱ǰ���壬û�иı�Ĭ������
		*/
		subItem->setFontSizeObj(160);//32*5

		subItem->setColor(ccc3(255, 0, 0));
		//subItem->setPosition(ccp(0, 300));//��仰�����ð�����Ϊɶ��������Ϊû��ֱ�Ӽ���menu���棬���Ǽ��������У�

		arr->addObject(subItem);//����������װ��  Item
	}
	CCMenuItemToggle* itemToggle = CCMenuItemToggle::createWithTarget(this, menu_selector(MenuLayer::Toggle), arr);
	menu->addChild(itemToggle);

	//������ȥ��һ��������item
	menu->addChild(
		CCMenuItemImage::create(IMAGE_go, IMAGE_go2, this, menu_selector(MenuLayer::Go))
		);

	menu->alignItemsVerticallyWithPadding(50);
}

void MenuLayer::initJumpSprite()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	for (int i = 1; i <= 5; i++)
	{
		/*CCSprite* s = CCSprite::create(Util::format(i, "switchWord", ".png"));
		addChild(s);*/
		//CCMoveTo* move = CCMoveTo::create(1, ccp(winSize.width / 2 - 260 * 3 + i * 260, winSize.height - 160));
		//s->runAction(CCSequence::create(move, CCRotateBy::create(1, 2 * 360), NULL));

		CCMenuItem* item = CCMenuItemImage::create(Util::format(i, IMAGE_switchWord, ".png"), Util::format(i, IMAGE_switchWord, ".png"));
		menu->addChild(item);
		item->setTarget(this, menu_selector(MenuLayer::rotateItem));
		item->setPosition(ccp(-winSize.width / 2, -winSize.height / 2));

		CCMoveTo* move = CCMoveTo::create(1, ccp(- 260 * 3 + i * 260, winSize.height/2 - 160));

		item->runAction(CCSequence::create(move, CCRotateBy::create(1, 2 * 360), NULL));
	}
}

void MenuLayer::initStore()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_store1, IMAGE_store2);
	menu->addChild(item);
	item->setPosition(ccp(0, -winSize.height / 3));
	item->setTarget(this, menu_selector(MenuLayer::AppStore));
	item->setScale(1.2);
}

void MenuLayer::rotateItem(CCObject* sender)
{
	CCMenuItem* item = (CCMenuItem*)sender;
	item->runAction(CCRotateBy::create(1, 360));
}

void MenuLayer::AppStore(CCObject* sender)
{
	changeScene(PayLayer);
}